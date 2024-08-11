#include "state_game.hpp"

#include "drawable_helpers.hpp"
#include "line.hpp"
#include "math_helper.hpp"
#include "random/random.hpp"
#include <box2dwrapper/box2d_world_impl.hpp>
#include <bullet.hpp>
#include <color/color.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <hud/hud.hpp>
#include <screeneffects/vignette.hpp>
#include <shape.hpp>
#include <state_menu.hpp>

StateGame::StateGame()
    : m_bulletSpawner(m_bulletSpawnInfos)
{
}

void StateGame::createBackground(float const w, float const h)
{
    m_backgroundL = std::make_shared<jt::Shape>();
    m_backgroundL->makeRect({ w, h }, textureManager());
    m_backgroundL->setColor(jt::Color { 27, 38, 50 });
    m_backgroundL->setIgnoreCamMovement(true);
    m_backgroundL->update(0.0f);

    m_backgroundR = std::make_shared<jt::Shape>();
    m_backgroundR->makeRect({ w, h }, textureManager());
    m_backgroundR->setColor(jt::Color { 15, 56, 15 });
    m_backgroundR->setPosition({ w / 2, 0.0f });
    m_backgroundR->setIgnoreCamMovement(true);
    m_backgroundR->update(0.0f);
}

void StateGame::onCreate()
{
    m_world = std::make_shared<jt::Box2DWorldImpl>(jt::Vector2f { 0.0f, 0.0f });

    float const w = static_cast<float>(GP::GetScreenSize().x);
    float const h = static_cast<float>(GP::GetScreenSize().y);

    using jt::Shape;

    createBackground(w, h);

    m_starsL = std::make_shared<jt::Stars>(
        20, jt::colors::White, jt::Vector2f { GP::GetScreenSize().x / 2, GP::GetScreenSize().y });
    add(m_starsL);
    m_starsL->setVelocity(jt::Vector2f { 0.0f, 10.0f });

    m_windR = std::make_shared<jt::WindParticles>(
        jt::Vector2f { GP::GetScreenSize().x, GP::GetScreenSize().y },
        std::vector<jt::Color> { jt::Color { 255, 255, 255, 80 }, jt::Color { 200, 205, 195, 120 },
            jt::Color { 130, 165, 130, 130 } });
    m_windR->setNumberOfParticles(60.0f);
    m_windR->setWindDirection({ 0.0f, 50.0f });
    m_windR->setShapeSize({ m_windScaleFactor, m_windScaleFactor });
    m_windR->setScale(jt::Vector2f { 1.0f, 3.0f } / m_windScaleFactor);
    add(m_windR);
    m_windR->m_offset = { -GP::GetScreenSize().x / 2, 0.0f };

    createPlayer();

    m_bullets = std::make_shared<jt::ObjectGroup<Bullet>>();
    add(m_bullets);

    m_hearts = std::make_shared<jt::ObjectGroup<Heart>>();
    add(m_hearts);

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);

    m_line = std::make_shared<jt::Line>(jt::Vector2f { 0.0f, GP::GetScreenSize().y });
    m_line->setPosition({ GP::GetScreenSize().x / 2.0f, 0.0f });

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);

    try {
        auto music = getGame()->audio().getPermanentSound("music");
        if (music == nullptr) {
            music = getGame()->audio().addPermanentSound("music", "event:/music");
            music->play();
        }
    } catch (std::exception const& e) {
        getGame()->logger().error(e.what(), { "menu", "music" });
    }

    createSpawnPatterns();

    m_overlay
        = jt::dh::createShapeRect(GP::GetScreenSize(), jt::colors::Transparent, textureManager());
}

void StateGame::onEnter()
{
    m_health = 100.0f;
    m_hud->getObserverHealth()->notify(static_cast<int>(m_health));
}

