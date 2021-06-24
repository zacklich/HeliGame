
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "GenObject.h"
#ifndef M_PI
#define M_PI 3.14159265
#endif
#define TO_RADIANS (M_PI/180.0)


static void
drawRectangle(double x, double y, double z, double width, double height)
{
    
    glPushMatrix();
    
    glTranslatef(x,z,y);
    
    glShadeModel(GL_SMOOTH);
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    glBegin(GL_QUADS);
    glColor3f(0.5,0.5,0.5);
    glNormal3f(0,1,0);
    glVertex3f( -width/2.0, z, -height/2.0);
    glVertex3f( width/2.0, z, -height/2.0);
    glVertex3f( width/2.0, z, height/2.0);
    glVertex3f( -width/2.0, z, height/2.0);
    glEnd();
    
    glPopMatrix();

}


void Helipad :: draw(double time)
{
  double hthickness = diameter * 0.1;
  double resolution = 0.1;
  double f;
  double hwidth = diameter * 0.4;
  double z = 0.01;
  double radius = diameter / 2.0;
   
  setTransformation();

  glColor3f(0, 0, 0.2);
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,1,0);
  //  glTexCoord2f( 0.5, 0.5 );
  glVertex3f(0, z, 0);  /* center */

  for (f = 2 * M_PI; f >= 0; f -= resolution) {
    glNormal3f(0,1,0);
    //    glTexCoord2f( 0.5f * cos(f) + 0.5f, 0.5f * sin(f) + 0.5f );
    glVertex3f(radius *0.99 * cos(f), z, radius *0.99 * sin(f));
  }
  // finish off
  glNormal3f(0,1,0);
  //glTexCoord2f( 0.5, 0.5 );
  glVertex3f(radius*0.99, z, 0);
  glEnd();


    glBegin(GL_LINE_STRIP);
    glColor3f(1,1,1);
    glNormal3f(0,1,0);
    glLineWidth(6.0);
    for (f = 2 * M_PI; f >= 0; f -= resolution) {
        glVertex3f(radius * cos(f),  z, radius * sin(f));
    }
    glVertex3f(radius*0.99, z, 0);
    glEnd();

    
    // draw the 'H'
    //    glNormal3f(0,1,0);
    drawRectangle(0,0,z,hwidth,hthickness);
    drawRectangle(-hwidth/2,0,z,hthickness,hwidth);
    drawRectangle(hwidth/2,0,z,hthickness,hwidth);
    
}


Helipad::Helipad(double d)
{
  diameter = d;
  isHelipad = true;
}
