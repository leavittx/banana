/* Lev Panov, 11-3, 2009 */

#include "tex.h"

int W = 800, H = 600;
double SyncTime;
double pi = 3.14159265358979;
double twopi = 2 * 3.14159265358979;

typedef struct 
{
  double r1;
  double r2;
  int N;
} WHEEL;

typedef struct 
{
  double Length;
  double Width;
  double Height;
  WHEEL FRONT_WHEEL, REAR_WHEEL;
  double REAR_FRAME_R, MID_FRAME_R, FRONT_FRAME_R, FACE_FRAME_R;
  double Seat_R, Seat_Height;
  double Helm_H, Helm_L, Helm_R;
  double RIM_R, RIM_W;
  double Space;
} BIKE;

BIKE Bike;

void DrawCoords( void )
{
  glDisable(GL_TEXTURE_2D);
  glLineWidth(3);
  glBegin(GL_LINES);

  glColor3d(1, 0, 0);
  glVertex3d(0, 0, 0);
  glVertex4d(10, 0, 0, 0);

  glColor3d(0, 1, 0);
  glVertex3d(0, 0, 0);
  glVertex4d(0, 10, 0, 0);

  glColor3d(0, 0, 1);
  glVertex3d(0, 0, 0);
  glVertex4d(0, 0, 10, 0);

  glEnd();
  glLineWidth(1);
  glEnable(GL_TEXTURE_2D);
}

void DrawCyl( double N, double s0, double s1, double h0, double h1, int tex, double scale )
{
  int i;

  glColor3d(1, 1, 1);
  glBindTexture(GL_TEXTURE_2D, tex);
  glBegin(GL_TRIANGLE_STRIP);
  for (i = 0; i <= N; i ++)
  {
    double a = i * twopi / N;
    glTexCoord2d(scale * i / N, 0);
    glVertex3d(s1 * sin(a), h1, s1 * cos(a));
    glTexCoord2d(scale * i / N, scale);
    glVertex3d(s0 * sin(a), h0, s0 * cos(a));
  }
  glEnd();
}

void DrawFence( double s0, double s1, double h0, double h1, int tex, double scale )
{
  glColor3d(1, 1, 1);
  glBindTexture(GL_TEXTURE_2D, tex);
  glBegin(GL_TRIANGLE_STRIP);

  glTexCoord2d(0, 0);
  glVertex3d(s1, h1, s1);
  glTexCoord2d(0, 1);
  glVertex3d(s0, h0, s0);

  glTexCoord2d(scale, 0);
  glVertex3d(s1, h1, -s1);
  glTexCoord2d(scale, 1);
  glVertex3d(s0, h0, -s0);

  glTexCoord2d(2 * scale, 0);
  glVertex3d(-s1, h1, -s1);
  glTexCoord2d(2 * scale, 1);
  glVertex3d(-s0, h0, -s0);

  glTexCoord2d(3 * scale, 0);
  glVertex3d(-s1, h1, s1);
  glTexCoord2d(3 * scale, 1);
  glVertex3d(-s0, h0, s0);

  glTexCoord2d(4 * scale, 0);
  glVertex3d(s1, h1, s1);
  glTexCoord2d(4 * scale, 1);
  glVertex3d(s0, h0, s0);

  glEnd();
}

void DrawSphere( double r, int tex, double scale )
{
  int i, j, N = 5 * 8, M = 5 * 15;
  double s = 1, s1 = 1, x, y, z, phy, theta, theta1;

  glBindTexture(GL_TEXTURE_2D, tex);

  for(i = 0; i < N; i ++)
  {
    glBegin(GL_QUAD_STRIP);
    theta = pi * i / N;
    theta1 = pi * (i + 1) / N;
    for (j = 0; j <= M; j ++)
    {
      phy = 2 * pi * j / M;
      x = sin(theta1) * cos(phy);
      y = cos(theta1);
      z = sin(theta1) * sin(phy);

      glTexCoord2d(s * (double)j / M, s1 * (double)(i + 1) / N);
      glVertex3d( r * x, r * y, r * z );

      x = sin(theta) * cos(phy);
      y = cos(theta);
      z = sin(theta) * sin(phy);

      glTexCoord2d(s * (double)j / M, s1 * (double)i / N);
      glVertex3d( r * x, r * y, r * z);
    }
    glEnd();
  }
}

