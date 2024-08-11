#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "animation.hpp"
#include "box2dwrapper/box2d_object.hpp"
#include "box2dwrapper/box2d_world_interface.hpp"
#include "shape.hpp"
#include <game_object.hpp>

class Bullet : public jt::GameObject {
public:
    explicit Bullet(std::shared_ptr<jt::Box2DWorldInterface> const& world);

    void setIsLeft(bool isLeft);

    std::weak_ptr<jt::Box2DObject> getPhysicsObject();
    void setAnimName(std::string const& string);
    bool getIsLeft() const;

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    bool m_isLeft { false };
    std::string m_animName {};

    std::shared_ptr<jt::Box2DWorldInterface> m_world;
    std::shared_ptr<jt::Animation> m_animation;

    std::shared_ptr<jt::Box2DObject> m_physicsObject;
};

#endif // ENEMY_HPP
