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

#include "gltext/glText.h"


#define DEFAULT_COLOR      0, 0, 0, 255 // OPAQUE BLACK
#define DEFAULT_FONT_NAME  "arial"
#define DEFAULT_FONT_SIZE  12

using namespace gltext;

// Initialization of static members
GT_Color4     glText::_s_def_color      {DEFAULT_COLOR};
std::string   glText::_s_def_font_name  {DEFAULT_FONT_NAME};
GT_FontSize   glText::_s_def_font_size  {DEFAULT_FONT_SIZE};


#undef DEFAULT_COLOR
#undef DEFAULT_FONT_NAME
#undef DEFAULT_FONT_SIZE


void glText::updateFont() {
  _font->release();
  _font = &glFont::getFont(_font_name, _font_size);
}
