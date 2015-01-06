#include "shader.hpp"

#define USE_FLOAT 0
#define USE_VECTOR 1
#define USE_MATRIX2x2 2
#define USE_MATRIX3x3 3
#define USE_MATRIX4x4 4

struct ShaderProgram
{
	GLuint id;
	GLint linked;

	bool created;
	bool used;

	std::string name;

	Shader vertex_shader;
	Shader fragment_shader;

	void create();
	void remove();
	void link();
	void use();

	void attach();
	void detach();

	void load();
	void read();
	void compile();

	bool isLinked();

	void print(const char* msg, bool error=false);
	std::string debugInfo();

	void uniform(const char *name, bool value);
	void uniform(const char *name, int value);
	void uniform(const char *name, float value);
	void uniform(const char *name, unsigned int type, unsigned int count, const float* value);

	ShaderProgram(const char* name) : name(name), vertex_shader(GL_VERTEX_SHADER), fragment_shader(GL_FRAGMENT_SHADER)
	{
		id = 0;
		linked = GL_FALSE;
		created = false;
		used = false;
	};
};

bool ShaderProgram::isLinked() { return (linked == GL_TRUE); }

std::string ShaderProgram::debugInfo()
{
	int infoLogLength = 0;
	std::string pResult = "NOTHING";

	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{
		GLsizei charsWritten = 0;

		std::vector<char> strInfoLog;
		strInfoLog.resize(infoLogLength + 1);
		glGetProgramInfoLog(id, infoLogLength, &charsWritten, &strInfoLog[0]);

		if (charsWritten)
			pResult = std::string(strInfoLog.begin(), strInfoLog.end() - 1);
		else
			pResult = "FAILED";
	}

	return pResult;
}

void ShaderProgram::print(const char* msg, bool error)
{
	if(error) fprintf(stderr,"Program (%s): %s\n", name.c_str() ,msg);
	else printf("Program (%s): %s\n", name.c_str() , msg);
}

void ShaderProgram::create()
{
	if (created) return;
	print("create");
	id = glCreateProgram();
	printGLError("glCreateProgram");
	created = true;
}

void ShaderProgram::remove()
{
	if (!created) return;
	print("remove");
	glDeleteProgram(id);
	printGLError("glDeleteProgram");
	created = false;
	used = false;
}


void ShaderProgram::use()
{
	if (!created || !isLinked() || used) return;
	print("use");
	glUseProgram(id);
	printGLError("glUseProgram");
	used = true;
}

void ShaderProgram::link()
{
	if (!created) return;

	print("link");

	glLinkProgram(id);
	//glValidateProgram

	// check
	linked = GL_FALSE;
	glGetProgramiv(id, GL_LINK_STATUS, &linked);

	if(linked == GL_FALSE)
	{
		print(debugInfo().c_str(),true);
		glLinkProgram(0);
	}
}

void ShaderProgram::attach()
{
	if (!created) return;
	print("attach");
	vertex_shader.attach(id);
	fragment_shader.attach(id);
}

void ShaderProgram::detach()
{
	if (!created) return;
	print("detach");
	vertex_shader.detach(id);
	fragment_shader.detach(id);
}

void ShaderProgram::read()
{
	print("read");
	vertex_shader.read();
	fragment_shader.read();
}

void ShaderProgram::compile()
{
	print("compile");
	vertex_shader.compile();
	fragment_shader.compile();
}

void ShaderProgram::load()
{
	print("load");

	read();
	compile();
	create();
	attach();
	link();

	if (linked != GL_TRUE) { remove(); return; }

	detach();
}

void ShaderProgram::uniform(const char *name, bool value)
{
	GLint location = glGetUniformLocation(id,name);
	if(location<0) return;
	glUniform1ui(location, value);
	printGLError("glUniform1ui");
}

void ShaderProgram::uniform(const char *name, int value)
{
	GLint location = glGetUniformLocation(id,name);
	if(location<0) return;
	glUniform1i(location, value);
	printGLError("glUniform1i");
}

void ShaderProgram::uniform(const char *name, float value)
{
	GLint location = glGetUniformLocation(id,name);
	if(location<0) return;
	glUniform1f(location, value);
	printGLError("glUniform1f");
}

void ShaderProgram::uniform(const char *name, unsigned int type, unsigned int count, const float* value)
{
	GLint location = glGetUniformLocation(id,name);
	if(location<0) return;

	switch (type)
	{
	case USE_VECTOR:
		switch (count)
		{
		case 2:
			glUniform2fv(location, 1, value);
			printGLError("glUniform1fv - vector2");
			break;
		case 3:
			glUniform3fv(location, 1, value);
			printGLError("glUniform1fv - vector3");
			break;
		case 4:
			glUniform4fv(location, 1, value);
			printGLError("glUniform1fv - vector4");
			break;
		}
		break;

	case USE_MATRIX2x2:
		glUniformMatrix2fv(location, count, GL_FALSE, value);
		printGLError("glUniformMatrix2fv");
		break;

	case USE_MATRIX3x3:
		glUniformMatrix3fv(location, count, GL_FALSE, value);
		printGLError("glUniformMatrix3fv");
		break;

	case USE_MATRIX4x4:
		glUniformMatrix4fv(location, count, GL_FALSE, value);
		printGLError("glUniformMatrix4fv");
		break;

	default:
		glUniform1fv(location, count, value);
		printGLError("glUniform1fv");
	}
}