// Include standard headers
#include <windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <complex>

// Include GLEW
//#define GLEW_STATIC
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE //vec3 v3 = v4.xyz();
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/rotate_vector.hpp>
//#include <glm/gtc/matrix_inverse.hpp>

// -------------------------------------------------------------------------------------------------

#define GL_REQUIRED_MAJOR 3
#define GL_REQUIRED_MINOR 2

#define WINDOW_NAME "OpenGL3 Example - Created by Mario Link"
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define WINDOW_ASPECT_RATIO (WINDOW_WIDTH * 1.f) / (WINDOW_HEIGHT * 1.f)
#define WINDOW_HALF_WIDTH WINDOW_WIDTH * 0.5f
#define WINDOW_HALF_HEIGHT WINDOW_HEIGHT * 0.5f

// -------------------------------------------------------------------------------------------------

GLFWwindow* window = NULL;
bool window_focus = false;
bool pause = false;
bool rightMouseHold = false;

glm::vec2 mouseDifPos;
glm::vec2 mousePos_old;
glm::vec2 mousePos;

bool FORWARD = false;
bool BACK = false;
bool RIGHT = false;
bool LEFT = false;
bool UP = false;
bool DOWN = false;

bool ROTATE_LEFT = false;
bool ROTATE_RIGHT = false;
bool ROTATE_UP = false;
bool ROTATE_DOWN = false;

// -------------------------------------------------------------------------------------------------

bool printGLError(const char *title, const bool nomsg = false, const bool noloop = false);

#include "init.hpp"
#include "plane.hpp"
#include "program.hpp"
#include "camera.hpp"
#include "matrix_transformations.hpp"

// -------------------------------------------------------------------------------------------------
void showFPS(bool reset=false);
void reload();
void showWireframe();

// -------------------------------------------------------------------------------------------------

void windowFocus_callback(GLFWwindow *window, int focused) { window_focus = (focused == GL_TRUE); }

void mousePos_callback(GLFWwindow *window, double xpos, double ypos)
{
	// new mouse pos
	mousePos = glm::vec2(-WINDOW_HALF_WIDTH + xpos, -WINDOW_HALF_HEIGHT + ypos);

	if (rightMouseHold)
	{
		// difference between old and new mouse pos
		mouseDifPos = mousePos - mousePos_old;
	}
}

void mouseWheel_callback(GLFWwindow * window, double xoffset, double yoffset)
{
}

void mouseButton_callback(GLFWwindow *window, int button, int action, int mods)
{
	bool press = (action == GLFW_PRESS);
	bool release = (action == GLFW_RELEASE);

	if (press || release)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			rightMouseHold = press ? true : false;
			if (release) camRotation = camCurrentRotation;
			mousePos_old = mousePos;
			mouseDifPos = glm::vec2(0,0);
		}
	}

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool press = (action == GLFW_PRESS);
	bool release = (action == GLFW_RELEASE);

	if (press || release)
	{
		if (press)
		{
			if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GL_TRUE);
			if (key == GLFW_KEY_PAUSE) { showFPS(true); pause = !pause; }
			if (key == GLFW_KEY_F5) { pause=true; showFPS(true); reload(); pause=false; }
			if (key == GLFW_KEY_F1) { showWireframe(); }
		}

		if (key == GLFW_KEY_W) FORWARD = press ? true : false;
		if (key == GLFW_KEY_A) LEFT = press ? true : false;
		if (key == GLFW_KEY_S) BACK = press ? true : false;
		if (key == GLFW_KEY_D) RIGHT = press ? true : false;
		if (key == GLFW_KEY_SPACE) UP = press ? true : false;
		if (key == GLFW_KEY_LEFT_CONTROL) DOWN = press ? true : false;

		if (key == GLFW_KEY_LEFT) ROTATE_LEFT = press ? true : false;
		if (key == GLFW_KEY_RIGHT) ROTATE_RIGHT = press ? true : false;
		if (key == GLFW_KEY_UP) ROTATE_UP = press ? true : false;
		if (key == GLFW_KEY_DOWN) ROTATE_DOWN = press ? true : false;
	}
}

bool run()
{
	glfwPollEvents(); // poll events
	return !glfwWindowShouldClose(window);
}

//Sleep(1); // 1ms
//Sleep(1000.0 / 60); // 60 FPS
void yield() { Sleep(1); }

void swap() { glfwSwapBuffers(window); }

void clear()
{
	//glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	printGLError("glClear");
}

std::string convertToStr(unsigned int value)
{
	std::ostringstream s;
	s << value;
	return s.str();
}

std::string convertToStr(double value)
{
	std::ostringstream s;
	s << value;
	return s.str();
}

double lastFrameTime = 0;
double frames = 0;
double max_frames[] = {0,0};

