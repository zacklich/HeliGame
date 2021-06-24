
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


HeliRotors :: HeliRotors(double d, double h,double a)
{
  Cylinder *cyl;
  TexturedBox *blade1;
  TexturedBox *blade2;

  angle = a;
  diameter = d;
  height = h;

  // Make a small cylinder to hold the blades

  cyl = new Cylinder(diameter*0.1,height*2);

  // Make blades
  blade1 = new TexturedBox(0.08,diameter,height);
  blade1->setDefaultColor(0.2,0.2,0.2);
  blade2 = new TexturedBox(diameter,0.08,height);
  blade2->setDefaultColor(0.2,0.2,0.2);
  addSubObject(cyl);
  addSubObject(blade1);
  addSubObject(blade2);


}

// Fragments of this found online; it was better than my quad strip version
void HeliRotors::draw(double time)
{
  // call parent class to set things up
  setTransformation();
  setRotation(0,time*720.0,angle);

}

