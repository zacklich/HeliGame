
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


void Courthouse :: draw(double time)
{
  setTransformation();
}

void Courthouse::setTextures(int *textures)
{
  int i;

  for (i = 0; i < 6; i++) faceTextures[i] = textures[i];
}

extern int bricks;

Courthouse::Courthouse(double w, double d, double h, int *textures)
{
  int i;
  TexturedBox *box1;
  TexturedBox *box2;
  Cylinder *cyl;
  TexturedRoof *roof;
  

  width = w;
  depth = d;
  height = h;
  
  int boxTextures[6] = {bricks, bricks, bricks, bricks, bricks, bricks};

    box1 = new TexturedBox(w,d,h*.1);
    box1->setPosition(0,0,0);
    box1->setTextures(textures);
    addSubObject(box1);

    box2 = new TexturedBox(w*.7,d*.6,h);
    box2->setPosition(0,0,0);
    box2->setTextures(boxTextures);
    addSubObject(box2);

    double inset = 0.90;
  
    cyl = new Cylinder(d*.05,h);
    cyl->setPosition(-w/2*inset,h/2,-d/2*inset);
    cyl->setDefaultTexture(textures[0]);
    addSubObject(cyl);
    
    cyl = new Cylinder(d*.05,h);
    cyl->setPosition(-w/2*inset,h/2,d/2*inset);
    cyl->setDefaultTexture(textures[0]);
    addSubObject(cyl);
    
    cyl = new Cylinder(d*.05,h);
    cyl->setPosition(w/2*inset,h/2,-d/2*inset);
    cyl->setDefaultTexture(textures[0]);
    addSubObject(cyl);
    
    cyl = new Cylinder(d*.05,h);
    cyl->setPosition(w/2*inset,h/2,d/2*inset);
    cyl->setDefaultTexture(textures[0]);
    addSubObject(cyl);

    roof = new TexturedRoof(w,d,h*.5);
    roof->setPosition(0,h,0);
    roof->setTextures(textures);
    addSubObject(roof);

    
    /*
    box2 = new TexturedBox(w*1.05,d*1.05,h*.1);
    box2->setPosition(0,((double) i)*h+(h*.9),0);
    box2->setDefaultColor(0.2,0.2,0.2);
    box2->setTextures(rooftextures);
    addSubObject(box1);
    addSubObject(box2);
    */

  

}