void showFPS(bool reset)
{
	double currentFrameTime = glfwGetTime();

	if (!pause) ++frames;

	if (reset || ((currentFrameTime - lastFrameTime) >= 1.0)) // If last measure was more than 1 sec ago
	{
		double fpms = frames > 0 ? (1000. / frames) : 0;

		// MAX
		if(max_frames[0] < frames)
		{
			max_frames[0] = frames;
			max_frames[1] = fpms;
		}

		if(reset)
		{
			 // reset frames
			lastFrameTime = 0;
			frames = 0;
			fpms = 0;
			max_frames[0] = 0;
			max_frames[1] = 0;
		}

		std::string title = std::string(WINDOW_NAME)
		+ " | FPS " + convertToStr(unsigned int(frames)) + " [" + convertToStr(unsigned int(max_frames[0])) + "]"
		+ " | FPMS " + convertToStr(fpms)+ " [" + convertToStr(max_frames[1]) + "]";

		glfwSetWindowTitle(window, title.c_str());

		// reset timer
		frames = 0;
		lastFrameTime = currentFrameTime;
	}
	
	//Sleep(1); // 1ms
	//Sleep(1000.0 / 60); // 60 FPS
}

void info()
{
	if (INIT_FAILED) return;

	// ------------------------------------------------------------
	// PRINT OPENGL INFO
	// ------------------------------------------------------------

	// GLFW Version
	int glfw_version[] = { 0, 0, 0 };
	glfwGetVersion(&glfw_version[0], &glfw_version[1], &glfw_version[2]);

	std::string version = (const char*)(glGetString(GL_VERSION));
	std::string glsl = (const char*)(glGetString(GL_SHADING_LANGUAGE_VERSION));
	std::string vendor = (const char*)(glGetString(GL_VENDOR));
	std::string renderer = (const char*)(glGetString(GL_RENDERER));

	// print OpenGl version
	printf("---------------------------------------------------------------------------\n");
	printf(" [ %s ]\n\n", WINDOW_NAME);
	printf(" OpenGL   | %s\n", version.c_str());
	printf(" GLSL     | %s\n", glsl.c_str());
	printf(" GLFW     | v.%d.%d.%d\n", glfw_version[0], glfw_version[1], glfw_version[2]);
	printf(" Vendor   | %s\n", vendor.c_str());
	printf(" Renderer | %s\n", renderer.c_str());
	printf("___________________________________________________________________________\n\n");

	int gl_version[] = { 0, 0 };
	glGetIntegerv(GL_MAJOR_VERSION, &gl_version[0]);
	glGetIntegerv(GL_MINOR_VERSION, &gl_version[1]);

	if (gl_version[0] < GL_REQUIRED_MAJOR || (gl_version[0] == GL_REQUIRED_MAJOR && gl_version[1] < GL_REQUIRED_MINOR))
	{
		INIT_FAILED = TRUE;
		glfwTerminate();
		fprintf(stderr, "OpenGL Version %d.%d is below required Version %d.%d!\n", gl_version[0], gl_version[1], GL_REQUIRED_MAJOR, GL_REQUIRED_MINOR);
		std::cin.ignore();
	}
}

void callbacks()
{
	// callbacks
	glfwSetWindowFocusCallback(window, windowFocus_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouseButton_callback);
	glfwSetScrollCallback(window, mouseWheel_callback);
	glfwSetCursorPosCallback(window, mousePos_callback);

	// Ensure we can capture the escape key being pressed below
	//glfwSetInputMode(window,GLFW_STICKY_KEYS,1);  //glfwEnable
	glfwSetCursorPos(window, WINDOW_WIDTH * .5, WINDOW_HEIGHT * .5);
}

void statics();
void static_uniforms();
void dynamic_uniforms();

void render()
{
	if (INIT_FAILED) return;

	callbacks();
	statics();
	plane(1);

	reload();

	while (run())
	{
		if (!window_focus || pause)
		{
			yield();
			continue;
		}

		dynamic_uniforms();

		plane(); // render plane

		swap();
		clear();
		showFPS();
	}

	plane(-1);
}

ShaderProgram program;

void reload()
{
	printf("\n----------\nReload\n----------\n\n");
	program.load();
	program.use();

	static_uniforms();
}

bool wireframe = false;

void showWireframe()
{
	wireframe = !wireframe;
	glPolygonMode(GL_FRONT, wireframe ? GL_LINE : GL_FILL);
	printGLError("glPolygonMode");
}

void statics()
{
	glEnable(GL_CULL_FACE);
	printGLError("glEnable");

	glFrontFace(GL_CCW);
	printGLError("glFrontFace");

	glClearColor(.75f, .75f, .75f, 1.f);
	printGLError("glClearColor");

	program.vertex_shader.url = "vertex_shader.glsl";
	program.fragment_shader.url = "fragment_shader.glsl";
}

void static_uniforms()
{
	float srceen_size[] = {WINDOW_WIDTH,WINDOW_HEIGHT};
	program.uniform("iResolution", USE_VECTOR, 2, srceen_size);

	Projection = projection_matrix();
	Model = model_matrix(plane_position,plane_rotation,plane_scaling);
}

double lastTime = 0;
void dynamic_uniforms()
{
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - lastTime;

	camera_movement(deltaTime, mouseDifPos.x, mouseDifPos.y);

	View = view_matrix(camPosition, camDirection, camUp); // update view matrix
	MVP = mvp_matrix(deltaTime); // update mvp matrix

	program.uniform("iGlobalTime", (float)currentTime);
	program.uniform("iMVP", USE_MATRIX4x4, 1, &MVP[0][0]);

	lastTime = currentTime;
}

int main(int argc, char** argv)
{
	init();
	info();
	render();
	return INIT_FAILED ? -1 : 0;
}