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

#ifndef GLFONT_H
#define GLFONT_H

#include <string>
#include <map>
#include <vector>
#include <queue>
#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>

#include <gltext/typedefs.h>

namespace gltext
{
  class glFont
  {
    public:
      struct Glyph
      {
        // Coordinates of the quad, relative to the text baseline = (font baseline - font descender)
        GT_Position2D   quad_coord[4];
        // Coordinates in the font texture
        GLfloat         text_coord[4][2];
        // Offset to advance to next glyph
        GT_Position2D   advance;
      };

      static void     addFontsDir   (const std::string& fonts_dir);
      static void     fontsFileExt  (const std::string& ext);
      static glFont&  get           (const std::string& font_name, GT_FontSize font_size);

      void release();

      // Destructor (public so it can be called from the caching map)
       ~glFont();

      glFont(const glFont &other) = delete;
      glFont& operator=(const glFont &other) = delete;

      inline GLuint textureId() const { return _texture_id; }

      inline const glFont::Glyph& glyph(GT_CharCode chr_code) const {
        return _glyphs.at(chr_code);
      }


    protected:
      static std::map<std::string, std::unique_ptr<glFont>> _fonts_cache;
      static std::map<std::string, uint8_t> _fonts_clients;
      static std::queue<std::string> _deletion_candidates;

      std::string    _name;
      GT_FontSize    _size;
      GLuint         _texture_id;

      std::map<GT_CharCode, glFont::Glyph> _glyphs;

      // Constructor
      glFont(std::string font_name, GT_FontSize font_size);

      inline static std::string uid(std::string font_name, GT_FontSize font_size) {
        return font_name + std::to_string(font_size);
      }

      inline std::string uid() {
        return uid(_name, _size);
      }

    private:
      static std::vector<std::string> _font_dirs;

      static std::string              _font_file_ext;
  };

} // END namespace gltext

#endif // GLFONT_H
