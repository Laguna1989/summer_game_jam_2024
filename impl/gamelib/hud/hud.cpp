#include "hud.hpp"
#include <color/color.hpp>
#include <drawable_helpers.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <hud/score_display.hpp>

std::shared_ptr<ObserverInterface<int>> Hud::getObserverHealth() const { return m_healthDisplay; }

std::shared_ptr<ObserverInterface<int>> Hud::getObserverScore() const { return m_scoreP2Display; }

void Hud::doCreate()
{
    m_healthText = std::make_shared<jt::Text>();
    m_healthText = jt::dh::createText(renderTarget(), "", 16, jt::Color { 248, 249, 254 });
    m_healthText->setTextAlign(jt::Text::TextAlign::LEFT);
    m_healthText->setIgnoreCamMovement(true);
    m_healthText->setPosition({ 10, 4 });

    m_healthDisplay = std::make_shared<ScoreDisplay>(m_healthText, "Health: ");

    m_scoreP2Text = jt::dh::createText(renderTarget(), "", 16, jt::Color { 248, 249, 254 });
    m_scoreP2Text->setTextAlign(jt::Text::TextAlign::RIGHT);
    m_scoreP2Text->setIgnoreCamMovement(true);
    m_scoreP2Text->setPosition({ GP::GetScreenSize().x - 10, 4 });

    m_scoreP2Display = std::make_shared<ScoreDisplay>(m_scoreP2Text, "P2 Score: ");
}

void Hud::doUpdate(float const elapsed)
{
    m_healthText->update(elapsed);
    m_scoreP2Text->update(elapsed);
}

void Hud::doDraw() const
{
    m_healthText->draw(renderTarget());
    m_scoreP2Text->draw(renderTarget());
}
