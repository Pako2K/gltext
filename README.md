# gltext
Simple c++ library to render text using OpenGL and FreeType

Library dependencies (includes)
  * FreeType 2.9.1
  * glew 2.1.0
  * glo2
  * glm 0.9.8.5

Libraries to be linked with in the using application
  * FreeType 2.9.1
  * glew 2.1.0
  * opengl32
  * glo2

The test application uses additionally GLFW in order to create a Window and the OpenGL context

Shaders files are automatically converted into header files so they are delivered inside the static library
