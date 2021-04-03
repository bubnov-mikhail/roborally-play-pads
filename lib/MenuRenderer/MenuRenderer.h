#pragma once

#include <MenuSystem.h>
#include <Nokia_LCD.h>
#include <LcdAssets.h>

class MenuRenderer : public MenuComponentRenderer
{
public:
    MenuRenderer(Nokia_LCD *_lcd);

    void render(Menu const &menu) const;

    void render_menu_item(MenuItem const &menu_item) const;

    void render_back_menu_item(BackMenuItem const &menu_item) const;

    void render_numeric_menu_item(NumericMenuItem const &menu_item) const;

    void render_menu(Menu const &menu) const;

    void render_header(const char *header) const;

    uint8_t getCursorXForCenteredText(const char *text) const;

private:
    Nokia_LCD *lcd;
    void render_menu_component(MenuComponent const &menu_item) const;
};