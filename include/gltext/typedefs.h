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

#ifndef GLTEXT_TYPEDEFS_H
#define GLTEXT_TYPEDEFS_H

#include <array>

namespace gltext
{
  // Position and Size in Window Coordinates (Origin in UL corner, Unit = pixel)
  using GT_PositionPxl = int16_t;

  class GT_Position2D : public std::array<GT_PositionPxl, 2>
  {
    public:
      GT_PositionPxl& x() { return this->at(0); }
      const GT_PositionPxl& x() const { return this->at(0); }
      GT_PositionPxl& y() { return this->at(1); }
      const GT_PositionPxl& y() const { return this->at(1); }
      GT_Position2D() : GT_Position2D(0, 0) {};
      GT_Position2D(GT_PositionPxl pos_x, GT_PositionPxl pos_y)
                    : std::array<GT_PositionPxl, 2>{pos_x, pos_y} {}
  };

  using GT_SizePxl = uint16_t;

  class GT_Size2D : public std::array<GT_SizePxl, 2>
  {
    public:
      GT_SizePxl& w() { return this->at(0); }
      const GT_SizePxl& w() const { return this->at(0); }
      GT_SizePxl& h() { return this->at(1); }
      const GT_SizePxl& h() const{ return this->at(1); }
      GT_Size2D() : GT_Size2D(0, 0) {};
      GT_Size2D(GT_SizePxl width, GT_SizePxl height)
                : std::array<GT_SizePxl, 2>{width, height} {}
  };

  // Color attribute (in scale 0 - 255, with support to 10-bit color attribute)
  using GT_ColorAttr = uint16_t;

  class GT_Color4 : public std::array<GT_ColorAttr, 4>
  {
    public:
      GT_ColorAttr& r() { return this->at(0); }
      const GT_ColorAttr& r() const { return this->at(0); }
      GT_ColorAttr& g() { return this->at(1); }
      const GT_ColorAttr& g() const { return this->at(1); }
      GT_ColorAttr& b() { return this->at(2); }
      const GT_ColorAttr& b() const { return this->at(2); }
      GT_ColorAttr& a() { return this->at(3); }
      const GT_ColorAttr& a() const { return this->at(3); }
      GT_Color4(GT_ColorAttr red, GT_ColorAttr green, GT_ColorAttr blue, GT_ColorAttr opaque)
                : std::array<GT_ColorAttr, 4>{red, green, blue, opaque} {}
  };

  class GT_Color : public std::array<GT_ColorAttr, 3> {
    public:
      GT_ColorAttr& r() { return this->at(0); }
      const GT_ColorAttr& r() const { return this->at(0); }
      GT_ColorAttr& g() { return this->at(1); }
      const GT_ColorAttr& g() const { return this->at(1); }
      GT_ColorAttr& b() { return this->at(2); }
      const GT_ColorAttr& b() const { return this->at(2); }
      GT_Color(GT_ColorAttr red, GT_ColorAttr green, GT_ColorAttr blue)
              : std::array<GT_ColorAttr, 3>{red, green, blue} {}
  };

  // Font size in pixels
  using GT_FontSize = uint16_t;

  // Character code (limited to UTF-8)
  using GT_CharCode = uint8_t;
}


#endif // GLTEXT_TYPEDEFS_H
