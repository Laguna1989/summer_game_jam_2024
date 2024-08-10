#include "heart.hpp"

#include "random/random.hpp"
#include "sprite.hpp"

void Heart::setIsLeft(bool isLeft) { m_isLeft = isLeft; }

jt::Vector2f Heart::getPosition() const { return m_position; }

void Heart::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();
    if (m_isLeft) {
        // TODO
        m_animation->loadFromAseprite("assets/dragon_herz.aseprite", textureManager());
    } else {

        m_animation->loadFromAseprite("assets/digital_herz.aseprite", textureManager());
    }
    m_animation->play("loop");

    m_position = jt::Random::getRandomPointIn(
        jt::Rectf { 10.0f + (m_isLeft ? 0.0f : 160.0f), 10.0f, 140.0f, 220.0f });
    m_animation->setPosition(m_position);
    m_animation->setOffset(jt::OffsetMode::CENTER);
    m_animation->flash(0.25f);

    m_glowOverlay = std::make_shared<jt::Sprite>("#g#40#150", textureManager());
    m_glowOverlay->setOffset(jt::OffsetMode::CENTER);
    m_glowOverlay->setPosition(m_position);
}

void Heart::doUpdate(float const elapsed)
{
    m_animation->update(elapsed);
    m_glowOverlay->update(elapsed);

    auto const maxTimeAlive = 10.0f;
    auto const flashTime = maxTimeAlive * 0.7f;
    if (!m_hasStartedBlinking) {
        if (getAge() > flashTime) {
            m_hasStartedBlinking = true;
            m_animation->flicker(maxTimeAlive - flashTime, 0.2f);
            m_glowOverlay->flicker(maxTimeAlive - flashTime, 0.2f);
        }
    }
    if (getAge() > maxTimeAlive) {
        kill();
    }
}

void Heart::doDraw() const
{
    m_glowOverlay->draw(renderTarget());
    m_animation->draw(renderTarget());
}
