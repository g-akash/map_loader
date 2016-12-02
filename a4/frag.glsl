uniform float matMa;
uniform float matMl;
uniform float matMs;
uniform float matMsp;
uniform float matMsm;

uniform sampler2D tex;

varying vec3 N;
varying vec3 v;

varying vec3 fpos1;
varying vec3 fpos2;
   

void main()
{
  vec3 I0 = vec3(gl_FrontLightProduct[0].diffuse);
  vec3 I1 = vec3(gl_FrontLightProduct[1].diffuse);
  vec3 A0 = vec3(gl_FrontLightProduct[0].ambient);
  vec3 A1 = vec3(gl_FrontLightProduct[1].ambient);
  
  vec4 objcolor = texture2D(tex,gl_TexCoord[0].st);
  //vec3 C = vec3(gl_Color);
  vec3 C = vec3(objcolor);
  //vec3 Ih0 = normalize(-gl_LightSource[0].position.xyz);
  //vec3 Ih1 = normalize(-gl_LightSource[1].position.xyz);
  vec3 Ih0 = normalize(fpos1);
  vec3 Ih1 = normalize(fpos2);
  
  //vec3 Ih0;
  //vec3 Ih1;
  //Ih0[0]=0.0;Ih0[1]=1.0;Ih0[2]=0.0;
  //Ih1[0]=0.0;Ih1[1]=-1.0;Ih1[2]=0.0;

  vec3 nh = normalize(N);
  vec3 uh = normalize(-v);
  vec3 rh0 = normalize(-Ih0+2.0*dot(Ih0,nh)*nh);
  vec3 rh1 = normalize(-Ih1+2.0*dot(Ih1,nh)*nh);

  vec3 res01 = Ih0*nh;
  vec3 res02 = -rh0*uh;
  float sum01 = res01[0]+res01[1]+res01[2];
  float sum02 = res02[0]+res02[1]+res02[2];

  vec3 res11 = Ih1*nh;
  vec3 res12 = -rh1*uh;
  float sum11 = res11[0]+res11[1]+res11[2];
  float sum12 = res12[0]+res12[1]+res12[2];


  vec3 S = matMsm*C + (1.0-matMsm)*vec3(1.0,1.0,1.0);
  vec3 fin_color = matMa*C*A0+matMa*C*A1
  					+matMl*dot(C,I0)*max(sum01,0.0)+matMs*S*I0*pow(max(sum02,0.0),matMsp)
  					+matMl*dot(C,I1)*max(sum11,0.0)+matMs*S*I1*pow(max(sum12,0.0),matMsp);
  

  
  vec4 final_color = vec4(fin_color[0],fin_color[1],fin_color[2],1.0);
  gl_FragColor = final_color;
 
}
