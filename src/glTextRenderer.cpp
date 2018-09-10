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

#include "gltext/glTextRenderer.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


#ifdef DEBUG
  #include <iostream>
  #define TRACE(str) std::cout << "DEBUG: " << str << std::endl;

  const std::string __VERT_SHADER_SRC__ {"shaders/glyph_vs.glsl"};
  const std::string __FRAG_SHADER_SRC__ {"shaders/glyph_fs.glsl"};
#else
  #include "../shaders/glyph_vs.glsl.h"
  #include "../shaders/glyph_fs.glsl.h"

  #define TRACE(str)
#endif

using namespace gltext;

glTextRenderer *glTextRenderer::_text_renderer {nullptr};
size_t glTextRenderer::_clients {0};

glTextRenderer& glTextRenderer::getRenderer() {
  if (!_text_renderer)
    _text_renderer = new glTextRenderer();

  ++_clients;
  return *_text_renderer;
}

void glTextRenderer::release() {
    --_clients;
    if ( !_clients )
      delete _text_renderer;
};

void glTextRenderer::draw(const std::string &text, const gltext::glFont &font, const gltext::GT_Position2D &position, const gltext::GT_Color4 &color) {
  GLshort baseline_x { position.x() };
  size_t  counter {0};

  std::vector<vertex> vertices(6 * text.size());
  for ( auto &chr : text ) {
    const gltext::glFont::Glyph &gly = font.glyph(chr);

    // Only if glyph width is not 0
    if (gly.quad_coord[0].x() != gly.quad_coord[3].x() ) {
      vertices[counter++] = { GLshort(gly.quad_coord[0].x() + baseline_x), GLshort(gly.quad_coord[0].y() + position.y()), gly.text_coord[0][0], gly.text_coord[0][1] };
      vertices[counter++] = { GLshort(gly.quad_coord[1].x() + baseline_x), GLshort(gly.quad_coord[1].y() + position.y()), gly.text_coord[1][0], gly.text_coord[1][1] };
      vertices[counter++] = { GLshort(gly.quad_coord[2].x() + baseline_x), GLshort(gly.quad_coord[2].y() + position.y()), gly.text_coord[2][0], gly.text_coord[2][1] };
      vertices[counter++] = { GLshort(gly.quad_coord[0].x() + baseline_x), GLshort(gly.quad_coord[0].y() + position.y()), gly.text_coord[0][0], gly.text_coord[0][1] };
      vertices[counter++] = { GLshort(gly.quad_coord[2].x() + baseline_x), GLshort(gly.quad_coord[2].y() + position.y()), gly.text_coord[2][0], gly.text_coord[2][1] };
      vertices[counter++] = { GLshort(gly.quad_coord[3].x() + baseline_x), GLshort(gly.quad_coord[3].y() + position.y()), gly.text_coord[3][0], gly.text_coord[3][1] };
    }
		baseline_x += gly.advance.x();
	}

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindVertexArray(_vao);
  glBindBuffer(GL_ARRAY_BUFFER, _vbuffer);
  _program->useProgram();
  glUniform3f(_uTextColor, GLfloat(color.r()) / 255, GLfloat(color.g()) / 255, GLfloat(color.b()) / 255);

	glBindTexture(GL_TEXTURE_2D, font.textureId());
	glBufferData(GL_ARRAY_BUFFER, counter * VERTEX_SIZE , vertices.data(), GL_STREAM_DRAW);

  glDrawArrays(GL_TRIANGLES, 0, counter);

	_program->releaseProgram();
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glDisable(GL_BLEND);
}


glTextRenderer::glTextRenderer() : _program {new glo2::Program()} {
  TRACE("glTextRenderer CONSTRUCTOR CALLED")

  // Program & shaders
  glo2::Shader vertex_shader(GL_VERTEX_SHADER);
  glo2::Shader fragment_shader(GL_FRAGMENT_SHADER);
  try{
    vertex_shader.loadSource(__VERT_SHADER_SRC__);
    fragment_shader.loadSource(__FRAG_SHADER_SRC__);
    _program->attachShader(vertex_shader);
    _program->attachShader(fragment_shader);
    _program->linkProgram();
  }
  catch (std::string &exc){
    TRACE("ERROR: Cannot create GL program for the Text renderer")
    throw exc;
  }

 	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vbuffer);

	// Attribute pointer is enabled for this vao forever
	GLint vVertex = glGetAttribLocation(_program->id(), "inPosition");
	glEnableVertexAttribArray(vVertex); //vertex
	glVertexAttribIPointer( vVertex,  // location
                          2,        // size of the input parameter
                          GL_SHORT, // type
                          VERTEX_SIZE, // stride
                          (void*)0 ); // array buffer offset

	GLint vTextureCoord = glGetAttribLocation(_program->id(), "inTexCoord");
	glEnableVertexAttribArray(vTextureCoord); //texture coordinates
	glVertexAttribPointer(vTextureCoord,  // location
                        2,        // size of the input parameter
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        VERTEX_SIZE, // stride
                        (void*)(sizeof(vertex::pos)) ); // array buffer offset

	glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  // Uniforms
	_uTextColor = glGetUniformLocation(_program->id(), "uTextColor");

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLint uProjection = glGetUniformLocation(_program->id(), "uProjection");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(viewport[2]), 0.0f, static_cast<float>(viewport[3]));
	_program->useProgram();
  glUniformMatrix4fv(uProjection, 1, GL_FALSE, &projection[0][0]);
  _program->releaseProgram();
}


glTextRenderer::~glTextRenderer() {
  TRACE("glTextRenderer DESTROYED")

  glDeleteBuffers(1, &_vbuffer);
  glDeleteVertexArrays(1, &_vao);
  //delete _program;
}
