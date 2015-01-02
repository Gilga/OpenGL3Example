#include <vector>

struct Shader
{
	GLuint id;
	GLenum type;
	GLint compiled;

	bool created;

	std::string url;
	std::string source;

	void create();
	void remove();

	void copy(const char* source);
	void read(const char* url);
	void read();

	bool compile();
	
	void attach(GLuint programID);
	void detach(GLuint programID);

	void print(const char* msg, bool error=false);
	std::string debugInfo();

	Shader(GLenum type) : type(type)
	{
		id=0;
		compiled = GL_FALSE;
		created = false;
	};
};

std::string Shader::debugInfo()
{
	int infoLogLength = 0;
	std::string pResult = "NOTHING";

	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{
		GLsizei charsWritten = 0;

		std::vector<char> strInfoLog;
		strInfoLog.resize(infoLogLength + 1);
		glGetShaderInfoLog(id, infoLogLength, &charsWritten, &strInfoLog[0]);

		if (charsWritten)
			pResult = std::string(strInfoLog.begin(), strInfoLog.end() - 1);
		else
			pResult = "FAILED";
	}

	return pResult;
}

void Shader::print(const char* msg, bool error)
{
	if(error) fprintf(stderr,"Shader (%s): %s\n", url.c_str() ,msg);
	else printf("Shader (%s): %s\n", url.c_str() , msg);
}

void Shader::create()
{
	if (created) return;
	print("create");
	id = glCreateShader(type); // create
	printGLError("glCreateShader");
	created = true;
}

void Shader::remove()
{
	if (!created) return;
	print("remove");
	glDeleteShader(id);
	printGLError("glDeleteShader");
	created = false;
}

void Shader::attach(GLuint programID)
{
	if (!created) return;
	print("attach");
	glAttachShader(programID, id);
	printGLError("glAttachShader");
}

void Shader::detach(GLuint programID)
{
	if (!created) return;
	print("detach");
	glDetachShader(programID, id);
	printGLError("glDetachShader");
}

bool Shader::compile()
{
	print("compile");

	remove(); // remove previous
	create();

	const char* source_chars = source.c_str();
	glShaderSource(id, 1, &source_chars, NULL);

	// compile
	glCompileShader(id);

	// check
	compiled = GL_FALSE;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);

	if (compiled == GL_FALSE)
	{
		print(debugInfo().c_str(),true);
		remove();
		return false;
	}

	return true;
}

void Shader::copy(const char* source)
{
	print("copy");
	this->source = source;
}

void Shader::read(const char* url)
{
	this->url = url;
	read();
}

void Shader::read()
{
	print("read");

	std::ifstream stream(url);

	if(stream.is_open())
	{
		std::stringstream buffer;
		buffer << stream.rdbuf();
		source = buffer.str();
		printf("file size %d.\n",source.size());
	}
	else
	{
		print("Cannot read file!",true);
	}
}

