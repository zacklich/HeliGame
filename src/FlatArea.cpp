
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


void FlatArea :: draw(double time)
{
  double z = 0;
  double scale;

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
    glScalef(scale, scale, scale);
    glMatrixMode(GL_MODELVIEW);

    glBindTexture(GL_TEXTURE_2D,defaultTexture);
  }

  glBegin(GL_QUADS);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0);
  glVertex3f( -width/2.0, z, -height/2.0);
  glTexCoord2f(1,0);
  glVertex3f( width/2.0, z, -height/2.0);
  glTexCoord2f(1,1);
  glVertex3f( width/2.0, z, height/2.0);
  glTexCoord2f(0,1);
  glVertex3f( -width/2.0, z, height/2.0);
  glEnd();


  if (defaultTexture != 0) {
    glDisable(GL_TEXTURE_2D);
  }

}


FlatArea::FlatArea(double w, double h, int texture)
{
  width = w;
  height = h;

  setDefaultTexture(texture);
}
