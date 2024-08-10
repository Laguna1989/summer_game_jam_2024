#ifndef BULLET_SPAWN_INFO_HPP
#define BULLET_SPAWN_INFO_HPP
#include "vector.hpp"
#include <game_properties.hpp>

struct BulletSpawnInfo {
    bool isLeft { true };
    float delay { 0.0f };
    jt::Vector2f position { 32.0f, 0.0f };
    jt::Vector2f velocity { 0.0f, GP::ShotVelocity() };
};

#endif // BULLET_SPAWN_INFO_HPP
