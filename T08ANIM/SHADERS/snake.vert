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
out vec4 DrawColor;

void main( void )
{
  DrawColor = InColor;
  DrawTex = InTexCoord;

  vec3 p = InPosition;
//  p.xz = vec2(p.x + 0.08 * sin(Time * 5 + p.z * 8 + indI + 20 * indJ), p.z + mod(Time * 0.701314, 5 * 0.701314));
    p.xz = vec2(p.x + 0.08 * sin(Time * 5 + p.z * 8 + indI + 20 * indJ), p.z + Time * 0.701314);
p.z -= 50;

  p.y += 0.13 * p.z;
  DrawPos = vec3(MatrWorld * vec4(p, 1));
  DrawNormal = normalize(mat3(MatrWorldInverseTranspose) * InNormal);
  gl_Position = MatrWVP * vec4(p + InNormal * 0.0000000001 * abs(sin(Time * 2 + Time * 2 + indI + 20 * indJ)) + 0*vec3(0, abs(sin(Time * 5 + indI + 20 * indJ)), 0), 1);
  //gl_Position = MatrWVP * vec4(InPosition, 1); // + InNormal * 0.0001 * sin(Time * 8), 1);
  //gl_Position = vec4(InPosition, 1);
}
