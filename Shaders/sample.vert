#version 330 core

layout(location = 0) in vec3 aPos;

//uniform float x;

uniform mat4 transform // refers to final transformation matrix

void main() {
	//vec3 newPos = vec3(aPos.x + x, aPos.y, aPos.z);
	gl_Position = transform * vec4(aPos, 1.0);
}