void DrawPlane( double l, int tex, double scale )
{
  glColor3d(1, 1, 1);
  glBindTexture(GL_TEXTURE_2D, tex);
  glBegin(GL_QUADS);
  glTexCoord2d(0, 0);
  glVertex3d(0, 0, 0);

  glTexCoord2d(scale, 0);
  glVertex3d(l, 0, 0);

  glTexCoord2d(scale, - scale);
  glVertex3d(l, l, 0);

  glTexCoord2d(0, - scale);
  glVertex3d(0, l, 0);
  glEnd();
}

void DrawCircle( double r, int tex )
{
  int N = 36;
  int i;
  double angle = 2 * pi / N;

  glBindTexture(GL_TEXTURE_2D, tex);

  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2d(0.5, 0.5);
  glVertex3d(0, 0, 0);

  for (i = 0; i < N; i ++)
  {
    glTexCoord2d((cos(angle * i) + 1.0) * 0.5, (sin(angle * i) + 1.0) * 0.5);
    glVertex3d(cos(angle * i) * r, sin(angle * i) * r, 0);
  }

  glTexCoord2d((1.0 + 1.0) * 0.5, (0.0 + 1.0) * 0.5);
  glVertex3d(r, 0, 0);
  glEnd();
}

static void DrawTorus( int numc, int numt )
{
   int i, j, k;
   double s, t, x, y, z;

   glDisable(GL_TEXTURE_2D);
   glColor3d(1, 1, 0);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

   for (i = 0; i < numc; i ++)
   {
      glBegin(GL_QUAD_STRIP);
      for (j = 0; j <= numt; j ++)
      {
         for (k = 1; k >= 0; k--)
         {
            s = (i + k) % numc + 0.5;
            t = j % numt;

            x = (1 + .1 * cos(s * twopi / numc)) * cos(t * twopi / numt);
            y = (1 + .1 * cos(s * twopi / numc)) * sin(t * twopi / numt);
            z = .1 * sin(s * twopi / numc);
            glVertex3f(x, y, z);
         }
      }
      glEnd();
   }

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glColor3d(1, 1, 1);
   glEnable(GL_TEXTURE_2D);
}

void DrawWheel( double r1, double r2, int N, int tex )
{
  double angle = 360.0 / N, r = (r1 + r2) / (3 * N);
  int i;

  //  glBindTexture(GL_TEXTURE_2D, tex);
  glDisable(GL_TEXTURE_2D);
  glColor3d(1, 0, 0.5);
  glutSolidTorus(r1, r2, 20, 20);
  glColor3d(1, 1, 1);
  glEnable(GL_TEXTURE_2D);

  for (i = 0; i < N; i ++)
  {
    DrawCyl(10, r, r, 0, r2, tex + 3, 1);
    glRotated(angle, 0, 0, 1);
  }
  /*
  for (i = 0; i < N; i ++)
  {
  DrawCyl(10, (r1 + r2) / 3, (r1 + r2) / 3, 0, r1, tex, 1);
  glRotated(angle, 0, 0, 1);
  }
  */
}

