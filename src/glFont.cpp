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

#include "gltext/glFont.h"

#include <limits>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>


#define MIN_CHAR_CODE 32
#define MAX_CHAR_CODE ( std::numeric_limits<GT_CharCode>::max() )

#if defined(_WIN32) || defined(_WIN64)
  #define FONTS_DIR "C:/Windows/Fonts/"
#else
  #define FONTS_DIR "/usr/share/fonts/truetype/"
#endif

#define FONT_EXT    ".ttf"

#define MAX_CACHE_SIZE 10


using namespace gltext;

std::vector<std::string>        glFont::_font_dirs {FONTS_DIR};
std::string                     glFont::_font_file_ext {FONT_EXT};
std::map<std::string, glFont*>  glFont::_fonts_cache;
std::map<std::string, uint8_t>  glFont::_fonts_clients;

void glFont::addFontsDir(std::string fonts_dir) {
  _font_dirs.push_back(fonts_dir);
}

void glFont::fontsFileExt(std::string ext) {
  _font_file_ext = ext;
}

glFont& glFont::getFont(std::string font_name, GT_FontSize font_size) {
  glFont *font;
  std::string font_uid { uid(font_name, font_size) };

  //Check whether the font is already loaded in the cache

  std::map<std::string, glFont*>::const_iterator iter;

  std::map<std::string, glFont*>::const_iterator m_it { _fonts_cache.find(font_uid) };
  if ( m_it != _fonts_cache.end() ) {
    font = m_it->second;
    _fonts_clients[font_uid]++;
  }
  // If not, create the new font
  else if ( _fonts_cache.size() < MAX_CACHE_SIZE ) {
    font = new glFont(font_name, font_size);
    _fonts_cache.insert(std::pair<std::string, glFont*>(font_uid, font));
    _fonts_clients.insert(std::pair<std::string, uint8_t>(font_uid, 1));
  }
  else {
    // Check which font in the cache has less clients
    std::map<std::string, uint8_t>::const_iterator c_it { _fonts_clients.begin()};
    uint8_t min_clients {255};
    std::string min_clients_uid {""};
    while ( c_it != _fonts_clients.end() ) {
        if ( c_it->second < min_clients ) {
          min_clients = c_it->second;
          min_clients_uid = c_it->first;
        }
        c_it++;
    }
    delete _fonts_cache[min_clients_uid];
    _fonts_cache.erase(min_clients_uid);
    _fonts_clients.erase(min_clients_uid);
    font = new glFont(font_name, font_size);
    _fonts_cache.insert(std::pair<std::string, glFont*>(font_uid, font));
    _fonts_clients.insert(std::pair<std::string, uint8_t>(font_uid, 1));
  }

  return *font;
}


void glFont::release() {
  //std::cout << "glFont RELEASED " << _name << " - " << _size << std::endl;
  _fonts_clients[uid()]--;
  if ( _fonts_clients[uid()] == 0 ) {
    if ( _fonts_cache.size() == MAX_CACHE_SIZE ) {
      _fonts_cache.erase(uid());
      _fonts_clients.erase(uid());
      delete this;
    }
  }
}


