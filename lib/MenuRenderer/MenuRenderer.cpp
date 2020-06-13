#include <MenuRenderer.h>

MenuRenderer::MenuRenderer(Nokia_LCD* _lcd)
{
    lcd = _lcd;
}

void MenuRenderer::render_header(const char* header) const {
    lcd->setCursor(0, 1);
    for (int i = 0; i < 14; ++i) {
        lcd->draw(LcdAssets::menuHeader, 6, true);
    }
    lcd->setInverted(true);
    lcd->setCursor(getCursorXForCenteredText(header), 1);
    lcd->print(header);
    lcd->setInverted(false);
}

void MenuRenderer::render(Menu const& menu) const {
    lcd->clear();
    render_header(menu.get_name());

    int8_t currentMenuIndex = menu.get_current_component_num();
    int8_t totalMenuItems = menu.get_num_components();
    int8_t totalMinusCurrent = totalMenuItems - currentMenuIndex;
    int8_t menuStartFrom = totalMinusCurrent > 2
        ? max(0, currentMenuIndex - 2)
        : max(0, currentMenuIndex - 4 + totalMinusCurrent);
    int8_t menuEndAt = min(menuStartFrom + 4, totalMenuItems);

    for (int i = menuStartFrom; i < menuEndAt; i++) {
        MenuComponent const* cp_m_comp = menu.get_menu_component(i);
        lcd->setCursor(0, lcd->getCursorY() + 1);
        cp_m_comp->render(*this);
    }
}

void MenuRenderer::render_menu_item(MenuItem const& menu_item) const {
    render_menu_component(menu_item);
}

void MenuRenderer::render_back_menu_item(BackMenuItem const& menu_item) const {
    render_menu_component(menu_item);
}

void MenuRenderer::render_numeric_menu_item(NumericMenuItem const& menu_item) const {
    render_menu_component(menu_item);
}

void MenuRenderer::render_menu(Menu const& menu) const {
    render_menu_component(menu);
}

void MenuRenderer::render_menu_component(MenuComponent const& menu_item) const {
    menu_item.is_current()
        ? lcd->draw(LcdAssets::menuItemSelected, 4, true)
        : lcd->draw(LcdAssets::menuItem, 4, true)
    ;
    lcd->print(menu_item.get_name());
}

uint8_t MenuRenderer::getCursorXForCenteredText(const char* text) const
{
    return (LcdAssets::displayWidth - strlen(text) * LcdAssets::charWidth) / 2;
}