void DrawBike( BIKE *Bike )
{
  /* Plane */
  glTranslated(-100, - Bike->REAR_WHEEL.r1 - Bike->REAR_WHEEL.r2, -100);
  glRotated(90, 1, 0, 0);
  DrawPlane(200, 0, 10);
  glRotated(-90, 1, 0, 0);
  glTranslated(100, Bike->REAR_WHEEL.r1 + Bike->REAR_WHEEL.r2, 100);
  /* End of Plane */

  /* Rear wheels */
  DrawWheel(Bike->REAR_WHEEL.r1, Bike->REAR_WHEEL.r2, Bike->REAR_WHEEL.N, 1);
  glTranslated(0, 0, Bike->Width);
  DrawWheel(Bike->REAR_WHEEL.r1, Bike->REAR_WHEEL.r2, Bike->REAR_WHEEL.N, 1);

  glRotated(- 90, 1, 0, 0);
  DrawCyl(15, Bike->REAR_FRAME_R, Bike->REAR_FRAME_R, 0, Bike->Width, 1, 1);
  glRotated(90, 1, 0, 0);
  /* End of Rear wheels */

  /* Middle frame and seat */
  glTranslated(0, 0, - Bike->Width / 2);
  glRotated(30, 0, 0, 1);
  DrawCyl(15, Bike->MID_FRAME_R, Bike->MID_FRAME_R, 0, Bike->Length / 2, 1, 1);

  glTranslated(0, Bike->Length / 2, 0);
  glRotated(-30, 0, 0, 1);
  DrawCyl(15, Bike->Seat_R, Bike->Seat_R, 0, Bike->Seat_Height, 4, 1);
  glRotated(90, 1, 0, 0);
  DrawCircle(Bike->Seat_R, 4);
  glRotated(-90, 1, 0, 0);
  glTranslated(0, Bike->Seat_Height, 0);
  glRotated(90, 1, 0, 0);
  DrawCircle(Bike->Seat_R, 4);
  glRotated(-90, 1, 0, 0);
  glTranslated(0, - Bike->Seat_Height, 0);
  /* End of Middle frame and seat */

  /* Front frame */
  glRotated(90, 0, 0, 1);
  DrawCyl(15, Bike->FRONT_FRAME_R, Bike->FRONT_FRAME_R, 0, Bike->Length / 2, 1, 1);
  glRotated(-90, 0, 0, 1);
  /* End of Front frame */

  /* Rim */
  glTranslated(- Bike->Length / 2, 0, 0);

  glTranslated(0, Bike->Helm_H + Bike->Helm_R + Bike->MID_FRAME_R, 0);
  glRotated(90, 1, 0, 0);
  DrawCyl(15, Bike->Helm_R, Bike->Helm_R, 0, Bike->Helm_L / 2, 1, 1);
  glTranslated(0, Bike->Helm_L / 2, 0);
  glRotated(90, 1, 0, 0);
  DrawCircle(Bike->Helm_R, 1);
  glRotated(-90, 1, 0, 0);
  glTranslated(0, - Bike->Helm_L, 0);
  glRotated(90, 1, 0, 0);
  DrawCircle(Bike->Helm_R, 1);
  glRotated(-90, 1, 0, 0);
  glTranslated(0, Bike->Helm_L / 2, 0);
  DrawCyl(15, Bike->Helm_R, Bike->Helm_R, 0, - Bike->Helm_L / 2, 1, 1);
  glRotated(-90, 1, 0, 0);
  /* End of Rim */
  //  glTranslated(0, - Bike->Helm_H - Bike->Helm_R - Bike->MID_FRAME_R, 0);

//  DrawCoords();

  /* Face */
  DrawCyl(15, Bike->FACE_FRAME_R, Bike->FACE_FRAME_R, 0, - (Bike->Helm_H + Bike->Height - 2 * (Bike->FRONT_WHEEL.r1 + Bike->FRONT_WHEEL.r2) - Bike->FACE_FRAME_R + Bike->Space/*2*/), 1, 1);
  glTranslated(0, - (Bike->Helm_H + Bike->Height - 2 * (Bike->FRONT_WHEEL.r1 + Bike->FRONT_WHEEL.r2) - Bike->FACE_FRAME_R + Bike->Space/*2*/), 0);

  glRotated(90, 1, 0, 0);
  DrawCyl(15, Bike->RIM_R, Bike->RIM_R, - Bike->RIM_W / 2, Bike->RIM_W / 2, 1, 1);
  glRotated(-90, 1, 0, 0);

  glTranslated(0, 0, Bike->RIM_W / 2);
  DrawCyl(15, Bike->RIM_R, Bike->RIM_R, 0, - (Bike->FRONT_WHEEL.r2 + Bike->RIM_R + Bike->Space), 1, 1);
  glTranslated(0, 0, - Bike->RIM_W);
  DrawCyl(15, Bike->RIM_R, Bike->RIM_R, 0, - (Bike->FRONT_WHEEL.r2 + Bike->RIM_R + Bike->Space), 1, 1);

  glTranslated(0, - (Bike->FRONT_WHEEL.r2 + Bike->RIM_R + 2), 0);
  glRotated(90, 1, 0, 0);
  DrawCyl(15, Bike->RIM_R / 2, Bike->RIM_R / 2, 0, Bike->RIM_W, 1, 1);
  glRotated(-90, 1, 0, 0);
  /* End of Face */

  /* Front Wheel */
  glTranslated(0, 0, Bike->RIM_W / 2);
  DrawWheel(Bike->FRONT_WHEEL.r1, Bike->FRONT_WHEEL.r2, Bike->FRONT_WHEEL.N, 1);
//  DrawCoords();
  /* End of Front wheel */
}

