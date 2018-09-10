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

#ifndef GTEXTRENDERER_H
#define GTEXTRENDERER_H

#include <string>
#include <vector>
#include <memory>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glo2/program.h>
#include <glo2/shader.h>

#include <gltext/typedefs.h>
#include <gltext/glFont.h>


namespace gltext
{
  class glTextRenderer
  {
    public:
      static glTextRenderer& getRenderer();
      void release();

      /**
        *  Draw a text
        */
      void draw(const std::string &text, const gltext::glFont &font, const gltext::GT_Position2D &position, const gltext::GT_Color4 &color);

      glTextRenderer(const glTextRenderer &other) = delete;
      glTextRenderer& operator=(const glTextRenderer &other) = delete;

    protected:
      static glTextRenderer *_text_renderer;

      GLuint _vao;
      GLuint _vbuffer;

      std::unique_ptr<glo2::Program> _program;

      GLint _uTextColor;

      glTextRenderer();

      ~glTextRenderer();

    private:
      static size_t _clients;
      struct vertex
      {
        GLshort pos[2]; // vertex position
        GLfloat text[2]; // texture coordinates
      };
      const size_t VERTEX_SIZE {sizeof(vertex::pos) + sizeof(vertex::text)};
  };
} // END namespace gltext

#endif // GTEXTRENDERER_H
