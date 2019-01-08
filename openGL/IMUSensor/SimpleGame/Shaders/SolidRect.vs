#version 330

in vec3 a_Position;

uniform mat4 u_ProjView;
uniform mat4 u_Model;

void main()
{
	vec4 newPosition;
	newPosition = u_ProjView * u_Model * vec4(a_Position, 1.f);
	gl_Position = newPosition;
}
