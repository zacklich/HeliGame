
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

static void addTriangle(triangle_t **tri, 
		 double x1, double y1, double z1,
		 double x2, double y2, double z2,
		 double x3, double y3, double z3)
{
  triangle_t *t = *tri;

  // put in triangle array.
  t->v1.x = x1; t->v1.y = y1; t->v1.z = z1;
  t->v2.x = x2; t->v2.y = y2; t->v2.z = z2;
  t->v3.x = x3; t->v3.y = y3; t->v3.z = z3;

  // Calculate the normal
  calc_normal(t,&(t->normal));

  (*tri)++;
}

static void addQuad(triangle_t **tri,
	     double x1, double y1, double z1,
	     double x2, double y2, double z2,
	     double x3, double y3, double z3,
	     double x4, double y4, double z4)
{
   addTriangle(tri, x1,y1,z1, x4,y4,z4, x2,y2,z2);
   addTriangle(tri, x3,y3,z3, x2,y2,z2, x4,y4,z4);
}



TrapezoidBox :: TrapezoidBox(double w, double l, double h, double wl, double ll)
{
  width = w;
  length = l;
  height = h;
  wlean = wl;
  llean = ll;

  computeTriangles();
}

void TrapezoidBox::computeTriangles(void)
{
    double wleanDistance = sin(degToRad(wlean))*(height/2);
    double lleanDistance = sin(degToRad(llean))*(height/2);

    triangle_t *t;

    // Bottom

    t = triangles;

    addQuad(&t,
	    -width/2.0, 0, length/2.0,
	    width/2.0, 0, length/2.0,
	    width/2.0, 0, -length/2.0,
	    -width/2.0, 0, -length/2.0);

#if 1
   // Side
    addQuad(&t,
	    width/2.0, 0,  length/2.0,
	    -width/2.0, 0, length/2.0,
	    -width/2.0+wleanDistance, height,  length/2.0-lleanDistance,
	    width/2.0-wleanDistance, height, length/2.0-lleanDistance);

    // Top
    addQuad(&t,
	    -width/2.0+wleanDistance, height, -length/2.0+lleanDistance,
	    width/2.0-wleanDistance, height, -length/2.0+lleanDistance,
	    width/2.0-wleanDistance, height, length/2.0-lleanDistance,
	    -width/2.0+wleanDistance, height,  length/2.0-lleanDistance);

    // Other Side

    addQuad(&t,
	    width/2.0-wleanDistance, height, -length/2.0+lleanDistance,
	    -width/2.0+wleanDistance, height, -length/2.0+lleanDistance,
	    -width/2.0, 0, -length/2.0,
	    width/2.0, 0, -length/2.0);

    addQuad(&t,
	    width/2.0, 0, -length/2.0,
	    width/2.0, 0, length/2.0,
	    width/2.0-wleanDistance, height,  length/2.0-lleanDistance,
	    width/2.0-wleanDistance, height, -length/2.0+lleanDistance);

    addQuad(&t,
	    -width/2.0+wleanDistance, height, -length/2.0+lleanDistance,
	    -width/2.0+wleanDistance, height,  length/2.0-lleanDistance,
	    -width/2.0, 0, length/2.0,
	      -width/2.0, 0, -length/2.0);
#endif

    ntriangles = t-triangles;
    if (ntriangles != 12) {
      //printf("should be 12 triangles.  Are we still in a 3D world?\n");
      //exit(1);
    }

}

static void texturesOn(int texture)
{

  glBindTexture(GL_TEXTURE_2D,texture);

  // enable texture mode, set color to white
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


  glColor3f(1,1,1);

  // Scale the texture
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  //  glScalef(0.2, 0.2, 0.2);
  //  glScalef(200,200,200);


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
  //    glDisable(GL_TEXTURE_GEN_S);
  //    glDisable(GL_TEXTURE_GEN_T);
  //    glDisable(GL_TEXTURE_GEN_R);
}


static void drawTrapTriangles(triangle_t *tri, int count)
{
  int i;

  glPushMatrix();
  glShadeModel(GL_SMOOTH);
  glBegin(GL_TRIANGLES);

  for (i = 0; i < count; i++) {
    //vector_t normal;

    //calc_normal(tri, &normal);

    glNormal3f(tri->normal.x, tri->normal.y, tri->normal.z);

                glTexCoord2f(0,0);
    //glTexCoord3f(tri->v1.x, tri->v1.y, tri->v1.z);
    glVertex3f(tri->v1.x, tri->v1.y, tri->v1.z);
                glTexCoord2f(1,0);
    //glTexCoord3f(tri->v2.x, tri->v2.y, tri->v2.z);
    glVertex3f(tri->v2.x, tri->v2.y, tri->v2.z);
                glTexCoord2f(0.5,1);
    // glTexCoord3f(tri->v3.x, tri->v3.y, tri->v3.z);
    glVertex3f(tri->v3.x, tri->v3.y, tri->v3.z);
    tri++;
  }

  glEnd();
  glPopMatrix();
}



// Fragments of this found online; it was better than my quad strip version
void TrapezoidBox::draw(double time)
{
  // call parent class to set things up
  setTransformation();

  if (defaultTexture != 0) {
    texturesOn(defaultTexture);
  }

  drawTrapTriangles(triangles, ntriangles);

  if (defaultTexture != 0) {
    texturesOff();
  }
}

