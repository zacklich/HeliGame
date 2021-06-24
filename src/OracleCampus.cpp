
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


OracleCampus :: OracleCampus(int texture)
{
  Cylinder *cyl1;
  Cylinder *cyl2;
  Cylinder *cyl3;

  cyl1 = new Cylinder(8,10);
  cyl1->setDefaultTexture(texture);
  cyl1->setPosition(-4.5,5,4.5);

  cyl2 = new Cylinder(8,15);
  cyl2->setDefaultTexture(texture);
  cyl2->setPosition(4.5,7.5,4.5);

  cyl3 = new Cylinder(8,20);
  cyl3->setDefaultTexture(texture);
  cyl3->setPosition(0,10,0);

  addSubObject(cyl1);
  addSubObject(cyl2);
  addSubObject(cyl3);

}





void OracleCampus::draw(double time)
{
  // call parent class to set things up
  setTransformation();

}