void StateGame::playerTakeDamage(bool isLeft)
{
    if (m_iframeTimer > 0.0f) {
        return;
    }
    m_iframeTimer = 1.0f;
    m_health -= GP::ShotDamage();
    m_hud->getObserverHealth()->notify(static_cast<int>(m_health));
    getGame()->gfx().camera().shake(0.45f, 3.5f);

    auto snd = getGame()->audio().addTemporarySound("event:/explosion");
    snd->play();
    m_overlay->flash(0.2f, jt::Color { 255, 255, 255, 100 });
    auto p = m_playerR.get();
    if (isLeft) {
        p = m_playerL.get();
    }
    p->m_animation->shake(0.75f, 5, 0.01);
    p->m_animation->flash(0.4f, jt::Color { 190, 38, 51, 255 });
}

void StateGame::updateShotCollisions(float elapsed)
{
    for (auto const& bullet : *m_bullets) {
        auto b = bullet.lock();
        auto const bp = b->getPhysicsObject().lock()->getPosition();
        auto const pp1 = m_playerL->getPosition();
        auto const pp2 = m_playerR->getPosition();

        auto const d1 = jt::MathHelper::distanceBetweenSquared(bp, pp1);
        auto const d2 = jt::MathHelper::distanceBetweenSquared(bp, pp2);
        auto minDist = std::min(d1, d2);
        auto const collisionRange = 12.0f;
        if (minDist < collisionRange * collisionRange) {
            b->kill();
            playerTakeDamage(b->getIsLeft());
        }
    }
}

void StateGame::createSpawnPatterns()
{
    m_spawnPatterns.emplace_back([this]() {
        m_bulletSpawner.spawnSingleMovingDown(jt::Random::getChance(), 0.0f);
        m_bulletSpawner.spawnSingleMovingDown(jt::Random::getChance(), 0.5f);
        m_bulletSpawner.spawnSingleMovingDown(jt::Random::getChance(), 1.0f);
        m_bulletSpawner.spawnSingleMovingDown(jt::Random::getChance(), 1.5f);
        m_bulletSpawner.spawnSingleMovingDown(jt::Random::getChance(), 2.0f);
        m_bulletSpawner.spawnSingleMovingDown(jt::Random::getChance(), 2.5f);
        m_bulletSpawner.spawnSingleMovingDown(jt::Random::getChance(), 3.0f);
        m_bulletSpawner.spawnSingleMovingDown(jt::Random::getChance(), 3.5f);
        m_spawnTimer += 2.0f;
    });
    m_spawnPatterns.emplace_back([this]() {
        const bool flip = jt::Random::getChance();
        m_bulletSpawner.spawnHorizontalLineMovingDownWithRandomMiss(flip, 0.0f);
        m_bulletSpawner.spawnHorizontalLineMovingDownWithRandomMiss(!flip, 1.9f);
        m_spawnTimer += 2.2f;
    });
    m_spawnPatterns.emplace_back([this]() {
        m_bulletSpawner.spawnSingleMovingRight(jt::Random::getChance(), 0.0f, 1.0f);
        m_bulletSpawner.spawnSingleMovingRight(jt::Random::getChance(), 0.5f, 1.0f);
        m_bulletSpawner.spawnSingleMovingRight(jt::Random::getChance(), 1.0f, 1.0f);
        m_bulletSpawner.spawnSingleMovingRight(jt::Random::getChance(), 1.5f, 1.0f);
        m_bulletSpawner.spawnSingleMovingLeft(jt::Random::getChance(), 2.0f, 1.0f);
        m_bulletSpawner.spawnSingleMovingLeft(jt::Random::getChance(), 2.5f, 1.0f);
        m_bulletSpawner.spawnSingleMovingLeft(jt::Random::getChance(), 3.0f, 1.0f);
        m_bulletSpawner.spawnSingleMovingLeft(jt::Random::getChance(), 3.5f, 1.0f);
        m_spawnTimer += 3.5f;
    });
    m_spawnPatterns.emplace_back([this]() {
        const bool flip = jt::Random::getChance();
        const bool flip2 = jt::Random::getChance();
        auto getPos = [flip2](int x) {
            if (flip2) {
                return 8 - x;
            }
            return x;
        };
        float timeFactor = 0.6f;
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(0), 0.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(1), 1.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(2), 2.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(3), 3.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(4), 4.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(5), 5.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(6), 6.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(7), 7.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(8), 8.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(8), 9.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(7), 10.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(6), 11.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(5), 12.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(4), 13.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(3), 14.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(2), 15.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(1), 16.0f * timeFactor);
        m_bulletSpawner.spawnHorizontalCorridor(flip, getPos(0), 17.0f * timeFactor);
        m_spawnTimer += 17.0f * timeFactor;
    });
    m_spawnPatterns.emplace_back([this]() {
        m_bulletSpawner.spawnHorizontalLineMovingDownWithRandomMiss(true, 0.0f);
        m_bulletSpawner.spawnHorizontalLineMovingDownWithRandomMiss(true, 1.75f);
        m_bulletSpawner.spawnHorizontalLineMovingDownWithRandomMiss(false, 3.5f);
        m_bulletSpawner.spawnHorizontalLineMovingDownWithRandomMiss(false, 5.25f);
        m_spawnTimer += 5.5f;
    });
    m_spawnPatterns.emplace_back([this]() {
        constexpr float timeBetweenSpawnsFactor = 1.5f;
        constexpr float velocityFactor = 0.75f;
        m_bulletSpawner.spawnSingleMovingDown(
            jt::Random::getChance(), timeBetweenSpawnsFactor * 0.4f, velocityFactor);
        m_bulletSpawner.spawnSingleMovingDown(
            jt::Random::getChance(), timeBetweenSpawnsFactor * 1.2f, velocityFactor);
        m_bulletSpawner.spawnSingleMovingDown(
            jt::Random::getChance(), timeBetweenSpawnsFactor * 2.0f, velocityFactor);
        m_bulletSpawner.spawnSingleMovingDown(
            jt::Random::getChance(), timeBetweenSpawnsFactor * 2.8f, velocityFactor);

        m_bulletSpawner.spawnSingleMovingRight(
            jt::Random::getChance(), timeBetweenSpawnsFactor * 0.0f + 0.2f, velocityFactor);
        m_bulletSpawner.spawnSingleMovingLeft(
            jt::Random::getChance(), timeBetweenSpawnsFactor * 0.8f + 0.2f, velocityFactor);
        m_bulletSpawner.spawnSingleMovingRight(
            jt::Random::getChance(), timeBetweenSpawnsFactor * 1.6f + 0.2f, velocityFactor);
        m_bulletSpawner.spawnSingleMovingLeft(
            jt::Random::getChance(), timeBetweenSpawnsFactor * 2.4f + 0.2f, velocityFactor);
        m_bulletSpawner.spawnSingleMovingLeft(
            jt::Random::getChance(), timeBetweenSpawnsFactor * 2.8f + 0.2f, velocityFactor);

        m_spawnTimer += 3.0f * timeBetweenSpawnsFactor;
    });
}

