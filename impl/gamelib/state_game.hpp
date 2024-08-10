#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

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

    std::string getName() const override;

private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Vignette> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };

    bool m_running { true };
    bool m_hasEnded { false };

    int m_scoreP1 { 0 };
    int m_scoreP2 { 0 };

    std::shared_ptr<Player> m_playerL { nullptr };
    std::shared_ptr<Player> m_playerR { nullptr };

    std::shared_ptr<jt::Line> m_line { nullptr };

    std::vector<BulletSpawnInfo> m_bulletSpawnInfos;
    std::shared_ptr<jt::ObjectGroup<Bullet>> m_bullets {};
    BulletSpawner m_bulletSpawner;

    void onCreate() override;
    void onEnter() override;
    void updateShotCollisions(float elapsed);
    void onUpdate(float const elapsed) override;
    void onDraw() const override;

    void endGame();
    void createPlayer();
    void updateBulletSpawns(float elapsed);
};

#endif
