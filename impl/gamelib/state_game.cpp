#include "state_game.hpp"
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

void StateGame::onCreate()
{
    m_world = std::make_shared<jt::Box2DWorldImpl>(jt::Vector2f { 0.0f, 0.0f });

    float const w = static_cast<float>(GP::GetScreenSize().x);
    float const h = static_cast<float>(GP::GetScreenSize().y);

    using jt::Shape;

    m_background = std::make_shared<Shape>();
    m_background->makeRect({ w, h }, textureManager());
    m_background->setColor(GP::PaletteBackground());
    m_background->setIgnoreCamMovement(true);
    m_background->update(0.0f);

    m_backgroundR = std::make_shared<Shape>();
    m_backgroundR->makeRect({ w, h }, textureManager());
    m_backgroundR->setColor(jt::Color { 15, 56, 15 });
    m_backgroundR->setPosition({ w / 2, 0.0f });
    m_backgroundR->setIgnoreCamMovement(true);
    m_backgroundR->update(0.0f);

    createPlayer();

    m_bullets = std::make_shared<jt::ObjectGroup<Bullet>>();
    add(m_bullets);

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);

    m_line = std::make_shared<jt::Line>(jt::Vector2f { 0.0f, GP::GetScreenSize().y });
    m_line->setPosition({ GP::GetScreenSize().x / 2.0f, 0.0f });

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);
}

void StateGame::onEnter()
{
    m_health = 100.0f;
    m_hud->getObserverHealth()->notify(static_cast<int>(m_health));
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
        if (minDist < 7.5f * 7.5f) {
            b->kill();
            m_health -= GP::ShotDamage();
            m_hud->getObserverHealth()->notify(static_cast<int>(m_health));
        }
    }
}

void StateGame::createPlayer()
{
    m_playerL = std::make_shared<Player>(m_world);
    m_playerL->setLeft(true);
    add(m_playerL);
    m_playerR = std::make_shared<Player>(m_world);
    m_playerR->setLeft(false);
    add(m_playerR);
}

void StateGame::updateBulletSpawns(float const elapsed)
{
    for (auto& bsi : m_bulletSpawnInfos) {
        bsi.delay -= elapsed;
        if (bsi.delay <= 0.0f) {
            auto bullet = std::make_shared<Bullet>(m_world);
            bullet->setAnimName(bsi.animationName);
            add(bullet);
            bullet->getPhysicsObject().lock()->setVelocity(bsi.velocity);
            bullet->getPhysicsObject().lock()->setPosition(bsi.position);
            bullet->setIsLeft(bsi.isLeft);
            m_bullets->push_back(bullet);
        }
    }

    std::erase_if(m_bulletSpawnInfos, [](auto const& bsi) { return bsi.delay <= 0.0f; });
}

void StateGame::onUpdate(float const elapsed)
{
    if (m_running) {
        m_world->step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());

        // update game logic here
        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::X)) {
            m_bulletSpawner.spawnHorizontalLineWithRandomMiss(true, 0.0f);
            m_bulletSpawner.spawnHorizontalLineWithRandomMiss(false, 2.0f);
        }
        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::Y)) {
            m_bulletSpawner.spawnHorizontalLineWithRandomMiss(true, 0.0f);
            m_bulletSpawner.spawnVerticalLineWithRandomMiss(false, 0.0f);
        }
        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::D)) { }
        if (getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)
            && getGame()->input().keyboard()->pressed(jt::KeyCode::Escape)) {
            endGame();
        }

        updateBulletSpawns(elapsed);

        updateShotCollisions(elapsed);

        if (m_health <= 0.0f) {
            endGame();
        }
    }

    m_background->update(elapsed);
    m_backgroundR->update(elapsed);
    m_vignette->update(elapsed);
    m_line->update(elapsed);
}

void StateGame::onDraw() const
{
    m_background->draw(renderTarget());
    m_backgroundR->draw(renderTarget());
    drawObjects();
    m_vignette->draw();
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
