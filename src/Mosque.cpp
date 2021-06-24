
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


void Mosque :: draw(double time)
{
  setTransformation();
}

void Mosque::setTextures(int *textures)
{
  int i;

  for (i = 0; i < 6; i++) faceTextures[i] = textures[i];
}

extern int bricks;

Mosque::Mosque(double w, double d, double h, int *textures)
{
  int i;
  TexturedBox *box1;
  TexturedBox *box2;
  Cylinder *cyl;
  Dome *dome;
  Cylinder *c2;  

  width = w;
  depth = d;
  height = h;
  
  int boxTextures[6] = {textures[LEFT],textures[RIGHT],textures[FRONT],textures[BACK],textures[LEFT],textures[RIGHT]};

    box1 = new TexturedBox(w,d,h*.1);
    box1->setPosition(0,0,0);
    box1->setTextures(boxTextures);
    addSubObject(box1);

    box2 = new TexturedBox(w*.7,d*.6,h);
    box2->setPosition(0,0,0);
    box2->setTextures(boxTextures);
    addSubObject(box2);

    double inset = 0.90;
  
    cyl = new Cylinder(d*.05,h*2);
    cyl->setPosition(-w/2*inset,h,-d/2*inset);
    cyl->setDefaultTexture(textures[BOTTOM]);
    addSubObject(cyl);

    c2 = new Cylinder(d*0.1, d*0.1);
    c2->setPosition(0,h*1.7,0);
    c2->setDefaultTexture(textures[TOP]);
    cyl->addSubObject(c2);

    
    cyl = new Cylinder(d*.05,h*2);
    cyl->setPosition(-w/2*inset,h,d/2*inset);
    cyl->setDefaultTexture(textures[BOTTOM]);
    addSubObject(cyl);

    c2 = new Cylinder(d*0.1, d*0.1);
    c2->setPosition(0,h*1.7,0);
    c2->setDefaultTexture(textures[TOP]);
    cyl->addSubObject(c2);

    
    cyl = new Cylinder(d*.05,h*2);
    cyl->setPosition(w/2*inset,h,-d/2*inset);
    cyl->setDefaultTexture(textures[BOTTOM]);
    addSubObject(cyl);
    
    c2 = new Cylinder(d*0.1, d*0.1);
    c2->setPosition(0,h*1.7,0);
    c2->setDefaultTexture(textures[TOP]);
    cyl->addSubObject(c2);


    cyl = new Cylinder(d*.05,h*2);
    cyl->setPosition(w/2*inset,h,d/2*inset);
    cyl->setDefaultTexture(textures[BOTTOM]);
    addSubObject(cyl);

    c2 = new Cylinder(d*0.1, d*0.1);
    c2->setPosition(0,h*1.7,0);
    c2->setDefaultTexture(textures[TOP]);
    cyl->addSubObject(c2);


    dome = new Dome(20,20,(d/2)*.5);
    dome->setPosition(0,h,0);
    dome->setDefaultTexture(textures[TOP]);
    addSubObject(dome);
  

}
