#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include "heart.hpp"
#include "screeneffects/trailing_circles.hpp"
#include "screeneffects/wind_particles.hpp"
#include "text.hpp"
#include <box2dwrapper/box2d_world_interface.hpp>
#include <bullet.hpp>
#include <bullet_spawn_info.hpp>
#include <bullet_spawner.hpp>
#include <game_state.hpp>
#include <object_group.hpp>
#include <player.hpp>
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class Line;
class Shape;
class Sprite;
class Vignette;
} // namespace jt

class Hud;

class StateGame : public jt::GameState {
public:
    StateGame();
    void createBackground(float w, float h);

    std::string getName() const override;

private:
    std::shared_ptr<jt::Shape> m_backgroundL;
    std::shared_ptr<jt::Shape> m_backgroundR;
    std::shared_ptr<jt::Vignette> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };

    bool m_running { true };
    bool m_hasEnded { false };

    int m_scoreP1 { 0 };
    int m_scoreP2 { 0 };

    std::shared_ptr<jt::WindParticles> m_windL { nullptr };
    std::shared_ptr<jt::WindParticles> m_windR { nullptr };
    float m_windScaleFactor { 10.0f };

    std::shared_ptr<Player> m_playerL { nullptr };
    std::shared_ptr<Player> m_playerR { nullptr };
    std::shared_ptr<jt::TrailingCircles> m_trailingCirclesL { nullptr };
    std::shared_ptr<jt::TrailingCircles> m_trailingCirclesR { nullptr };

    std::shared_ptr<jt::Line> m_line { nullptr };

    std::vector<BulletSpawnInfo> m_bulletSpawnInfos;
    std::shared_ptr<jt::ObjectGroup<Bullet>> m_bullets {};
    BulletSpawner m_bulletSpawner;
    std::vector<std::function<void()>> m_spawnPatterns;

    float m_health { 100.0f };
    float m_spawnTimer { 1.0f };
    float m_velocityMultiplier { 1.0f };

    std::shared_ptr<jt::ObjectGroup<Heart>> m_hearts;

    float m_spawnHeartTimer { 20.0f };

    void onCreate() override;
    void onEnter() override;
    void playerTakeDamage();
    void updateShotCollisions(float elapsed);
    void createSpawnPatterns();
    void spawnNewBullets(float elapsed);
    void updateHearts(float elapsed);
    void onUpdate(float const elapsed) override;
    void onDraw() const override;

    void endGame();
    void createPlayer();
    void updateBulletSpawns(float elapsed);
    int getStage() const;
};

#endif
