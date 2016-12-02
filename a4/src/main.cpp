#include "Common.hpp"
#include "Global.hpp"
#include <cstdlib>

// This function is called whenever the display needs to be updated
void
displayCallback()
{
  // Set the camera matrix (the projection matrix doesn't need to be touched unless the window is resized)
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrix(Global::camera.getWorldToCamera());

  // Clear the framebuffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // Issue OpenGL calls to draw the scene
  if (!Global::scene.draw())
    exit(-1);

  // This function swaps the draw buffer and the display buffer. The current frame, in the draw buffer, is transferred to the
  // display. The previous display buffer is now the new draw buffer. Using two buffers reduces flickering, since we don't
  // draw directly to the display. (We set up double-buffering with GLUT_DOUBLE in the main function.)
  glutSwapBuffers();
}

// This function is called whenever the window is resized
void
resizeCallback(int new_width, int new_height)
{
  // Prevent a divide by zero, when window is too short (you can't make a window of zero width)
  if (new_height == 0) new_height = 1;

  // Get the aspect ratio of the window
  float aspect_ratio = new_width / (float)new_height;

  // Set the viewport to be the entire window
  glViewport(0, 0, new_width, new_height);

  // Produce a new perspective matrix, to handle the aspect ratio
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(Global::camera.getFOV(),
                 aspect_ratio,
                 Global::camera.getNearDistance(),
                 Global::camera.getFarDistance());

  // Trigger a redraw
  glutPostRedisplay();
}

// This function is called whenever the user presses a normal key
void
keyboardCallback(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27 /* ESC */:
    case 'q':
      exit(0);  // no other way to get out of GLUT loop
      break;
    case 'w':
      if(Global::scene.drawwireframe)Global::scene.drawwireframe=false;
      else Global::scene.drawwireframe=true;
      Global::scene.draw();
      displayCallback();
    default:
      return;
  }

  glutPostRedisplay();
}

// This function is called whenever the user presses a "special" key, e.g. an arrow key
void
specialKeyCallback(int key, int x, int y)
{
  Mat4 mult;
  double angle = 0.09;
  for(int i=0;i<4;i++)
  {
    for(int j=0;j<4;j++)
    {
      mult[i][j]=0.0;
    }
  }
  
  switch (key)
  {
    case GLUT_KEY_LEFT:  /* TODO: Adjust Global::camera to turn left by a small angle */
      
      mult[0][0]=cos(angle);
      mult[0][2]=sin(angle);
      mult[1][1]=1;
      mult[2][0]=-sin(angle);
      mult[2][2]=cos(angle);
      mult[3][3]=1;
      Global::camera.setCameraToWorld(Global::camera.getCameraToWorld()*mult);
      break;
    case GLUT_KEY_RIGHT: /* TODO: Adjust Global::camera to turn right by a small angle */
      mult[0][0]=cos(-angle);
      mult[0][2]=sin(-angle);
      mult[1][1]=1;
      mult[2][0]=-sin(-angle);
      mult[2][2]=cos(-angle);
      mult[3][3]=1;
      Global::camera.setCameraToWorld(Global::camera.getCameraToWorld()*mult);
      break;
    case GLUT_KEY_UP:    /* TODO: Adjust Global::camera to move a step forward */
      
      mult[0][0]=1;
      mult[1][1]=1;
      mult[2][2]=1;
      mult[3][3]=1;
      mult[2][3]=-20;
      Global::camera.setCameraToWorld(Global::camera.getCameraToWorld()*mult);
      break;
    case GLUT_KEY_DOWN:  /* TODO: Adjust Global::camera to move a step backward */
      
      mult[0][0]=1;
      mult[1][1]=1;
      mult[2][2]=1;
      mult[3][3]=1;
      mult[2][3]=20;
      Global::camera.setCameraToWorld(Global::camera.getCameraToWorld()*mult);
      break;
    case GLUT_KEY_PAGE_UP:  /* TODO: Adjust Global::camera to look up by a small angle */
     
      mult[0][0]=1;
      mult[1][1]=cos(angle);
      mult[1][2]=-sin(angle);
      mult[2][1]=sin(angle);
      mult[2][2]=cos(angle);
      mult[3][3]=1;
      Global::camera.setCameraToWorld(Global::camera.getCameraToWorld()*mult);
      break;
    case GLUT_KEY_PAGE_DOWN:  /* TODO: Adjust Global::camera to look down by a small angle */
     
      mult[0][0]=1;
      mult[1][1]=cos(-angle);
      mult[1][2]=-sin(-angle);
      mult[2][1]=sin(-angle);
      mult[2][2]=cos(-angle);
      mult[3][3]=1;
      Global::camera.setCameraToWorld(Global::camera.getCameraToWorld()*mult);
      break;

    default:
      return;
  }

  glutPostRedisplay();
}

