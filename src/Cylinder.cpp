
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
#include "TextureMap.h"

extern TextureMap textureMap;


Cylinder :: Cylinder(double d, double h)
{
  diameter = d;
  height = h;
}



// Fragments of this found online; it was better than my quad strip version
void Cylinder::draw(double time)
{
  double radius = diameter / 2.0;
  double i;
  double resolution  = 0.1;
  double scale;

  // call parent class to set things up
  setTransformation();

  // Does this move into GenObject?
  if (defaultTexture != 0) {
    // enable texture mode, set color to white
    glEnable(GL_TEXTURE_2D);
    glColor3f(1,1,1);

    // Scale the texture
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    scale = textureMap.scaleForTexture(defaultTexture);
    glScalef(scale,scale,scale);
    glMatrixMode(GL_MODELVIEW);

    glBindTexture(GL_TEXTURE_2D,defaultTexture);
  }

  // We define our cylinders with the origin at the center.

  glTranslatef(0, -height/2.0, 0);

  // Drop of cylinder - fan of traingles.
  // Cylinder is flat, normal is straight up

  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,1,0);
  glTexCoord2f( 0.5, 0.5 );
  glVertex3f(0, height, 0);  /* center */

  for (i = 2 * M_PI; i >= 0; i -= resolution) {
    glNormal3f(0,1,0);
    glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
    glVertex3f(radius * cos(i), height, radius * sin(i));
  }

  // finish off
  glNormal3f(0,1,0);
  glTexCoord2f( 0.5, 0.5 );
  glVertex3f(radius, height, 0);
  glEnd();

  // bottom of cylinder, normal faces down
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,-1,0);
  glTexCoord2f( 0.5, 0.5 );
  glVertex3f(0, 0, 0);  /* center */

  for (i = 0; i <= 2 * M_PI; i += resolution) {
    glNormal3f(0,-1,0);
    glTexCoord2f( 0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f );
    glVertex3f(radius * cos(i), 0, radius * sin(i));
  }

  glEnd();

  // The tube itself, 

  glBegin(GL_QUAD_STRIP);
  for (i = 0; i <= 2 * M_PI; i += resolution) {
    const float tc = ( i / (float)( 2 * M_PI ) );
    glNormal3f(cos(i), 0, sin(i));
    glTexCoord2f( tc, 0.0 );
    glVertex3f(radius * cos(i), 0, radius * sin(i));
    glTexCoord2f( tc, 1.0 );
    glVertex3f(radius * cos(i), height, radius * sin(i));
  }

  // Finish the circle.
  glTexCoord2f( 0.0, 0.0 );
  glVertex3f(radius, 0, 0);
  glTexCoord2f( 0.0, 1.0 );
  glVertex3f(radius, height, 0);
  glEnd();

  if (defaultTexture != 0) {
    glDisable(GL_TEXTURE_2D);
  }

}

