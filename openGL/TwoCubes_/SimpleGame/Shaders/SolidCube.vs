#version 330

in vec3 a_Position;
in vec3 a_Normal;
in vec4 a_Color;

uniform mat4 u_ProjView;
uniform mat4 u_Model;
uniform mat4 u_Rot;

out vec3 v_Normal;
out vec4 v_Color;

void main()
{
	gl_Position = u_ProjView * u_Model * vec4(a_Position, 1.f);
	v_Normal = (u_Rot * vec4(a_Normal, 1.0)).rgb;
	v_Color = a_Color;
}