void
mousePressCallback(int button, int state, int x, int y)
{
  // glutPostRedisplay();
}

void
mouseMoveCallback(int x, int y)
{
  // glutPostRedisplay();
}

int
main(int argc, char * argv[])
{
  if (argc != 2 && argc != 7)
  {
    cerr << "Usage: " << argv[0] << " <scene-filename> [<ma> <ml> <ms> <msp> <msm>]" << endl;
    return -1;
  }

  // Setup for GLUT, which produces a window onto which you can draw using OpenGL
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  // glutInitWindowPosition(50, 50);
  glutInitWindowSize(800, 600);
  glutCreateWindow("CS475 OpenGL Viewer");

  // Initialize GL extensions via GLEW
  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    // Problem: glewInit failed, something is seriously wrong
    cerr << "Error: " << glewGetErrorString(err) << endl;
    return -1;
  }

  // Check if shaders are supported
  if (!GLEW_ARB_shader_objects)
  {
    cerr << "Your GL driver does not support shaders! Please update your driver.\n";
    cerr << "If that doesn't work, please contact course staff immediately." << endl;
    return -1;
  }

  // Set up callbacks
  glutDisplayFunc(displayCallback);
  glutReshapeFunc(resizeCallback);
  glutKeyboardFunc(keyboardCallback);
  glutSpecialFunc(specialKeyCallback);
  glutMouseFunc(mousePressCallback);
  glutMotionFunc(mouseMoveCallback);

  // Read command line
  string mesh_path = argv[1];
  float ma = 0.8f;
  float ml = 0.7f;
  float ms = 0.8f;
  float msp = 20.0f;
  float msm = 0.0f;
  if (argc == 7)
  {
    ma  = atof(argv[2]);
    ml  = atof(argv[3]);
    ms  = atof(argv[4]);
    msp = atof(argv[5]);
    msm = atof(argv[6]);
  }

  Material m(ma, ml, ms, msp, msm, 0.0f, 0.0f, 0.0f);

  // Load the scene
  cout << "Loading scene '" << mesh_path << '\'' << endl;
  if (!Global::scene.load(mesh_path, m))
  {
    cerr << "Error loading scene!" << endl;
    return -1;
  }

  // Activate depth test and 2D texturing
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_TEXTURE_2D);



  Mat4 mult;
  for(int i=0;i<4;i++)
  {
    for(int j=0;j<4;j++)
    {
      mult[i][j]=0.0;
    }
  }
  mult[0][0]=1.0;
  mult[1][1]=1.0;
  mult[2][2]=1.0;
  mult[3][3]=1.0;
  Vec3 mymax = Global::scene.max;
  Vec3 mymin = Global::scene.min;
  cout<<mymax<<" "<<mymin<<endl;
  mult[0][3]=(mymax[0]+mymin[0])/2.0;
  mult[1][3]=1.5*mymax[1];
  mult[2][3]=(mymax[2]+mymin[2])/2.0;
  Global::camera.setCameraToWorld(Global::camera.getCameraToWorld()*mult);

  for(int i=0;i<4;i++)
  {
    for(int j=0;j<4;j++)
    {
      mult[i][j]=0.0;
    }
  }

  mult[0][0]=1;
  mult[1][1]=cos(-90);
  mult[1][2]=-sin(-90);
  mult[2][1]=sin(-90);
  mult[2][2]=cos(-90);
  mult[3][3]=1;
  Global::camera.setCameraToWorld(Global::camera.getCameraToWorld()*mult);

  cout<<Global::camera.getCameraToWorld()<<endl;
  // Enter the GLUT input processing loop (resizeCallback will be called once just before the window is displayed -- initial
  // camera setup happens here)
  glutMainLoop();

  // This is never reached
  return 0;
}
