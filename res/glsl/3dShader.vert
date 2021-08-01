#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoords;
layout (location = 2) in vec3 normal;

out vec2 pass_textureCoords;
out vec3 fragPos;
out vec3 pass_normal;


uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main ()
{
	pass_textureCoords = textureCoords;
	fragPos = vec3(ModelMatrix * vec4(position, 1.0));
	pass_normal = mat3(transpose(inverse(ModelMatrix))) * normal;

	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(position, 1);
}