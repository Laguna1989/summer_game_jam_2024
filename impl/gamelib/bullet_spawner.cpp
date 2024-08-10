
#include "bullet_spawner.hpp"

#include "random/random.hpp"

BulletSpawner::BulletSpawner(std::vector<BulletSpawnInfo>& bulletInfos)
    : m_bulletSpawnInfos(bulletInfos)
{
}

void BulletSpawner::spawnHorizontalLine(bool isLeft, float delay)
{
    int numberOfSpawn = 8;
    for (auto i = 0; i != numberOfSpawn; ++i) {
        BulletSpawnInfo bsi;
        bsi.position = jt::Vector2f { 160.0f / numberOfSpawn * (i + 0.5f), -10.0f };
        bsi.velocity = jt::Vector2f { 0.0f, GP::ShotVelocity() };
        bsi.isLeft = isLeft;
        bsi.delay = delay;
        if (isLeft) {
            bsi.animationName = "assets/Lightball.aseprite";
        }
        m_bulletSpawnInfos.push_back(bsi);
    }
}

void BulletSpawner::spawnHorizontalLineMovingDownWithRandomMiss(bool isLeft, float delay)
{
    int numberOfSpawn = 8;
    int missID = jt::Random::getInt(0, numberOfSpawn - 1);
    for (auto i = 0; i != numberOfSpawn; ++i) {
        if (i == missID) {
            continue;
        }
        BulletSpawnInfo bsi;
        bsi.position
            = jt::Vector2f { GP::GetScreenSize().x / 2.0f / numberOfSpawn * (i + 0.5f), -10.0f };
        bsi.velocity = jt::Vector2f { 0.0f, GP::ShotVelocity() };
        bsi.isLeft = isLeft;
        bsi.delay = delay;
        if (isLeft) {
            bsi.animationName = "assets/Lightball.aseprite";
        }
        m_bulletSpawnInfos.push_back(bsi);
    }
}

void BulletSpawner::spawnVerticalLineWithRandomMiss(bool isLeft, float delay)
{
    int numberOfSpawn = 8;
    int missID = jt::Random::getInt(0, numberOfSpawn - 1);
    for (auto i = 0; i != numberOfSpawn; ++i) {
        if (i == missID) {
            continue;
        }
        BulletSpawnInfo bsi;
        bsi.position = jt::Vector2f { -10.0f, GP::GetScreenSize().y / numberOfSpawn * i };
        bsi.velocity = jt::Vector2f { GP::ShotVelocity(), 0.0f };
        bsi.isLeft = isLeft;
        bsi.delay = delay;
        bsi.animationName = "assets/doc.aseprite";
        if (isLeft) {
            bsi.animationName = "assets/Fire.aseprite";
        }

        m_bulletSpawnInfos.push_back(bsi);
    }
}

void BulletSpawner::spawnSingleMovingDown(bool isLeft, float delay, float velocityFactor)
{
    int numberOfSpawn = 8;
    int missID = jt::Random::getInt(0, numberOfSpawn - 1);
    for (auto i = 0; i != numberOfSpawn; ++i) {
        if (i != missID) {
            continue;
        }
        BulletSpawnInfo bsi;
        bsi.position
            = jt::Vector2f { GP::GetScreenSize().x / 2.0f / numberOfSpawn * (i + 0.5f), -10.0f };
        bsi.velocity = jt::Vector2f { 0.0f, GP::ShotVelocity() * velocityFactor };
        bsi.isLeft = isLeft;
        bsi.delay = delay;
        if (isLeft) {
            bsi.animationName = "assets/Lightball.aseprite";
        }
        m_bulletSpawnInfos.push_back(bsi);
    }
}

void BulletSpawner::spawnSingleMovingRight(bool isLeft, float delay, float velocityFactor)
{
    int numberOfSpawn = 8;
    int missID = jt::Random::getInt(0, numberOfSpawn - 1);
    for (auto i = 0; i != numberOfSpawn; ++i) {
        if (i != missID) {
            continue;
        }
        BulletSpawnInfo bsi;
        bsi.position = jt::Vector2f { -10.0f, GP::GetScreenSize().y / numberOfSpawn * i };
        bsi.velocity = jt::Vector2f { GP::ShotVelocity() * velocityFactor, 0.0f };
        bsi.isLeft = isLeft;
        bsi.delay = delay;
        bsi.animationName = "assets/doc.aseprite";
        if (isLeft) {
            bsi.animationName = "assets/Fire.aseprite";
        }

        m_bulletSpawnInfos.push_back(bsi);
    }
}

void BulletSpawner::spawnSingleMovingLeft(bool isLeft, float delay, float velocityFactor)
{
    int numberOfSpawn = 8;
    int missID = jt::Random::getInt(0, numberOfSpawn - 1);
    for (auto i = 0; i != numberOfSpawn; ++i) {
        if (i != missID) {
            continue;
        }
        BulletSpawnInfo bsi;
        bsi.position = jt::Vector2f { GP::GetScreenSize().x / 2.0f,
            GP::GetScreenSize().y / numberOfSpawn * i };
        bsi.velocity = jt::Vector2f { -GP::ShotVelocity() * velocityFactor, 0.0f };
        bsi.isLeft = isLeft;
        bsi.delay = delay;
        bsi.animationName = "assets/clippy.aseprite";
        if (isLeft) {
            bsi.animationName = "assets/Green.aseprite";
        }

        m_bulletSpawnInfos.push_back(bsi);
    }
}
