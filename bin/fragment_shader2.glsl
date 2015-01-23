#version 130 // OpenGL Version 3.0

uniform vec2 iResolution;	// viewport resolution (in pixels)
uniform float iGlobalTime;	// shader playback time (in ms)

in Vertex
{
	vec3 position;
	vec3 normal;
} vertex;

out vec4 OutColor;	// new color

const vec3 lightPosition = vec3(0.25f,0.0f,0.75f);
const vec3 cameraPosition = vec3(-0.5f,0.0f,0.75f);

const vec3 materialAmbient = vec3(0.2f,0.2f,0.4f);
const vec3 materialDiffuse = vec3(0.5f,1.0f,0.5f);
const vec3 materialSpecular = vec3(1.0f,0.5f,0.5f);
const vec3 lightColor = vec3(1.0f,1.0f,1.0f);

const float lightIntense = 1.0f;
const float diffuseIntense = 0.75f;
const float ambientIntense = 1.0f;
const float specularIntense = 2.0f;
const float gammaAmount = 2.2f;

const float lightAttenuation = 10.0f;
const float shininessCoefficient = 0.75f;

void main()
{
   vec3 D = (lightPosition - vertex.position); 
   vec3 L = normalize(D);
   vec3 N = normalize(vertex.normal);
   vec3 E = normalize(cameraPosition - vertex.position); 
   vec3 R = reflect(-L, N); // reflection

   float cosTheta = max(0.0f, dot(E, R)); // get theta
   float specularCoefficient = pow( cosTheta, shininessCoefficient);

   float diffuseCoefficient = max(0.0f, dot(N, L)+0.0f); // phong_weight

   vec3 ambient = materialAmbient * ambientIntense;
   vec3 diffuse = materialDiffuse * lightColor * diffuseIntense;
   vec3 specular = materialSpecular * lightColor * specularCoefficient * specularIntense;

   float attenuation = 1.0 / (1.0 + lightAttenuation * pow(length(D), 2));

   vec3 color = ambient;

   if ( diffuseCoefficient > 0.0 ) {
	   color += (diffuse + specular) * attenuation * lightIntense;
   }

   OutColor = vec4(pow(color, vec3(gammaAmount)),1.0f);

}