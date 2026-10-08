#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

/*
out vec3 crntPos;
out vec3 Normal;
out vec3 color;
out vec2 texCoord;
out vec4 fragPosLight;
*/

out DATA
{
    vec3 Normal;
	vec3 color;
	vec2 texCoord;
    mat4 projection;
	mat4 model;
	vec3 lightPos;
	vec3 camPos;
} data_out;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
/*
	crntPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
   //	gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
	Normal = aNormal;
   color = aColor;
   texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
   fragPosLight = lightProjection * vec4(crntPos, 1.0f);

   gl_Position = camMatrix * vec4(crntPos, 1.0);
*/
	gl_Position = model * translation * rotation * scale * vec4(aPos, 1.0f);
	data_out.Normal = aNormal;
	data_out.color = aColor;
	data_out.texCoord = aTex;
	data_out.projection = camMatrix;
	data_out.model = model * translation * rotation * scale;
	data_out.lightPos = lightPos;
	data_out.camPos = camPos;
}