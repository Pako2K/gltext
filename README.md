# gltext
Simple c++ library to render text using OpenGL and FreeType

Library dependencies (includes)
  * FreeType 2.9.1
  * glew 2.1.0
  * glo2
  * glm 0.9.8.5

Application dependencies (libraries to be linked with)
  * FreeType 2.9.1
  * glew 2.1.0
  * opengl32
  * glo2

The test application uses additionally GLFW in order to create a Window and the OpenGL context

Shaders files are manually converted into header files so they are delivered inside the static library
