#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "box2dwrapper/box2d_object.hpp"
#include "box2dwrapper/box2d_world_interface.hpp"
#include "shape.hpp"
#include <game_object.hpp>

class Bullet : public jt::GameObject {
public:
    explicit Bullet(std::shared_ptr<jt::Box2DWorldInterface> const& world);

    void setIsLeft(bool isLeft);

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    bool m_isLeft { false };

    std::shared_ptr<jt::Box2DWorldInterface> m_world;
    std::shared_ptr<jt::Shape> m_shape;

    std::shared_ptr<jt::Box2DObject> m_physicsObject;
};

#endif // ENEMY_HPP
