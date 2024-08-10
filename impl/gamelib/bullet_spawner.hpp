#ifndef BULLET_SPAWNER_HPP
#define BULLET_SPAWNER_HPP

#include <bullet_spawn_info.hpp>
#include <vector>

class BulletSpawner {
public:
    explicit BulletSpawner(std::vector<BulletSpawnInfo>& bulletInfos);
    void spawnHorizontalLine(bool isLeft, float delay);
    void spawnHorizontalLineWithRandomMiss(bool isLeft, float delay);
    void spawnVerticalLineWithRandomMiss(bool isLeft, float delay);
    void spawnSingleRandomHorizontal(bool isLeft, float delay);
    void spawnSingleRandomVertical(bool isLeft, float delay);

private:
    std::vector<BulletSpawnInfo>& m_bulletSpawnInfos;
};

#endif // BULLET_SPAWNER_HPP
