#version 330

in vec2 pass_textureCoords;
in vec3 fragPos;
in vec3 pass_normal;

out vec4 color;

uniform sampler2D TextureSampler;
uniform vec3 LightPos;
uniform vec3 LightColor;

void main()
{
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * LightColor;

	vec3 norm = normalize(pass_normal);
	vec3 lightDir = normalize(LightPos - fragPos); 
	float diff = 1.0;
	vec3 diffuse = diff * LightColor;

	color = texture(TextureSampler, pass_textureCoords) * vec4(ambient + diffuse, 1.0f);

}