#version 330 core
out vec4 color;
uniform vec4 ourColor;

void main()
{
	color = ourColor;
	//color = vec4((cur_position+1.0)/2, 1.0f);
}