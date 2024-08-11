#include "wind_particles.hpp"
#include <random/random.hpp>
#include <screeneffects/screen_wrap.hpp>
#include <stdexcept>

jt::WindParticles::WindParticles(
    jt::Vector2f const& screenSize, std::vector<jt::Color> const& colors)
    : m_screenSize { screenSize }
    , m_colors { colors }
{
    if (m_colors.empty()) {
        throw std::invalid_argument {
            "colors list for WindParticles must contain at least one entry,"
        };
    }
}

void jt::WindParticles::doCreate()
{
    for (auto i = 0u; i != m_numberOfParticles; ++i) {
        auto s = std::make_shared<jt::Shape>();
        s->makeRect(m_shapeSize, textureManager());
        s->setPosition(jt::Random::getRandomPointIn(m_screenSize));
        s->setScreenSizeHint(m_screenSize);

        auto const index = jt::Random::getInt(0, m_colors.size() - 1);
        s->setColor(m_colors.at(index));

        m_shapes.push_back(s);
        m_factors.push_back(jt::Random::getFloatGauss(1, 0.1f));
    }
}

void jt::WindParticles::doUpdate(float const elapsed)
{
    for (auto i = 0u; i != m_shapes.size(); ++i) {
        auto& s = m_shapes.at(i);
        s->setScale(m_scale);
        auto p = s->getPosition();
        //        float const f =
        p += m_windDirection * elapsed * m_windSpeedFactor * m_factors.at(i);
        s->setPosition(p);
        jt::wrapOnScreen(*s.get(), std::max(m_shapeSize.x, m_shapeSize.y));
        s->update(elapsed);
    }
}

void jt::WindParticles::doDraw() const
{
    if (!m_enabled) {
        return;
    }
    for (auto const& s : m_shapes) {
        s->draw(renderTarget());
    }
}

void jt::WindParticles::setNumberOfParticles(std::size_t numberOfParticles)
{
    m_numberOfParticles = numberOfParticles;
}

void jt::WindParticles::setWindDirection(jt::Vector2f const& direction)
{
    m_windDirection = direction;
}

void jt::WindParticles::setShapeSize(jt::Vector2f const& shapeSize) { m_shapeSize = shapeSize; }

void jt::WindParticles::setScale(jt::Vector2f const& scale) { m_scale = scale; }

void jt::WindParticles::setEnabled(bool enabled) { m_enabled = enabled; }

void jt::WindParticles::setZ(int zLayer)
{
    for (auto const& s : m_shapes) {
        s->setZ(zLayer);
    }
}
