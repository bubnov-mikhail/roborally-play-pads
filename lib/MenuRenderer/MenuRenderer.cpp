#include <MenuRenderer.h>

MenuRenderer::MenuRenderer(Nokia_LCD* _lcd)
{
    lcd = _lcd;
}

void MenuRenderer::render(Menu const& menu) const {
    lcd->clear();
    menu.render(*this);

    uint8_t currentMenuIndex = menu.get_current_component_num();
    uint8_t totalMenuItems = menu.get_num_components();
    uint8_t menuStartFrom = currentMenuIndex > 2
        ? currentMenuIndex - 2
        : 0
    ;
    uint8_t menuEndAt = (totalMenuItems - currentMenuIndex) > 2
        ? totalMenuItems - currentMenuIndex
        : totalMenuItems
    ;
    lcd->setCursor(0, 1);
    for (int i = menuStartFrom; i < menuEndAt; i++) {
        MenuComponent const* cp_m_comp = menu.get_menu_component(i);
        cp_m_comp->render(*this);
        lcd->setCursor(0, lcd->getCursorY() + 1);
    }
}

void MenuRenderer::render_menu_item(MenuItem const& menu_item) const {
    if (menu_item.is_current()) {
        lcd->setInverted(true);
    }
    lcd->setCursor((displayWidth - sizeof(menu_item.get_name()) / sizeof(char)) / 2, 0);
    lcd->print(menu_item.get_name());
    lcd->setInverted(false);
}

void MenuRenderer::render_back_menu_item(BackMenuItem const& menu_item) const {
    render_menu_item(menu_item);
}

void MenuRenderer::render_numeric_menu_item(NumericMenuItem const& menu_item) const {
    render_menu_item(menu_item);
}

void MenuRenderer::render_menu(Menu const& menu) const {
    lcd->setCursor(0, 0);
    for (int i = 0; i < 14; ++i) {
        lcd->draw(LcdAssets::menuHeader, 6, true);
    }
    lcd->setInverted(true);
    lcd->setCursor((displayWidth - sizeof(menu.get_name()) / sizeof(char)) / 2, 0);
    lcd->print(menu.get_name());
    lcd->setInverted(false);
}