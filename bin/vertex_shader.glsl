#version 130 // OpenGL Version 3.0

in vec4 Position;	// local postion of vertex
uniform mat4 iMVP;	// Model-View-Projection Matrix
uniform mat4 iModel; // Model Matrix

out Vertex
{
	vec3 position;
	vec3 normal;
} vertex;

//out Vertex vertex;

void main()
{
	gl_Position = iMVP * Position;
	vertex.position = (iModel * Position).xyz;
	vertex.normal =  (iModel * vec4(0,0,1,0)).xyz;
}