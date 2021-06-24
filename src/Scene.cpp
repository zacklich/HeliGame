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
#include "Scene.h"

using namespace std;

Scene::Scene()
{

}


// Add an object to the list of objects in the scene.
void Scene::addObject(GenObject *obj)
{
  objects.push_back(obj);
}

// Set the scene time.
void Scene::setTime(double curTime)
{
  now = curTime;
}

// Draw all the objects in the scene.
void Scene::draw(void)
{
  vector<GenObject *>::const_iterator obj;

  for (obj = objects.begin(); obj != objects.end(); ++obj) {
    (*obj)->drawAll(now);
  }
}
