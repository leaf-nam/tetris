#ifndef __COLOR_PICKER_HPP__
#define __COLOR_PICKER_HPP__

#include "color.hpp"
#include "theme.hpp"

#include <tetromino/tetromino.hpp>

class ColorPicker
{
  public:
    ColorPicker() = default;
    Color get_random_color();
    std::string get_block_color(int type);
    std::string get_block_color(const Tetromino& tetromino);
    Color get_color_key(const Tetromino& tetromino);
    Color get_color_key(int type);
};

#endif //__COLOR_PICKER_HPP__
