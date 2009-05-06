#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <math.h>
#include <conio.h>
double  r1 = 1, r2 = 3, r3 = 1, r4 = 2.5, R1 = 20, L = 10, L1 = 3, L2 = 3, L3 = 3, V = 0.5, p = 0;      /*double  r1 = 1, r2 = 3, r3 = 1, r4 = 2.5, R1 = 20, L = 10, L1 = 3, L2 = 3, L3 = 3, V = 0.5;*/
void Keyboard(unsigned char key, int x, int y)
{
  if(key == 13) exit(0);
  if(key=='-')  V-=0.5;
  if(key=='+')  V+=0.1;
  if(key=='/')  R1+=0.5;
  if(key=='*')  R1-=0.5;
}
int TSK_Width, TSK_Height;
double PI = 3.14159265358979323846264338;
void Reshape(int Width, int Height)
{
  glViewport(0, 0, TSK_Width = Width, TSK_Height = Height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (double)Width / Height, 1, 5000);
  gluLookAt(50, 15, 15 , 0, 0, 0, 0, 1, 0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
void Fence(double s0, double s1, double h0, double h1, double R)

{
  glBegin(GL_TRIANGLE_STRIP);
  glColor3d(1 * cos(R), 0, 0);
  glVertex3d(s1, h1, s1);
  glVertex3d(s0, h0, s0);

  glColor3d(1 * sin(R), 1 * cos(R), 0);
  glVertex3d(-s1, h1, s1);
  glVertex3d(-s0, h0, s0);

  glColor3d(1 * cos(R) * sin(R), 0, 1 * sin(R));
  glVertex3d(-s1, h1, -s1);
  glVertex3d(-s0, h0, -s0);

  glColor3d(0, 1 * cos(R) * cos(R), 0);
  glVertex3d(s1, h1, -s1);
  glVertex3d(s0, h0, -s0);

  glColor3d(0, 0.8, 0.7);
  glVertex3d(s1, h1, s1);
  glVertex3d(s0, h0, s0);
  glEnd();
}

void Cylinder(double r1, double h0, double h1)
{
  double i, N = 1024, PI = 3.14159265358979323846264338, a, z, x;
  glBegin(GL_TRIANGLE_STRIP);
  for(i = 0; i <= N; i+= 1)
  {
   a = i * (2 * PI / N);
   z = r1 * cos(a);
   x = r1 * sin(a);
   glVertex3d(x , h0, z);
   glVertex3d(x , h1, z);
  }
  glEnd();
}
void Koleso(double r1,double r2)
{
  double alfa=0,N=15,i;
  glutWireTorus(r1,r2,30,30);
  for(i=1;i<=N;i++)
  {
         glBegin(GL_LINES);
         glVertex3d( 0, 0, 0);
         if(((double)(i/2)-(int)(i/2))==0)
           glVertex3d( r2*sin(alfa), r2*cos(alfa),0.2);
         else
               glVertex3d( r2*sin(alfa), r2*cos(alfa),-0.2);
         alfa=2*i*PI/N;
         glEnd();
  }
}
void Display(void)
{
  /*glPushMatrix();
  glPopMatrix();*/
  double c = L/(2*R1);
  double alfa= 90 - acos(c)*180/PI;
  double static Time=0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   Fence(500,0,-r2-r1,-r2-r1,2);                                              // пол
  glColor3d(1, 0.5, 0);                                               //
  glPushMatrix();
  glPushMatrix();
  glTranslated(-20,0,0);
  glRotated(Time+60,0,1,0);
    glTranslated(R1,0,0);
        //glTranslated(-20,0,0);
    glRotated(90,0,1,0);
    glPushMatrix();
      glRotated(-Time*(R1/r2),0,0,1);
           Koleso(r1,r2);                                 // переднее колесо
    glPopMatrix();
    glPushMatrix();
      glTranslated(0,0,1.2);
           glColor3d(1, 0.3, 0);                                  //
           Cylinder(0.3,0,r1+r2+0.5);                                 // планка руля 1
          glTranslated(0,0,-2.4);
           Cylinder(0.3,0,r1+r2+0.5);                                 // планка руля 2
    glPopMatrix();
     Cylinder(0.5,r1+r2+0.5,r1+r2+0.5+L2);                                // передняя вертикальная планка
    glPushMatrix();
      glRotated(-90,1,0,0);
           glColor3d(0, 1, 0.5);                                  //
           Cylinder(0.2,-2,2);                        // передняя ось
          glPushMatrix();
                glTranslated(0,2,0);
                glPushMatrix();
                  glRotated(180+Time*(R1/r2),0,1,0);                      // педаль 1
                  glTranslated(r2,0,0);
                       Cylinder(0.4,0,2);
        glPopMatrix();
                glRotated(90,0,0,1);
                glRotated(Time*(R1/r2),1,0,0);
                 Cylinder(0.2,0,r2);                                              // ось педали 1
          glPopMatrix();
          glPushMatrix();
                glTranslated(0,-4,0);
                glRotated(180+Time*(R1/r2),0,1,0);                        // педаль 2
                glTranslated(-r2,0,0);
             Cylinder(0.4,0,2);
      glPopMatrix();
          glPushMatrix();
                glTranslated(0,-2,0);
                glRotated(-90,0,0,1);
                glRotated(-Time*(R1/r2),1,0,0);
                 Cylinder(0.2,0,r2);                                              // ось педали 2
          glPopMatrix();
          glTranslated(0,0,r1+r2+0.5);
           glColor3d(0, 1, 0.5);                                  //
           Cylinder(0.3,-1.5,1.5);                            // передняя гооизонтальная планка
          glTranslated(0,0,L2);
       Cylinder(0.3,-L3,L3);                              // руль
    glPopMatrix();
    glRotated(90,0,0,1);
    glTranslated(r2+r1+0.5+L2/2,0,0);
    glRotated(-alfa,1,0,0);
     Cylinder(0.5,0,L);                                   // горизонтальная планка
    glRotated(90,1,0,0);
    glTranslated(-2*r2+r4-2*r1+r3-0.5-L2/2,0,-L);
     Cylinder(0.5,-L1+r3/2,L1-r3/2);                          // задняя ось
         Cylinder(0.2,-L1-r3/2,L1+r3/2);                              // задняя ось
    glRotated(90,1,0,0);
    glPushMatrix();
          glRotated(-90,0,0,1);
           glColor3d(1, 0.3, 0);                              //
       Cylinder(0.5,0,2*r1+2*r2-r3-r4+2+L2/2);                             // задняя вертикальная планка
           Fence(1,1,2*r1+2*r2-r3-r4+2+L2/2,2*r1+2*r2-r3-r4+2.5+L2/2,1);                            // седлушка
           Fence(1,0,2*r1+2*r2-r3-r4+2.5+L2/2,2*r1+2*r2-r3-r4+2.5+L2/2,1);
    glPopMatrix();
    glTranslated(0,0,L1);
    glPushMatrix();
      glRotated(Time*((R1-L1)/r4),0,0,1);
           glColor3d(1, 0.5, 0);                                      //
       Koleso(r3,r4);                                 // заднее колесо 1
    glPopMatrix();
    glTranslated(0,0,-(2*L1));
    glPushMatrix();
      glRotated(Time*((R1+L1)/r4),0,0,1);
       Koleso(r3,r4);                                 // заднее колесо 2
    glPopMatrix();
  glPopMatrix();
  glPopMatrix();
  glFinish();
  glutSwapBuffers();
  Time=Time+V;
}
void Idle (void)
{
  glutPostRedisplay();
}
void main (void)
{
  FreeConsole();
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1000,1000);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("YAHOO!!!");
  glEnable(GL_DEPTH_TEST);
  glClearColor(0, 0.5, 1, 1);
  glutKeyboardFunc(Keyboard);
  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutIdleFunc(Idle);
  glutFullScreen();
  glutMainLoop();
  getch();
}