void Reshape( int Width, int Height )
{
  glViewport(1, 1, W = Width, H = Height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (double)W / H, 1, 500);
  gluLookAt(85, 27, 36, 0, 0, 0, 0, 1, 0);
  //gluLookAt(5, 13, 13, 0, 5, 0, 0, 1, 0);
  //gluLookAt(5, 5, 13, 0, 5, 0, 0, 1, 0);
  //gluLookAt(50, -20, -50, 0, 0, 0, 0, 1, 0);
  //gluLookAt(50, -5.6, -50, 0, 0, 0, 0, 1, 0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Keyboard( unsigned char key, int x, int y )
{
  if (key == 27)
    exit(EXIT_SUCCESS);
}

void SpecialFunc( int key, int x, int y )
{

}

void Display( void )
{
  double t = 50 * SyncTime;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glRotated(t, 0, 1, 0);

//  DrawCoords();

  glColor3d(1, 1, 1);
  glEnable(GL_TEXTURE_2D);
   
  //glutSolidTeapot(10);
 
  DrawBike(&Bike);

  glPushMatrix();
  glScaled(20, 20, 20);
  DrawTorus(3, 5);
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);

  glRotated(- t, 0, 1, 0);
  glPopMatrix();
  glFlush();
  glutSwapBuffers();
}

void Idle( void )
{
  static double StartTime = -1;

  if (StartTime == -1)
    StartTime = clock();
  else
    SyncTime = (double)(clock() - StartTime) / CLOCKS_PER_SEC;

  glutPostRedisplay();
}

