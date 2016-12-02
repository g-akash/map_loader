#include "Scene.hpp"

bool
Scene::load(string const & filename, Material const & m)
{
  
  material = m;
  OBJParser parser;
  bool didParse = parser.parseOBJ(filename, &myMesh);
  if (!didParse)
  {
    cerr << "Since we couldnt parse the OBJ file, we will now quit" << endl;
    exit(1);
  }
  

  max=myMesh.max;
  min=myMesh.min;



  //set up the lights here.
  float a=myMesh.max[0],b=5*myMesh.max[1],c=(myMesh.max[2]+myMesh.min[2])/2.0,x=(myMesh.max[0]+myMesh.min[0])/2.0,y=(myMesh.max[1]+myMesh.min[1])/2.0,z=(myMesh.max[2]+myMesh.min[2])/2.0;
  

  Vec3 dir1(a-x,b-y,c-z);
  a=myMesh.min[0];
  Vec3 dir2(a-x,b-y,c-z);

  shader.setUniform("pos1",dir1);
  shader.setUniform("pos2",dir2);
  GLfloat light_position0[] = {dir1[0],dir1[1],dir1[2]};
  GLfloat light_ambient0[] = { 0.6, 0.6, 0.0, 0.0 };
  GLfloat light_diffuse0[] = { 0.2, 0.2, 0.5, 0.0 };

  GLfloat light_position1[] = {dir2[0],dir2[1],dir2[2]};
  GLfloat light_ambient1[] = { 0.6, 0.6, 0.0, 0.0 };
  GLfloat light_diffuse1[] =  { 0.2, 0.2, 0.5, 0.0 };

  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);

  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glShadeModel(GL_SMOOTH);

  for(size_t fid = 0; fid < myMesh.faces.size(); fid++)
  {
    GLuint tex = createGLTexture(myMesh.materials[myMesh.faces[fid]->materialId]->textureImage);
    //glGenTextures(1, &tex);
    //glBindTexture(GL_TEXTURE_2D, tex);

    myMesh.materials[myMesh.faces[fid]->materialId]->textureId=tex;

  }

  drawwireframe=false;
  // TODO: Replace these lines with more of your own code to process the loaded scene if necessary.
  // TODO: Hint - texture maps are only on the CPU at this point.

  return didParse;
}

bool
Scene::draw()
{
  
  SaveGLState save_gl_state;  // will restore state upon destruction

  // Load the shader if it hasn't been loaded yet
  if (!shader.isLoaded())
    if (!shader.load("vert.glsl", "frag.glsl"))
      return false;

  // Use the shader
  shader.enable();

  // TODO: Replace these lines with your own code to draw the loaded scene

  //SOME SAMPLE CODE TO GET YOU GOING:
  // for (size_t fid = 0; fid < myMesh.faces.size(); ++fid)
  // {
  //   OBJFace* f = myMesh.faces[fid];
  //   glBegin(GL_POLYGON);
  //   for (size_t vi = 0; vi < f->v.size(); ++vi)
  //   {
  //     OBJVertex const & vertex = myMesh.vertices[f->v[vi]];
  //   }
  //   glEnd();
  // }
  
  shader.setUniform("matMa",float(getMaterial().getMA()));
  shader.setUniform("matMl",float(getMaterial().getML()));
  shader.setUniform("matMs",float(getMaterial().getMS()));
  shader.setUniform("matMsp",float(getMaterial().getMSP()));
  shader.setUniform("matMsm",float(getMaterial().getMSM()));
  
  for(size_t fid = 0; fid < myMesh.faces.size(); fid++)
  {

    OBJFace * f = myMesh.faces[fid];
    shader.setUniform("tex",myMesh.materials[myMesh.faces[fid]->materialId]->textureId);    
    RGB color = myMesh.materials[myMesh.faces[fid]->materialId]->C;
    //cout<<myMesh.materials[myMesh.faces[fid]->materialId]->textureId<<endl;
    glBindTexture(GL_TEXTURE_2D, myMesh.materials[myMesh.faces[fid]->materialId]->textureId);
    if(drawwireframe)glBegin(GL_LINE_STRIP);
    else glBegin(GL_POLYGON);
    glColor3f(color[0],color[1],color[2]);

    Vec3 v0 = myMesh.vertices[f->v[0]].pos;
    Vec3 v1 = myMesh.vertices[f->v[1]].pos;
    Vec3 v2 = myMesh.vertices[f->v[f->v.size()-1]].pos;
    Vec3 edge01 = (v1 - v0);
    Vec3 edge02 = (v2 - v0);
    Vec3 face_normal = edge01 ^ edge02;
    face_normal.normalize();
    glNormal3f(face_normal.x(), face_normal.y(), face_normal.z());

    for(size_t vi=0; vi<f->v.size();vi++)
    {
      OBJVertex const & vertex = myMesh.vertices[f->v[vi]];
      glVertex3f(vertex.pos.x(),vertex.pos.y(),vertex.pos.z());
      Vec2 texcoord = myMesh.vertexTexCoords[f->v[vi]].coord;
      glTexCoord2f(texcoord.x(),texcoord.y());

    }
    glEnd();
  }

  // Don't replace this line!
  return true;
}
