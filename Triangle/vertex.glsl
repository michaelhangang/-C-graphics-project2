#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; 
layout (location = 2) in vec4 vUVx2;

out vec3 FragPos; //object world location 
out vec3 Normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fUVx2;
void main()
{
    gl_Position =projection * view * model *vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal =mat3(transpose(inverse(model))) * aNormal;
	fUVx2 = vUVx2;
}