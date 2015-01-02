bool INIT_FAILED = FALSE;

bool printGLError(const char *title, const bool nomsg, const bool noloop)
{
	bool result = true;

	GLenum err = glGetError();

	while (err != GL_NO_ERROR)
	{
		std::string error = "GL_UNKNOW_ERROR";

		switch (err)
		{
		case GL_INVALID_ENUM:					error = "GL_INVALID_ENUM";           break;
		case GL_INVALID_VALUE:					error = "GL_INVALID_VALUE";          break;
		case GL_INVALID_OPERATION:				error = "GL_INVALID_OPERATION";      break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:	error = "GL_INVALID_FRAMEBUFFER_OPERATION";  break;
		case GL_OUT_OF_MEMORY:					error = "GL_OUT_OF_MEMORY";          break;
		case GL_STACK_UNDERFLOW:				error = "GL_STACK_UNDERFLOW";     break;
		case GL_STACK_OVERFLOW:					error = "GL_STACK_OVERFLOW";     break;
		}

		if (!nomsg) fprintf(stderr, "OpenGL (%s): %s\n",title,error.c_str());

		err = glGetError();

		if (result) result = false;
		break;
	}

	return result;
}

void glfw_error_callback(int error, const char *description) { fprintf(stderr, "GLFW (%d): %s\n", error, description); }

void initGLFW()
{
	if (INIT_FAILED) return;

	glfwTerminate(); // Close all windows and terminate GLFW

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initalize GLFW!\n");
		INIT_FAILED = TRUE;
		return;
	}

	// callbacks
	glfwSetErrorCallback(glfw_error_callback);
}

void initWindow()
{
	if (INIT_FAILED) return;

	//int major = 3; int minor = 2;
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // GLFW_OPENGL_COMPAT_PROFILE,GLFW_OPENGL_CORE_PROFILE
	//glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, 0);

	// framebuffer hints
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	//glfwWindowHint(GLFW_AUX_BUFFERS, 32);
	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

	glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
	glfwWindowHint(GLFW_REFRESH_RATE, 0);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL); //glfwGetPrimaryMonitor()

	if (!window)
	{
		fprintf(stderr, "Failed to create window!\n");
		INIT_FAILED = TRUE;
		return;
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	window_focus = true;
}

void initGL()
{
	if (INIT_FAILED) return;

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW!\n");
		INIT_FAILED = TRUE;
		return;
	}

	printGLError("INIT GLEW");
}

void init()
{
	initGLFW();
	initWindow();
	initGL();
}