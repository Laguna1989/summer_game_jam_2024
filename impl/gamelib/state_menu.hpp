﻿#ifndef GAME_STATE_MENU_HPP
#define GAME_STATE_MENU_HPP

#include <game_state.hpp>

namespace jt {
class MarioCloudsHorizontal;

class Text;
class Shape;
class Animation;
class Sprite;
class Vignette;

} // namespace jt

class StateMenu : public jt::GameState {
public:
    std::string getName() const override;

    void setScore(int s) { m_score = s; };

private:
    std::shared_ptr<jt::Animation> m_background;
    std::shared_ptr<jt::MarioCloudsHorizontal> m_clouds;
    std::shared_ptr<jt::Animation> m_logo;
    std::shared_ptr<jt::Animation> m_unterLogo;

    std::shared_ptr<jt::Animation> m_titleAnimation;
    std::shared_ptr<jt::Text> m_textStart;
    std::shared_ptr<jt::Text> m_textExplanation;
    std::shared_ptr<jt::Text> m_textCredits;
    std::shared_ptr<jt::Text> m_textVersion;
    std::shared_ptr<jt::Shape> m_overlay;
    std::shared_ptr<jt::Vignette> m_vignette;

    std::shared_ptr<jt::Text> m_highscoreText;

    int m_score { 0 };

    bool m_started { false };
    float m_timerToStart { 0.0f };
    bool m_switched { false };

    void onCreate() override;
    void onEnter() override;
    void createVignette();
    void createShapes();

    void createMenuText();
    void createTextCredits();
    void createTextStart();

    void createTweens();
    void createInstructionTweenColor1();
    void createInstructionTweenColor2();
    void createTweenTitleAlpha();
    void createTweenOverlayAlpha();
    void createTweenCreditsPosition();
    void createTweenExplanation();

    void onUpdate(float const elapsed) override;
    void updateDrawables(float const& elapsed);
    void checkForTransitionToStateGame();
    void startTransitionToStateGame();

    void onDraw() const override;
    void createTextExplanation();
};

#endif
