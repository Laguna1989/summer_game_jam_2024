#ifndef INC_JAMTEMPLATE_WIND_PARTICLES_HPP
#define INC_JAMTEMPLATE_WIND_PARTICLES_HPP

#include "game_object.hpp"
#include "shape.hpp"

namespace jt {
class WindParticles : public jt::GameObject {

public:
    WindParticles(jt::Vector2f const& screenSize, std::vector<jt::Color> const& colors);
    float m_windSpeedFactor { 1.0f };

    void setNumberOfParticles(std::size_t numberOfParticles);
    void setWindDirection(jt::Vector2f const& direction);
    void setShapeSize(jt::Vector2f const& shapeSize);
    void setScale(jt::Vector2f const& scale);

    void setEnabled(bool enabled);

    void setZ(int zLayer);

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    jt::Vector2f m_screenSize { 0.0f, 0.0f };
    std::vector<jt::Color> m_colors;
    std::size_t m_numberOfParticles { 100u };
    jt::Vector2f m_shapeSize { 8.0f, 2.0f };
    jt::Vector2f m_scale { 1.0f, 1.0f };

public:
private:
    std::vector<std::shared_ptr<jt::Shape>> m_shapes;
    std::vector<float> m_factors;

    jt::Vector2f m_windDirection { -150.0f, 0.0f };

    bool m_enabled { true };
};

} // namespace jt

#endif // INC_JAMTEMPLATE_WIND_PARTICLES_HPP