void StateGame::spawnNewBullets(float elapsed)
{

    m_spawnTimer -= elapsed;
    if (m_spawnTimer <= 0) {

        int stage = getStage();
        int maxStage = m_spawnPatterns.size();
        if (stage >= maxStage) {
            stage = stage % (maxStage);
        }
        m_spawnPatterns.at(stage)();
        m_spawnTimer += 4.0f;
    }
}

void StateGame::updateHearts(float elapsed)
{
    m_spawnHeartTimer -= elapsed;
    if (m_health <= 45.0f) {
        // faster hearts if below 45% health
        m_spawnHeartTimer -= elapsed;
    }

    if (m_spawnHeartTimer <= 0) {
        m_spawnHeartTimer += 25.0f;

        auto h = std::make_shared<Heart>();
        h->setIsLeft(jt::Random::getChance());
        add(h);
        m_hearts->push_back(h);
    }

    for (auto& heart : *m_hearts) {
        auto h = heart.lock();
        auto const hp = h->getPosition();
        auto const plp = m_playerL->getPosition();
        auto const prp = m_playerR->getPosition();

        auto const distL = jt::MathHelper::distanceBetweenSquared(hp, plp);
        auto const distR = jt::MathHelper::distanceBetweenSquared(hp, prp);
        auto const minDistance = std::min(distL, distR);
        auto const collisionRange = 15.0f;
        if (minDistance <= collisionRange * collisionRange) {
            h->kill();
            if (m_health >= 100.0f) {

                m_health += 5.0f;
            } else {
                m_health += 20.0f;
            }
            m_hud->getObserverHealth()->notify(m_health);
            auto snd = getGame()->audio().addTemporarySound("event:/pickup");
            snd->play();
        }
    }
}

