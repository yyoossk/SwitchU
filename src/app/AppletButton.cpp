#include "AppletButton.hpp"
#include "../core/Renderer.hpp"

namespace ui {

AppletButton::AppletButton() {
    setCornerRadius(16.f);
    setPadding(6.f);
}

void AppletButton::activate() {
    if (m_action) m_action();
}

void AppletButton::onContentRender(Renderer& ren) {
    if (!m_icon || !m_icon->valid()) return;

    Rect cr = contentRect();

    // Draw icon centered inside the content area, preserving aspect ratio
    float iconSz = std::min(cr.width, cr.height);
    float ix = cr.x + (cr.width  - iconSz) * 0.5f;
    float iy = cr.y + (cr.height - iconSz) * 0.5f;

    ren.drawTextureRounded(m_icon, {ix, iy, iconSz, iconSz},
                           cornerRadius() - 4.f,
                           Color::white().withAlpha(m_opacity));
}

} // namespace ui
