#version 130

uniform vec2 dim;

in vec2 pos;
in vec4 color;

out vec4 vertColor;

void main()
{
  vertColor = color;
  gl_Position = vec4(pos / dim, 0.0f, 1.0f);
}
