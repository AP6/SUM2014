#version 420

uniform float Time;
uniform sampler2D Tex;
uniform float indI;
uniform float indJ;
uniform float PartNo;

uniform float Trans;

out vec4 UseColor;

in vec3 DrawPos;
in vec3 DrawNormal;
in vec2 DrawTex;
in vec4 DrawColor;

void main( void )
{
  vec3 L = normalize(vec3(1 * cos(Time * 3.0), 5 * sin(Time * 5) * 0 + 0.2, 1 * sin(Time * 3.0)));

  UseColor = vec4(0, 0, 0, 1);

  float nl = dot(L, normalize(DrawNormal));
  if (nl > 0)
    UseColor.xyz += vec3(0.31, 0.51, 0.71) * nl;

  vec3 L1 = normalize(vec3(21, 5, 1));
  vec4 tc = texture2D(Tex, vec2(1, -1) + vec2(1, -1) * DrawTex.xy);

  nl = dot(L1, normalize(DrawNormal));
  if (nl > 0)
    UseColor.xyz += vec3(tc) * nl;
  UseColor.xyz += vec3(1, 1, 1) * int(1.01 * mod(DrawPos.z * 0.510 + Time * 1.1, 1));
  if (Trans < 1)
  {
    //if (abs(sin(DrawPos.z)) > 0.81)
    //  discard;
    float s = abs(pow(sin(Time + 10 * DrawPos.x), 2));
    UseColor.xyz = vec3(s / 3.5, s, 1);
    UseColor.a = 0.1;
  }
  else
    UseColor.a = 1;
  UseColor = vec4(DrawNormal, 1);
//  UseColor = vec4(DrawColor.rgb * DrawNormal.xyz, 1);
}
