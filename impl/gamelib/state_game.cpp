﻿#include "state_game.hpp"
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
    m_background->setColor(jt::Color { 27, 38, 50 });
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

void StateGame::playerTakeDamage()
{
    m_health -= GP::ShotDamage();
    m_hud->getObserverHealth()->notify(static_cast<int>(m_health));
    getGame()->gfx().camera().shake(0.45f, 4.0f);
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
            playerTakeDamage();
        }
    }
}

void StateGame::spawnNewBullets(float elapsed)
{
    m_spawnTimer -= elapsed;
    if (m_spawnTimer <= 0) {

        int stage = static_cast<int>(getAge()) / GP::StageTime();
        if (stage == 0) {
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 0.0f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 0.5f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 1.0f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 1.5f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 2.0f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 2.5f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 3.0f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 3.5f);
            m_spawnTimer += 2.0f;
        } else if (stage == 1) {

            bool flip = jt::Random::getChance();
            m_bulletSpawner.spawnHorizontalLineWithRandomMiss(flip, 0.0f);
            m_bulletSpawner.spawnHorizontalLineWithRandomMiss(!flip, 1.6f);
            m_spawnTimer += 1.4f;
        } else if (stage == 2) {
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 0.0f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 0.5f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 1.0f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 1.5f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 2.0f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 2.5f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 3.0f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 3.5f);
            m_spawnTimer += 2.5f;
        } else if (stage == 3) {
            m_bulletSpawner.spawnHorizontalLineWithRandomMiss(true, 0.0f);
            m_bulletSpawner.spawnHorizontalLineWithRandomMiss(true, 1.4f);
            m_bulletSpawner.spawnHorizontalLineWithRandomMiss(false, 2.8f);
            m_bulletSpawner.spawnHorizontalLineWithRandomMiss(false, 4.2f);
            m_spawnTimer += 4.0f;
        } else if (stage == 4) {
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 0.0f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 0.4f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 0.8f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 1.2f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 1.6f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 2.0f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 2.4f);
            m_bulletSpawner.spawnSingleRandomHorizontal(jt::Random::getChance(), 2.8f);

            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 0.0f + 0.2f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 0.4f + 0.2f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 0.8f + 0.2f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 1.2f + 0.2f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 1.6f + 0.2f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 2.0f + 0.2f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 2.4f + 0.2f);
            m_bulletSpawner.spawnSingleRandomVertical(jt::Random::getChance(), 2.8f + 0.2f);
        }
        m_spawnTimer += 4.0f;
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
    m_velocityMultiplier = 1.0f + getAge() / (GP::StageTime() * 10.0f);
    for (auto& bsi : m_bulletSpawnInfos) {
        bsi.delay -= elapsed;
        if (bsi.delay <= 0.0f) {
            auto bullet = std::make_shared<Bullet>(m_world);
            bullet->setAnimName(bsi.animationName);
            add(bullet);
            bullet->getPhysicsObject().lock()->setVelocity(bsi.velocity * m_velocityMultiplier);
            ;
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
        // if (getGame()->input().keyboard()->justPressed(jt::KeyCode::X)) {
        //     m_bulletSpawner.spawnHorizontalLineWithRandomMiss(true, 0.0f);
        //     m_bulletSpawner.spawnHorizontalLineWithRandomMiss(false, 2.0f);
        // }
        // if (getGame()->input().keyboard()->justPressed(jt::KeyCode::Y)) {
        //     m_bulletSpawner.spawnHorizontalLineWithRandomMiss(true, 0.0f);
        //     m_bulletSpawner.spawnVerticalLineWithRandomMiss(false, 0.0f);
        // }
        if (getGame()->input().keyboard()->justPressed(jt::KeyCode::D)) { }
        if (getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)
            && getGame()->input().keyboard()->pressed(jt::KeyCode::Escape)) {
            endGame();
        }

        spawnNewBullets(elapsed);

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
