#ifndef HEART_HPP
#define HEART_HPP

#include "animation.hpp"
#include <box2dwrapper/box2d_object.hpp>
#include <game_object.hpp>

class Heart : public jt::GameObject {
public:
    void setIsLeft(bool isLeft);
    [[nodiscard]] jt::Vector2f getPosition() const;

    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::shared_ptr<jt::Animation> m_animation { nullptr };

    bool m_hasStartedBlinking { false };
    bool m_hasBeenPickedUp { false };

    jt::Vector2f m_position;
    bool m_isLeft { false };

    std::shared_ptr<jt::Sprite> m_glowOverlay { nullptr };
};

#endif // HEART_HPP
