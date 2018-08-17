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

#include <string>
#include <iostream>

// Using GLEW-2.1.0: requires glew.h and the static libraries glew32s.lib  (compiled as a dependency in
//        project glew) and libopengl32.a (BE SURE THIS THE ORDER!!!)
#define GLEW_STATIC
#include <GL/glew.h>
// Using GLFW-3.2.1: requires the header file glfw3.h and the static libraries libglfw3.a and libgdi32.a
#include <GLFW/glfw3.h>

#include <gltext/glText.h>


// Number of frame buffer samples for antialiasing
constexpr int FRAME_BUFFER_SAMPLES {1};

// Number of screen refreshes to complete before next framebuffer swap
constexpr int SWAP_INTERVAL {0};


void createWnd() throw(std::string);
void show();

GLFWwindow * _window;


int main(int argc, char** args) {

  try{
    createWnd();
  }
  catch(std::string &exc){
    std::cout << "Error creating window: " + exc << std::endl;
    return 1;
  }

  // Window Event Loop
  try{
    show();;
  }
  catch(std::string &exc){
    std::cout << "Error showing window: " + exc << std::endl;
    return 1;
  }

  return 0;
}


/// ************************************************************************************** ///


void createWnd() throw(std::string)
{
  //Start GLFW for the whole application (all windows reuse the same GLFW)
	if (!glfwInit()) {
		// Initialization failed
		throw std::string("GLFW CANNOT BE INITIALIZED.");
	}

  // Using OpenGL 4.0, core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_SAMPLES, FRAME_BUFFER_SAMPLES); // antialiasing

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  // Get primary monitor and its current resolution (video mode)
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	if (!monitor) {
    glfwTerminate();
		throw std::string("Could not get monitor.");
	}
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	if (!mode) {
    glfwTerminate();
		throw std::string("Could not get Video mode.");
	}

  // Resolution of the screen is in pixels
	//_window = glfwCreateWindow(mode->width, mode->height, "GL WINDOW",  nullptr, nullptr);
	_window = glfwCreateWindow(mode->width, mode->height, "GL WINDOW",  glfwGetPrimaryMonitor(), nullptr);
	if (!_window){
    glfwTerminate();
    throw std::string("Could not create window.");
	}

	// Make this window OpenGL context the current one
	glfwMakeContextCurrent(_window);
	if (!glfwGetCurrentContext()) {
		glfwDestroyWindow(_window);
		glfwTerminate();
		throw std::string("Couldn't make OpenGL context the current one.");
	}

  // Refresh for the current OpenGL context
	glfwSwapInterval(SWAP_INTERVAL);


	/// INITIALIZE OPEN GL
	glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    glfwDestroyWindow(_window);
    glfwTerminate();
    throw std::string("Not possible to initialize GL: " + std::string((char *)glewGetErrorString(err)));
  }
}


void show()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  gltext::glText txt_fps {"", 0, 0};
  txt_fps.font("stencil", 24);
  //txt_fps.font(24);
  txt_fps.color(gltext::GT_Color(205, 205, 0));

  gltext::glText txt {"aÄÄSSida apioas KJJSääaop sfpdfu asdu aisdua aius isu sui  uss su su. asddd sdd, isido pfa opsfp dfuisidopf aopsfpdfu isidopf", 50, 50};
  txt.color(gltext::GT_Color(100, 0, 0));
  gltext::glText txt1 {"1aÄÄSSida apioas KJJSääaop sfpdfuisidop faopsfp df uisi dopf aop sfpdfu isidopf", 50, 50};
  txt1.color(gltext::GT_Color(255, 0, 0));
  gltext::glText txt2 {"2aÄÄSSida apioas KJJSääa opsfpdfui sid opfaop sfpdfu isid opf aopsfp dfu isidopf", 50, 50};
  txt2.color(gltext::GT_Color(255, 0, 0));


  glfwShowWindow(_window);

  // Event loop
  GLint64 timer;
  GLint64 new_timer;
  GLint64 diff_timer;
  std::string elapsed_time {""}, fps{" fps"};
  glGetInteger64v(GL_TIMESTAMP, &timer);
  new_timer = timer;
  uint32_t num_frames {0};
	do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    txt_fps.text(fps);
    txt_fps.show();
    //myfont.draw(fps, glm::vec2(10,10), glm::uvec2(100,20), glm::vec3(100,0,0), 1.0f, wgl::alignment::LEFT);

    txt.font(10);
    txt.show();
    txt1.font(14);
    txt1.position(50,70);
    txt1.show();
    txt2.font(19);
    txt2.position(50,80);
    txt2.show();
    txt.font(24);
    txt.position(50,120);
    txt.show();
        txt.font(9);
    txt.position(50,160);
    txt.show();
    txt.position(50,180);
    txt.show();
    txt.position(50,200);
    txt.show();
    txt.position(50,220);
    txt.show();
    txt.position(50,240);
    txt.show();
    txt.position(50,260);
    txt.show();
    txt.position(50,280);
    txt.show();
    txt.position(50,300);
    txt.show();
    txt.position(50,320);
    txt.show();
    txt.position(50,340);
    txt.show();
    txt.position(450,360);
    txt.show();
    txt.position(450,380);
    txt.show();
    txt.position(450,400);
    txt.show();
    txt.position(450,450);
    txt.show();
    txt.position(450,550);
    txt.show();
    txt.position(450,650);
    txt.show();
    txt.position(450,750);
    txt.show();
    txt.position(450,850);
    txt.show();
    txt.position(450,950);
    txt.show();

//myfont.draw("aidaä-üö apioas UIASDZUj KJJSääaopsfpdfuisidopf aopsfpdfuisidopf", glm::vec2(10,50), glm::uvec2(100,20), glm::vec3(100,0,0), 1.0f, wgl::alignment::LEFT);
//myfont.draw("aida apioas KJJSääaopsfpdfuisidopfaopsfpdfuisidopf aopsfpdfuisidopf", glm::vec2(10,80), glm::uvec2(100,20), glm::vec3(100,0,0), 1.0f, wgl::alignment::LEFT);

    diff_timer = new_timer - timer;
    if (diff_timer > 500000000) {
      fps = std::to_string(int(num_frames * 1e9 / (diff_timer))) + " fps";
      std::cout << fps << std::endl;
      timer = new_timer;
      num_frames = 0;
    }


    //std::cout << fps << std::endl;

    glGetInteger64v(GL_TIMESTAMP, &new_timer);
    num_frames++;

//    elapsed_time = std::to_string(1e6 * new_timer - timer);




    glfwSwapBuffers(_window);

		glfwPollEvents();
	} while (!glfwWindowShouldClose(_window));

}