glFont::glFont(std::string font_name, GT_FontSize font_size) throw(std::string) : _name{font_name}, _size{font_size} {
  std::cout << "glFont CONSTRUCTOR CALLED " << font_name << " - " << font_size << std::endl;

  // Initialize Freetype
  FT_Library ft_lib;
  if (FT_Init_FreeType(&ft_lib))
    throw std::string("ERROR::FREETYPE: Could not init FreeType Library");


  FT_Face face;
  FT_Error error;
  uint8_t indx {0};
  do{
    error = FT_New_Face(ft_lib, std::string(_font_dirs[indx] + font_name + _font_file_ext).data(), 0, &face);
    indx++;
  } while ( error == FT_Err_Cannot_Open_Resource && indx < _font_dirs.size() );

  if ( error ) {
    if ( error == FT_Err_Cannot_Open_Resource)
      throw std::string("Font file cannot be opened: " + _font_dirs[--indx] + font_name + _font_file_ext);
    else if ( error == FT_Err_Unknown_File_Format )
      throw std::string("Unknown font file format");
    else
      throw std::string("Unknown error in Font file");
  }

  if ( FT_Set_Pixel_Sizes(face, 0, font_size) )
    throw std::string("Font Size not allowed");


  // Create ONE texture for all the glyphs. The height of the texture is the maximum height of the font.
  // Each glyph bitmap is aligned at the top of the texture, and follows the previous glyph bitmap.
  // For each glyph it is necessary to store additionally the relative coordinates of its bitmap within the texture
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction --> maybe this should be in the read method?????

  //    1 - Reserve memory for the font bitmap: use the max glyph height as vertical size and calculate the total width
  //        by adding the width of each glyph
  uint16_t texture_height { uint16_t(face->size->metrics.height >> 6) };
  size_t   texture_width  { 0 };
  GT_PositionPxl  descender { 0 };
  for (FT_ULong chr = MIN_CHAR_CODE; chr <= MAX_CHAR_CODE; chr++) {
    // Load character glyph
    if ( !FT_Load_Char(face, chr, FT_LOAD_RENDER) )
      texture_width += face->glyph->bitmap.width;
    // Store Descender
    descender = std::max(descender, GT_PositionPxl(face->glyph->bitmap.rows - face->glyph->bitmap_top));
  }

  // Create 2D texture allocating only the memory
  glGenTextures(1, &_texture_id);
  glBindTexture(GL_TEXTURE_2D, _texture_id);
  glTexImage2D( GL_TEXTURE_2D,
                0, // level
                GL_RED,
                texture_width,
                texture_height,
                0, // border width (This value must be 0)
                GL_RED,
                GL_UNSIGNED_BYTE,
                nullptr );
  // Set texture options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Horizontal position of each glyph within the texture, in pixels
  size_t text_pos_X { 0 };
  // Store the glyphs information
  for (FT_ULong chr = MIN_CHAR_CODE; chr <= MAX_CHAR_CODE; chr++) {
    // Load character glyph
    if (FT_Load_Char(face, chr, FT_LOAD_RENDER)) {
      std::cout << "ERROR: Failed to load Glyph " << chr << " (" << font_name << ")" << std::endl;
      _glyphs.insert(std::pair<GT_CharCode, glFont::Glyph>(chr, glFont::Glyph { { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
                                                                                { { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
                                                                                { 0, 0 } } ));
      continue;
    }

    // Store the glyph in the texture
    glTexSubImage2D(GL_TEXTURE_2D,
                    0, // level
                    text_pos_X, //xoffset​
                    0, // yoffset​
                    face->glyph->bitmap.width, // width​
                    face->glyph->bitmap.rows, // height​
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer );

    // Texture coord.
    GLfloat text_X_left   = GLfloat(text_pos_X) / texture_width;
    GLfloat text_X_right  = text_X_left + GLfloat(face->glyph->bitmap.width) / texture_width;
    GLfloat text_Y_top    = GLfloat(face->glyph->bitmap.rows) / texture_height;
    // Quad coord.
    GT_PositionPxl quad_X_left    = GT_PositionPxl(face->glyph->bitmap_left);
    GT_PositionPxl quad_X_right   = quad_X_left + GT_PositionPxl(face->glyph->bitmap.width);
    GT_PositionPxl quad_Y_top     = descender + GT_PositionPxl(face->glyph->bitmap_top);
    GT_PositionPxl quad_Y_bottom  = quad_Y_top - GT_PositionPxl(face->glyph->bitmap.rows);

    glFont::Glyph character = { { { quad_X_left, quad_Y_bottom },
                                  { quad_X_left, quad_Y_top },
                                  { quad_X_right, quad_Y_top },
                                  { quad_X_right, quad_Y_bottom }
                                },
                                { { text_X_left, text_Y_top },
                                  { text_X_left, 0 },
                                  { text_X_right, 0 },
                                  { text_X_right, text_Y_top } },
                                { GT_PositionPxl(face->glyph->advance.x >> 6), GT_PositionPxl(face->glyph->advance.y >> 6) } };

    _glyphs.insert(std::pair<GT_CharCode, glFont::Glyph>(chr, character));

    // Set new position in the buffer for next glyph
    text_pos_X += face->glyph->bitmap.width;
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  FT_Done_Face(face);
  FT_Done_FreeType(ft_lib);
}


glFont::~glFont() {
  std::cout << "glFont DESTROYED " << _name << " - " << _size << std::endl;

  //Delete font texture
  glDeleteTextures(1, &_texture_id);
}


