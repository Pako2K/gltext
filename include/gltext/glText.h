//  MIT License
//
//  Copyright (c) 2018 Francisco de Lanuza
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.

#ifndef GLTEXT_H
#define GLTEXT_H

#include "typedefs.h"
#include "glFont.h"
#include "glTextRenderer.h"


namespace gltext
{
  class glText
  {
    public:
      /**
        * Constructor: creates a text in the specified position, using default font and color
        */
      glText(std::string text, GT_PositionPxl pos_X, GT_PositionPxl pos_Y)
            : glText(text, GT_Position2D{pos_X, pos_Y}) {}

      /**
        * Constructor: creates a text in the specified position, using default font and color
        */
      glText(std::string text, GT_Position2D position)
            : _text{text}, _position{position}, _font{&glFont::getFont(_s_def_font_name, _s_def_font_size)},
              _renderer{glTextRenderer::getRenderer()} {}

      ~glText() {
        _renderer.release();
        _font->release();
      }

      /**
        * Renders the text
        */
      void show() const { _renderer.draw(_text, *_font, _position, _color); }

      /**
        * GETTERS
        */
      std::string   text() const      { return _text; }
      GT_Position2D position() const  { return _position; }
      GT_Color4     color4() const    { return _color; }
      GT_Color      color() const     { return GT_Color{_color[0], _color[1], _color[2]}; }
      std::string   fontName() const  { return _font_name; }
      GT_FontSize   fontSize() const  { return _font_size; }

      /**
        * SETTERS
        */
      static void defColor    (GT_Color4 new_color)  { _s_def_color = new_color; }
      static void defFontName (std::string new_name) { _s_def_font_name = new_name; }
      static void defFontSize (GT_FontSize new_size) { _s_def_font_size = new_size; }
      void text     (std::string new_text)  { _text = new_text; }
      void position (GT_Position2D new_pos) { _position = new_pos; }
      void position (GT_PositionPxl pos_X, GT_PositionPxl pos_Y) { _position = GT_Position2D{pos_X, pos_Y}; }
      void font (std::string new_name, GT_FontSize new_size)  {
        if ( new_name != _font_name || new_size != _font_size ) {
          _font_name = new_name;
          _font_size = new_size;
          updateFont();
        }
      }
      void font (std::string new_name)  {
        if ( new_name != _font_name ) {
          _font_name = new_name;
          updateFont();
        }
      }
      void font (GT_FontSize new_size)  {
        if ( new_size != _font_size ) {
          _font_size = new_size;
          updateFont();
        }
      }
      void color    (GT_Color4 new_color)   { _color = new_color; }
      void color    (GT_Color new_color)    { for ( char i = 0; i < 3; i++ ) _color[i] = new_color[i]; }

    protected:
      static std::string  _s_def_font_name;
      static GT_FontSize  _s_def_font_size;
      static GT_Color4    _s_def_color;

      std::string       _text;
      GT_Position2D     _position;
      std::string       _font_name {_s_def_font_name};
      GT_FontSize       _font_size {_s_def_font_size};
      glFont            *_font;
      GT_Color4         _color {_s_def_color};

      glTextRenderer    &_renderer;

    private:
      void updateFont();
  };

} // END namespace gltext

#endif // GLTEXT_H