void Init( void )
{
  CheckerTexSet(0);
/*
  G24_Tex_Load("E:\\CL11321\\tex\\m.g24", 1);
  G24_Tex_Load("E:\\CL11321\\tex\\brick.g24", 2);
  BMP_Tex_Load("E:\\CL11321\\tex\\m.bmp", 3);
  G24_Tex_Load("E:\\CL11321\\tex\\roof.g24", 4);
  G24_Tex_Load("E:\\CL11321\\tex\\gr.g24", 5);
  G24_Tex_Load("E:\\CL11321\\tex\\pyatack.g24", 6);
  G24_Tex_Load("E:\\CL11321\\tex\\woodshgl.g24", 7);
  G24_Tex_Load("E:\\CL11321\\tex\\grass.g24", 8);
*/
  G24_Tex_Load("/home/lev/lp3-get/X/PICS/M.G24", 1);
  G24_Tex_Load("/home/lev/lp3-get/X/PICS/BRICK.G24", 2);
//  BMP_Tex_Load("/home/lev/lp3-get/X/PICS/M.BMP", 3);
  G24_Tex_Load("/home/lev/lp3-get/X/PICS/ROOF.G24", 4);
  G24_Tex_Load("/home/lev/lp3-get/X/PICS/GR.G24", 5);
  G24_Tex_Load("/home/lev/lp3-get/X/PICS/PYATACK.G24", 6);
  G24_Tex_Load("/home/lev/lp3-get/X/PICS/WOODSHGL.G24", 7);
  G24_Tex_Load("/home/lev/lp3-get/X/PICS/GRASS.G24", 8);

  #if 0
  G24_Tex_Load("/home/np3/Bike/banana/bike/PICS/M.G24", 1);
  G24_Tex_Load("/home/np3/Bike/banana/bike/PICS/BRICK.G24", 2);
//  BMP_Tex_Load("/home/np3/Bike/banana/bike/PICS/M.BMP", 3);
  G24_Tex_Load("/home/np3/Bike/banana/bike/PICS/ROOF.G24", 4);
  G24_Tex_Load("/home/np3/Bike/banana/bike/PICS/GR.G24", 5);
  G24_Tex_Load("/home/np3/Bike/banana/bike/PICS/PYATACK.G24", 6);
  G24_Tex_Load("/home/np3/Bike/banana/bike/PICS/WOODSHGL.G24", 7);
  G24_Tex_Load("/home/np3/Bike/banana/bike/PICS/GRASS.G24", 8);
  #endif

  Bike.Width = 20;
  Bike.Length = 40;
  Bike.REAR_WHEEL.N = 10;
  Bike.REAR_WHEEL.r1 = 1.5;
  Bike.REAR_WHEEL.r2 = 8;
  Bike.FRONT_WHEEL.N = 10;
  Bike.FRONT_WHEEL.r1 = 1.5;
  Bike.FRONT_WHEEL.r2 = 9;
  Bike.REAR_FRAME_R = 1;
  Bike.MID_FRAME_R = 1;
  Bike.FRONT_FRAME_R = 1;
  Bike.FACE_FRAME_R = 1;
  Bike.Seat_Height = 2;
  Bike.Seat_R = 4;
  Bike.Helm_H = 0.3;
  Bike.Helm_L = 20;
  Bike.Helm_R = 1.2;
  Bike.RIM_R = 0.8;
  Bike.RIM_W = 8;
  Bike.Space = 2;

  Bike.Height = (Bike.REAR_WHEEL.r1 + Bike.REAR_WHEEL.r2) * 1 + Bike.Length / 2.0 * sqrt(3) / 2.0;
}

int main( int argc, char **argv )
{
/*
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(W, H);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("T45BIKE");
*/

  glutInit (&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH); //set the display to Double buffer, with depth
  glutGameModeString( "1680x1050:32@75" ); //the settings for fullscreen mode
  glutEnterGameMode(); //set glut to fullscreen using the settings in the line above

  glEnable(GL_DEPTH_TEST);
//  glEnable(GL_POLYGON_STIPPLE);

  Init();

//  glutFullScreen();

  glutKeyboardFunc(Keyboard);
  glutSpecialFunc(SpecialFunc);
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutIdleFunc(Idle);
  glutMainLoop();
  
  return 0;
}



#if 0
/* Face */
DrawCyl(15, Bike->FACE_FRAME_R, Bike->FACE_FRAME_R, 0, - Bike->Height - Bike->Helm_H, 2, 1);
glTranslated(0, - Bike->Height - Bike->Helm_H, 0);

glRotated(90, 1, 0, 0);
DrawCyl(15, Bike->RIM_R, Bike->RIM_R, - Bike->RIM_W / 2, Bike->RIM_W / 2, 2, 1);
glRotated(-90, 1, 0, 0);

glTranslated(0, 0, Bike->RIM_W / 2);
DrawCyl(15, Bike->RIM_R, Bike->RIM_R, 0, - Bike->FRONT_WHEEL.r2, 2, 1);
glTranslated(0, 0, - Bike->RIM_W);
DrawCyl(15, Bike->RIM_R, Bike->RIM_R, 0, - Bike->FRONT_WHEEL.r2, 2, 1);

glTranslated(0, - Bike->FRONT_WHEEL.r2, 0);
glRotated(90, 1, 0, 0);
DrawCyl(15, Bike->RIM_R / 2, Bike->RIM_R / 2, 0, Bike->RIM_W, 2, 1);
glRotated(-90, 1, 0, 0);
/* End of Face */

/* Front Wheel */
glTranslated(0, 0, Bike->RIM_W / 2);
DrawWheel(Bike->FRONT_WHEEL.r1, Bike->FRONT_WHEEL.r2, Bike->FRONT_WHEEL.N, 1);
//  DrawCoords();
/* End of Front wheel */
#endif
