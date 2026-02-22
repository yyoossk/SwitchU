#pragma once
#include "../ui/GlassWidget.hpp"
#include "../core/Texture.hpp"
#include "../core/Types.hpp"
#include <functional>
#include <string>

namespace ui {

class Renderer;

/// Small glass-backed button with an icon texture, used for system applets.
class AppletButton : public GlassWidget {
public:
    AppletButton();

    void setIcon(Texture* tex)        { m_icon = tex; }
    Texture* icon() const             { return m_icon; }

    void setLabel(const std::string& l) { m_label = l; }
    const std::string& label() const    { return m_label; }

    /// Action executed when the button is activated (tap / A press).
    void setAction(std::function<void()> fn) { m_action = std::move(fn); }
    void activate();

    /// Hit test (screen coordinates).
    bool hitTest(float sx, float sy) const { return m_rect.contains(sx, sy); }

protected:
    void onContentRender(Renderer& ren) override;

private:
    Texture*              m_icon = nullptr;
    std::string           m_label;
    std::function<void()> m_action;
};

} // namespace ui
