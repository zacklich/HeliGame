
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


TexturedBox :: TexturedBox(double w, double l, double h)
{
  width = w;
  length = l;
  height = h;
}

static void texturesOn(int texture)
{
  double scale;

  glBindTexture(GL_TEXTURE_2D,texture);

  // enable texture mode, set color to white
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


  glColor3f(1,1,1);

  // Scale the texture
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();

  scale = textureMap.scaleForTexture(texture);
  glScalef(scale,scale,scale);

  glMatrixMode(GL_MODELVIEW);

  //  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  //  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  //  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

  //glEnable(GL_TEXTURE_GEN_S);
  //glEnable(GL_TEXTURE_GEN_T);
  //glEnable(GL_TEXTURE_GEN_R);

  glEnable(GL_TEXTURE_2D);

}

static void texturesOff(void)
{
  glDisable(GL_TEXTURE_2D);
}




void TexturedBox::setTextures(int *textures)
{
  int i;

  for (i = 0; i < 6; i++) faceTextures[i] = textures[i];
}

// Fragments of this found online; it was better than my quad strip version

//  
//

void TexturedBox::draw(double time)
{
  double x,y,z;
  double normdir = 1;
  int i;

  // Scale the textures to keep them 1:1 
  double xscale = (width < height) ? height/width : width/height;
  double yscale = (length < height) ? height/length : height/length;
  double zscale = (length < width) ? width/length : width/length;

  zscale = 1;

  // call parent class to set things up
  setTransformation();

  x = width / 2;
  y = height;
  z = length / 2;

  // Top
  if (faceTextures[TOP] != 0) {
    texturesOn(faceTextures[TOP]);
  } else {
     glColor3f(color.red, color.green, color.blue);
  }
  glBegin(GL_QUADS);
  glNormal3f(0,1*normdir,0);
  glTexCoord2f(0,0);  glVertex3f(-x,y,-z);
  glTexCoord2f(0,zscale);  glVertex3f(-x,y,z);
  glTexCoord2f(1,zscale);  glVertex3f(x,y,z);
  glTexCoord2f(1,0);  glVertex3f(x,y,-z);
  glEnd();
  texturesOff();

  // Bottom
  if (faceTextures[BOTTOM] != 0) {
    texturesOn(faceTextures[BOTTOM]);
  } else {
     glColor3f(color.red, color.green, color.blue);
  }

  glBegin(GL_QUADS);
  glNormal3f(0,-1*normdir,0);
  glTexCoord2f(0,0);  glVertex3f(-x,0,-z);
  glTexCoord2f(0,zscale);  glVertex3f(-x,0,z);
  glTexCoord2f(1,zscale);  glVertex3f(x,0,z);
  glTexCoord2f(1,0);  glVertex3f(x,0,-z);
  glEnd();
  texturesOff();

  // Left (-x)
  if (faceTextures[LEFT] != 0) {
    texturesOn(faceTextures[LEFT]);
  } else {
    glColor3f(color.red, color.green, color.blue);
  }
  
  glBegin(GL_QUADS);
  glNormal3f(-1*normdir,0,0);
  glTexCoord2f(yscale,1);  glVertex3f(-x,y,-z);
  glTexCoord2f(0,1);  glVertex3f(-x,y,z);
  glTexCoord2f(0,0);  glVertex3f(-x,0,z);
  glTexCoord2f(yscale,0);  glVertex3f(-x,0,-z);
  glEnd();
  texturesOff();


  // Right (+x)
  if (faceTextures[RIGHT] != 0) {
    texturesOn(faceTextures[RIGHT]);
  } else {
    glColor3f(color.red, color.green, color.blue);
  }

  glBegin(GL_QUADS);
  glNormal3f(1*normdir,0,0);
  glTexCoord2f(0,1);  glVertex3f(x,y,-z);
  glTexCoord2f(yscale,1);  glVertex3f(x,y,z);
  glTexCoord2f(yscale,0);  glVertex3f(x,0,z);
  glTexCoord2f(0,0);  glVertex3f(x,0,-z);
  glEnd();
  texturesOff();

  // Front (-z)
  if (faceTextures[FRONT] != 0) {
    texturesOn(faceTextures[FRONT]);
  } else {
    glColor3f(color.red, color.green, color.blue);
  }

  glBegin(GL_QUADS);
  glNormal3f(0,0,-1*normdir);
  glTexCoord2f(0,1);  glVertex3f(-x,y,-z);
  glTexCoord2f(0,0);  glVertex3f(-x,0,-z);
  glTexCoord2f(xscale,0);  glVertex3f(x,0,-z);
  glTexCoord2f(xscale,1);  glVertex3f(x,y,-z);
  glEnd();
  texturesOff();

  // Back (+z)

  if (faceTextures[BACK] != 0) {
    texturesOn(faceTextures[BACK]);
  } else {
    glColor3f(color.red, color.green, color.blue);
  }
  glBegin(GL_QUADS);
  //    glColor3f(1,0,0);
  glNormal3f(0,0,1*normdir);
  glTexCoord2f(xscale,1);  glVertex3f(-x,y,z);
  glTexCoord2f(xscale,0);  glVertex3f(-x,0,z);
  glTexCoord2f(0,0);  glVertex3f(x,0,z);
  glTexCoord2f(0,1);  glVertex3f(x,y,z);
  glEnd();
  texturesOff();

}

