
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


GenObject :: GenObject()
{
  //constructor goes here
  xpos = 0;
  ypos = 0;
  zpos = 0;
  cur_pitch = 0;
  cur_yaw = 0;
  cur_roll = 0;
  size = 1;

  // If not specified, the color is gray.
  color.red = 0.5;
  color.green = 0.5;
  color.blue = 0.5;

  defaultTexture = 0;
  
}


void GenObject:: setDefaultTexture(int texID)
{
  defaultTexture = texID;
}

void GenObject::setDefaultColor(float red, float green, float blue)
{
  color.red = red;
  color.green = green;
  color.blue = blue;
}

void GenObject:: addSubObject(GenObject *obj)
{
  subObjects.push_back(obj);
}


void GenObject::drawSubObjects(double curTime)
{
  vector<GenObject *>::const_iterator obj;

  for (obj = subObjects.begin(); obj != subObjects.end(); ++obj) {
    (*obj)->drawAll(curTime);
  }

}

void GenObject :: drawAll(double curTime) 
{
  glPushMatrix();
  draw(curTime);
  drawSubObjects(curTime);
  glPopMatrix();
}



void GenObject :: setPosition(double x, double y, double z)
{
  xpos = x;
  ypos = y;
  zpos = z;
}

void GenObject :: setRotation (double pitch, double yaw, double roll)
{
  cur_pitch = pitch;
  cur_yaw = yaw;
  cur_roll = roll;
}

void GenObject :: setScale(double scale)
{
  size = scale;
}

void GenObject :: setTransformation(void)
{
    // Translate to where the object's origin is
    glTranslatef(xpos, ypos, zpos);
    glRotatef(cur_yaw, 0, 1, 0);
    glRotatef(cur_roll, 1, 0, 0);
    glRotatef(cur_pitch, 0, 0, 1);

    // Allow object scaling    
    glScalef(size,size,size);

    // Set the default color
    glColor3f(color.red, color.green, color.blue);
    
}

void GenObject :: draw(double time)
{
  
}

