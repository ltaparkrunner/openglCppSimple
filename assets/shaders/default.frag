#version 430 core
out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;

in vec3 color;
in vec2 texCoord;
in vec4 fragPosLight;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D shadowMap;
uniform samplerCube shadowCubeMap;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;
uniform float farPlane;

vec4 spotLight() {
   float outerCone = 0.90f;
   float innerCone = 0.95f;
   float ambient = 0.20f;

   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(lightPos - crntPos);

   float diffuse = max(dot(normal, lightDirection), 0.0f);

   // specular lighting
	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
      float specularLight = 0.50f;
      vec3 viewDirection = normalize(camPos -crntPos);
//      vec3 reflectionDirection = reflect(-lightDirection, normal);
      vec3 halfwayVec = normalize(viewDirection + lightDirection);
      float specAmount = pow(max(dot(viewDirection, halfwayVec), 0.0f), 16);
      float specular = specAmount * specularLight;
   };

   float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
   float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	// Shadow value
	float shadow = 0.0f;
	// Sets lightCoords to cull space
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
		float bias = max(0.00025f * (1.0f - dot(normal, lightDirection)), 0.000005f);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);

	}

	return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) * inten + ambient) + texture(specular0, texCoord).r * specular * (1.0f - shadow) * inten) * lightColor;

//   return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor ;
}

vec4 pointLight() {
   vec3 lightVec = lightPos - crntPos;
   float dist = length(lightVec);
   float a = 0.0003;
   float b = 0.00002;
   float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);
   float ambient = 0.20f;

   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(lightPos - crntPos);

   float diffuse = max(dot(normal, lightDirection), 0.0f);

   // specular lighting
	float specular = 0.0f;
   if (diffuse != 0.0f)
   {
      float specularLight = 0.50f;
      vec3 viewDirection = normalize(camPos -crntPos);
      vec3 halfwayVec = normalize(viewDirection + lightDirection);
      // vec3 reflectionDirection = reflect(-lightDirection, normal);
      float specAmount = pow(max(dot(viewDirection, halfwayVec), 0.0f), 16);
      float specular = specAmount * specularLight;
   };

	// Shadow value
	float shadow = 0.0f;
	vec3 fragToLight = crntPos - lightPos;
	float currentDepth = length(fragToLight);
	float bias = max(0.5f * (1.0f - dot(normal, lightDirection)), 0.0005f); 

	// Not really a radius, more like half the width of a square
	int sampleRadius = 2;
	float offset = 0.02f;
	for(int z = -sampleRadius; z <= sampleRadius; z++)
	{
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowCubeMap, fragToLight + vec3(x, y, z) * offset).r;
				// Remember that we divided by the farPlane?
				// Also notice how the currentDepth is not in the range [0, 1]
				closestDepth *= farPlane;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
	}
	// Average shadow
	shadow /= pow((sampleRadius * 2 + 1), 3);

	return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) * inten + ambient) + texture(specular0, texCoord).r * specular * (1.0f - shadow) * inten) * lightColor;
//   return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor ;
}

vec4 directLight() {
   float ambient = 0.20f;

   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));

   float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specular = 0.0f;
   if (diffuse != 0.0f)
   {
      float specularLight = 0.50f;
      vec3 viewDirection = normalize(camPos -crntPos);
//      vec3 reflectionDirection = reflect(-lightDirection, normal);
      vec3 halfwayVec = normalize(viewDirection + lightDirection);
      float specAmount = pow(max(dot(viewDirection, halfwayVec), 0.0f), 16);
      float specular = specAmount * specularLight;
   };

	// Shadow value
	float shadow = 0.0f;
	// Sets lightCoords to cull space
	vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
	if(lightCoords.z <= 1.0f)
	{
		// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0f) / 2.0f;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
		float bias = max(0.025f * (1.0f - dot(normal, lightDirection)), 0.0005f);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);

	}

   // return (texture(diffuse0, texCoord) * (diffuse /* inten*/ + ambient) + texture(specular0, texCoord).r * specular /* inten*/) * lightColor ;
   return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) + ambient) + texture(specular0, texCoord).r * specular  * (1.0f - shadow)) * lightColor;

}


float near = 0.1f;
float far = 100.0f;
const float steepness = 0.5f;
const float offset = 5.0f;

float linearizeDepth(float depth)
{
	return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));
}

float logisticDepth(float depth)
{
   float zVal = linearizeDepth(depth);
	return (1 / (1 + exp(-steepness * (zVal - offset))));
}

void main()
{
//	outputs final color
//   FragColor = directLight();
   FragColor = pointLight();
}
