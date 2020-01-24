#version 430 core

in vec2 texCoord0;
in vec3 normal0;
in vec4 colour0;
in vec4 FragPos;
in float vHeight;

uniform vec3 cameraPos;

out vec4 colourExport;

uniform sampler2D mainTexture;
uniform sampler2D layeredTexture[3];

vec4 makeColour()
{
	float diffDir;
	float spec;
	float specularStrength = 4;

	vec3 norm = normalize(normal0.xyz);
	vec3 lightDir = normalize((vec3(0.0, 700.0, -1000.0) - FragPos.xyz).xyz);
	diffDir = max(dot(norm, lightDir), 0.4);

	vec3 lightDiffuse = diffDir * vec3(1.0, 1.0, 1.0); // Diffuse direction and the light colour

	vec3 viewDir = normalize(cameraPos - FragPos.xyz);
	vec3 reflectDir = reflect(-lightDir, norm);

	spec = pow(max(dot(viewDir, reflectDir), 0.0), 16) * specularStrength;

	return vec4((lightDiffuse + spec) * colour0, 0.0);
}

void main()
{
	vec4 expColour = makeColour();

	vec4 terrain_colours[4];
	terrain_colours[0] = texture(mainTexture, texCoord0);
	terrain_colours[1] = texture(layeredTexture[0], texCoord0);
	terrain_colours[2] = texture(layeredTexture[1], texCoord0);
	terrain_colours[3] = texture(layeredTexture[2], texCoord0);

	vec4 textureSet = vec4(0.0, 0.0, 0.0,1);
	float levels[7] = float[7]( -10.0, 0.0, 10.0, 20.0, 30.0, 40.0, 50.0 );

	bool isTextureColorSet = false;
	for(int i = 0; i < levels.length() && !isTextureColorSet; i++)
	{
		if(vHeight > levels[i]) {
			continue;
		}

		int currentSamplerIndex = i / 2;

		if(i % 2 == 0) {
			textureSet = terrain_colours[currentSamplerIndex];
		}
		else
		{
			int nextSamplerIndex = currentSamplerIndex+1;
			vec4 textureColorCurrent = terrain_colours[currentSamplerIndex];
			vec4 textureColorNext = terrain_colours[nextSamplerIndex];

			float levelDiff = levels[i] - levels[i-1];
			float factorNext = (vHeight - levels[i-1]) / levelDiff;
			float factorCurrent = 1.0f - factorNext;
         
			textureSet = textureColorCurrent*factorCurrent + textureColorNext*factorNext;
		}

		isTextureColorSet = true;
	}

	if(!isTextureColorSet)
	{
		int lastSamplerIndex = levels.length() / 2;
		textureSet = terrain_colours[lastSamplerIndex];
	}

	colourExport = colour0 * textureSet * clamp(dot(vec3(0,1,0), normalize(normal0)), 0.4, 1.0);
}