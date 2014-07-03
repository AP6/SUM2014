#version 420

uniform float Time;
uniform sampler2D Tex;
uniform float indI;
uniform float indJ;
uniform float PartNo;

out vec4 UseColor;

in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTex;

void main( void )
{
  UseColor = vec4(0.50, 0.50 + 0.1 * sin(Time * 5 + DrawPos.x * 10) + 0.1 * sin(Time * 5 + DrawPos.z * 10), 1.0, 0.30);
}
