#ifndef BULLET_SPAWNER_HPP
#define BULLET_SPAWNER_HPP

#include <bullet_spawn_info.hpp>
#include <vector>

class BulletSpawner {
public:
    explicit BulletSpawner(std::vector<BulletSpawnInfo>& bulletInfos);
    void spawnHorizontalLine(bool isLeft, float delay);
    void spawnHorizontalLineMovingDownWithRandomMiss(bool isLeft, float delay);
    void spawnVerticalLineWithRandomMiss(bool isLeft, float delay);
    void spawnSingleMovingDown(bool isLeft, float delay, float velocityFactor = 1.0f);
    void spawnSingleMovingRight(bool isLeft, float delay, float velocityFactor = 1.0f);
    void spawnSingleMovingLeft(bool isLeft, float delay, float velocityFactor = 1.0f);

private:
    std::vector<BulletSpawnInfo>& m_bulletSpawnInfos;
};

#endif // BULLET_SPAWNER_HPP
