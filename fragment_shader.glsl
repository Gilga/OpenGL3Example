#version 130 // OpenGL Version 3.0

uniform vec2 iResolution;	// viewport resolution (in pixels)
uniform float iGlobalTime;	// shader playback time (in ms)

void main()
{
	vec2 UV = gl_FragCoord.xy / iResolution;
	gl_FragColor = vec4(UV,0.5 + 0.5 * sin(iGlobalTime),1);
}