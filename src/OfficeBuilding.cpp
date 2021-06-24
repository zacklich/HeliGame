
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


void OfficeBuilding :: draw(double time)
{

}

void OfficeBuilding::setTextures(int *textures)
{
  int i;

  for (i = 0; i < 6; i++) faceTextures[i] = textures[i];
}

OfficeBuilding::OfficeBuilding(double w, double d, double h, int nfloors, int *textures)
{
  int i;
  TexturedBox *box1;
  TexturedBox *box2;

  width = w;
  depth = d;
  height = h;
  floors = nfloors;
  int rooftextures[6] = {textures[0],textures[0],0,0,0,0};

  for (i = 0; i < nfloors; i++) {
    box1 = new TexturedBox(w,d,h*.9);
    box1->setPosition(0,((double) i)*h,0);
    box1->setTextures(textures);

    box2 = new TexturedBox(w*1.10,d*1.10,h*.1);
    box2->setPosition(0,((double) i)*h+(h*.9),0);
    box2->setDefaultColor(0.2,0.2,0.2);
    box2->setTextures(rooftextures);
    addSubObject(box1);
    addSubObject(box2);
  }

}
