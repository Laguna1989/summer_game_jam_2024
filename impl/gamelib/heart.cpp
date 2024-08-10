#include "heart.hpp"

#include "random/random.hpp"

void Heart::setIsLeft(bool isLeft) { m_isLeft = isLeft; }

jt::Vector2f Heart::getPosition() const { return m_position; }

void Heart::doCreate()
{
    m_animation = std::make_shared<jt::Animation>();
    if (m_isLeft) {
        // TODO
        m_animation->loadFromAseprite("assets/Dragon.aseprite", textureManager());
    } else {
        // TODO
        m_animation->loadFromAseprite("assets/Dragon.aseprite", textureManager());
    }
    m_animation->play("loop");

    m_position = jt::Random::getRandomPointIn(
        jt::Rectf { 10.0f + (m_isLeft ? 0.0f : 160.0f), 10.0f, 140.0f, 220.0f });
    m_animation->setPosition(m_position);
    m_animation->setOffset(jt::OffsetMode::CENTER);
}

void Heart::doUpdate(float const elapsed)
{
    m_animation->update(elapsed);

    auto const maxTimeAlive = 10.0f;
    if (m_hasStartedBlinking) {
        if (getAge() > maxTimeAlive * 0.8f) {
            m_hasStartedBlinking = true;
            m_animation->flicker(maxTimeAlive * 0.2f, 0.1f);
        }
    }
    if (getAge() > maxTimeAlive) {
        kill();
    }
}

void Heart::doDraw() const { m_animation->draw(renderTarget()); }
