#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "animation.hpp"
#include "shape.hpp"
#include <box2dwrapper/box2d_object.hpp>
#include <game_object.hpp>

class Player : public jt::GameObject {
public:
    explicit Player(std::shared_ptr<jt::Box2DWorldInterface> world);
    void setLeft(bool isLeft);

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
    jt::Vector2f getPosition() const;

    std::shared_ptr<jt::Box2DWorldInterface> m_world;

    bool m_isLeft { false };

    std::shared_ptr<jt::Animation> m_animation { nullptr };
    std::shared_ptr<jt::Box2DObject> m_physicsObject;
};

#endif // PLAYER_HPP
