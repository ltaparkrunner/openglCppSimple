#version 430 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
uniform sampler2D tex0;

float near = 0.1f;
float far = 100.0f;

float linearizeDepth(float depth)
{
   return(2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

const float DEFAULT_STEEPNESS = 0.5; // Обратите внимание: в GLSL лучше писать 0.5 вместо 0.5f
const float DEFAULT_OFFSET = 5.0;

float logisticDepth(float depth)
{
   float zVal = linearizeDepth(depth);
   return (1/(1 + exp(-DEFAULT_STEEPNESS * (zVal- DEFAULT_OFFSET))));
}

void main()
{
   // FragColor = texture(tex0, texCoord);
   FragColor = vec4(vec3(linearizeDepth(gl_FragCoord.z) / far), 1.0f);
   // float depth = logisticDepth(gl_FragCoord.z);
   // FragColor = direcLight() * (1.0f - depth) + vec4(depth * vec3(0.85f, 0.85f, 0.90f), 1.0f);
}