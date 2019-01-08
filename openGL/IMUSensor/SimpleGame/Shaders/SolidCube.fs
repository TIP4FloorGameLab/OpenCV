#version 330

in vec3 v_Normal;
in vec4 v_Color;

out vec4 FragColor;

void main()
{
    float lighting = dot(v_Normal, vec3(0, 0, 1.0));
	FragColor.rgb = v_Color.rgb * abs(lighting);
    FragColor.a = v_Color.a;
}
