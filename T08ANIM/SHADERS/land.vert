#version 420

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

uniform vec4 UseColor;
uniform mat4 MatrWVP;
uniform mat4 MatrWorld;
uniform mat4 MatrWorldInverseTranspose;

uniform float indI;
uniform float indJ;
uniform float Time;


out vec3 DrawPos;
out vec3 DrawNormal;
out vec2 DrawTex;

void main( void )
{
  DrawTex = InTexCoord;
  DrawPos = vec3(MatrWorld * vec4(InPosition, 1));
  DrawNormal = normalize(mat3(MatrWorldInverseTranspose) * (InNormal * vec3(1, 1./2, 1)));
  gl_Position = MatrWVP * vec4(InPosition * vec3(1, 2, 1), 1);
  //gl_Position = InPosition;
}
