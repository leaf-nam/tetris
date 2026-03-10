#include "render/window_console_renderer.hpp"

#include "render/theme.hpp"

#include <conio.h>
#include <iostream>
#include <windows.h>

using namespace std;

void ConsoleRenderer::clear()
{
    printf("\033[H");
    printf("%s", Theme::getInstance().color(Color::BACKGROUND, true).c_str());
    printf("\033[2J");
    fflush(stdout);
}

void ConsoleRenderer::show_cursor()
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(console_handle, &info);
}
void ConsoleRenderer::hide_cursor()
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(console_handle, &info);
}

void ConsoleRenderer::set_cursor(int x, int y) { printf("\033[%d;%dH", y + 1, x + 1); }

void ConsoleRenderer::set_cursor(Pos pos) { set_cursor(pos.x, pos.y); }

void ConsoleRenderer::set_color(Color color) { printf("%s", get_color(color).c_str()); };

void ConsoleRenderer::set_color(Color foreground, Color background)
{
    printf("%s%s", get_color(foreground).c_str(), get_color(background, true).c_str());
};

void ConsoleRenderer::print_s(const char* const s, Color key)
{
    printf("%s%s%s%s", get_color(key).c_str(), get_color(Color::BACKGROUND, true).c_str(), s,
           get_color(Color::BACKGROUND).c_str());
}

void ConsoleRenderer::print_s(const char* const s, Color foreground, Color background)
{
    printf("%s%s%s%s", get_color(foreground).c_str(), get_color(background, true).c_str(), s,
           get_color(background).c_str());
}

void ConsoleRenderer::print_s(string s, Color key) { print_s(s.c_str(), key); }

void ConsoleRenderer::print_s(string s, Color foreground, Color background)
{
    print_s(s.c_str(), foreground, background);
}

void ConsoleRenderer::print_s(string s) { printf("%s", s.c_str()); }
