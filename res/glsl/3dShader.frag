#version 330

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 pass_textureCoords;
in vec3 fragPos;
in vec3 pass_normal;

out vec4 color;

uniform vec3 ViewPos;
uniform Material ObjMaterial;
uniform Light LightProp;

void main()
{
	
	vec3 ambient = texture(ObjMaterial.diffuse, pass_textureCoords).rgb * LightProp.ambient;

	vec3 norm = normalize(pass_normal);
	vec3 lightDir = normalize(-LightProp.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = texture(ObjMaterial.diffuse, pass_textureCoords).rgb * diff * LightProp.diffuse;

	vec3 viewDir = normalize(ViewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), ObjMaterial.shininess);
	vec3 specular = texture(ObjMaterial.specular, pass_textureCoords).rgb * LightProp.specular * spec;
	
	color = vec4(ambient + diffuse + specular, 1.0f);

}