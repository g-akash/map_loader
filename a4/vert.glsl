varying vec3 N;
varying vec3 v;
uniform vec3 pos1;
uniform vec3 pos2;
varying vec3 fpos1;
varying vec3 fpos2;


void main()
{
  v = vec3(gl_ModelViewMatrix * gl_Vertex);       
  N = normalize(gl_NormalMatrix * gl_Normal);
  gl_Position = ftransform();
  gl_FrontColor = gl_Color;
  gl_BackColor = gl_Color;
  fpos1 = (gl_ModelViewMatrix * vec4(pos1,1.0)).xyz;
  fpos2 = (gl_ModelViewMatrix * vec4(pos2,1.0)).xyz;


  gl_TexCoord[0] = gl_MultiTexCoord0;
}
