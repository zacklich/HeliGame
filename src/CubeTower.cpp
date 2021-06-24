
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
extern int roof2;


void CubeTower :: draw(double time)
{
  setTransformation();
}

void CubeTower::setTextures(int *textures)
{
  int i;

  for (i = 0; i < 6; i++) faceTextures[i] = textures[i];
}

extern int bricks;

CubeTower::CubeTower(double w, double d, double h, int *textures)
{
  int i;
  TexturedBox *box[30];
  

  width = w;
  depth = d;
  height = h;
  
  int boxTextures[6] = {bricks, bricks, bricks, bricks, bricks, bricks};
  


  for (i=0;i<30;i++){
    box[i] = new TexturedBox(w,d,h);
    box[i]->setPosition(0,h*(double)i,0);
    box[i]->setRotation(0.0,i*5.0,0.0);
    box[i]->setTextures(textures);
    addSubObject(box[i]);
  }

 
  

}
