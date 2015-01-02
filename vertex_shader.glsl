#version 130 // OpenGL Version 3.0

in vec4 Position;	// local postion of vertex
uniform mat4 iMVP;	// ModelViewMatrix

void main()
{
	gl_Position = iMVP * Position;
}