void StateGame::createPlayer()
{
    m_trailingCirclesL = std::make_shared<jt::TrailingCircles>();
    m_trailingCirclesL->setColorMultiply(jt::Color { 100, 100, 100, 255 });
    add(m_trailingCirclesL);
    m_trailingCirclesR = std::make_shared<jt::TrailingCircles>();
    m_trailingCirclesR->setAnimationFile("assets/squares.aseprite");
    add(m_trailingCirclesR);

    m_playerL = std::make_shared<Player>(m_world);
    m_playerL->setLeft(true);
    add(m_playerL);
    m_playerR = std::make_shared<Player>(m_world);
    m_playerR->setLeft(false);
    add(m_playerR);
}

void StateGame::updateBulletSpawns(float const elapsed)
{
    m_hud->getObserverScore()->notify(getAge());

    auto const velocityOffset = std::sqrt(getAge() / (GP::StageTime() * 7.0f));
    m_velocityMultiplier = 0.65f + velocityOffset;
    m_windR->m_windSpeedFactor = m_velocityMultiplier;
    float velocityMultiplierL = m_velocityMultiplier;
    float velocityMultiplierR = m_velocityMultiplier;
    // if (m_velocityMultiplier >= 2.0f) {
    if (getStage() > m_spawnPatterns.size()) {
        velocityMultiplierL += getStage() % 2 == 0 ? 0.5f : 0.0f;
        velocityMultiplierR += getStage() % 2 == 1 ? 0.5f : 0.0f;
    }
    // }

    for (auto& bsi : m_bulletSpawnInfos) {
        bsi.delay -= elapsed;
        if (bsi.delay <= 0.0f) {
            auto bullet = std::make_shared<Bullet>(m_world);
            bullet->setAnimName(bsi.animationName);
            add(bullet);
            auto const multiplier = bsi.isLeft ? velocityMultiplierL : velocityMultiplierR;
            bullet->getPhysicsObject().lock()->setVelocity(bsi.velocity * multiplier);
            bullet->getPhysicsObject().lock()->setPosition(bsi.position);
            bullet->setIsLeft(bsi.isLeft);
            m_bullets->push_back(bullet);
        }
    }

    std::erase_if(m_bulletSpawnInfos, [](auto const& bsi) { return bsi.delay <= 0.0f; });
}

int StateGame::getStage() const
{
    int const stage = static_cast<int>(getAge()) / GP::StageTime();
    return stage;
}

void StateGame::onUpdate(float const elapsed)
{
    if (m_running) {
        m_world->step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());

        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::D)) { }
        if (getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)
            && getGame()->input().keyboard()->pressed(jt::KeyCode::Escape)) {
            endGame();
        }

        m_trailingCirclesL->setPosition(m_playerL->getPosition());
        m_trailingCirclesR->setPosition(m_playerR->getPosition());

        spawnNewBullets(elapsed);

        updateBulletSpawns(elapsed);

        updateShotCollisions(elapsed);

        updateHearts(elapsed);

        if (m_health <= 0.0f) {
            endGame();
        }
    }

    m_iframeTimer -= elapsed;
    m_backgroundL->update(elapsed);
    m_backgroundR->update(elapsed);
    m_vignette->update(elapsed);
    m_line->update(elapsed);
    m_overlay->update(elapsed);
}

void StateGame::onDraw() const
{
    m_backgroundL->draw(renderTarget());
    m_backgroundR->draw(renderTarget());
    drawObjects();
    m_vignette->draw();
    m_overlay->draw(renderTarget());
    m_line->draw(renderTarget());
    m_hud->draw();
}

void StateGame::endGame()
{
    if (m_hasEnded) {
        // trigger this function only once
        return;
    }
    m_hasEnded = true;
    m_running = false;

    auto state = std::make_shared<StateMenu>();
    // TODO set highscore
    getGame()->stateManager().switchState(state);
}

std::string StateGame::getName() const { return "State Game"; }
