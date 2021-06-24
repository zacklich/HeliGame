
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


Dome :: Dome(int sx, int sy, double r)
{
  scalex = sx;
  scaley = sy;
  radius = r;
}



static void texturesOn(int texture)
{
  double scale;

  glBindTexture(GL_TEXTURE_2D,texture);


  glColor3f(1,1,1);

  // Scale the texture
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();

  scale = textureMap.scaleForTexture(texture);
  glScalef(scale,scale,scale);

  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_TEXTURE_2D);

}

static void texturesOff(void)
{
  glDisable(GL_TEXTURE_2D);
}




// Dome code courtesy of
// https://community.khronos.org/t/half-sphere/49408/3
// (normals and textures not included)

void Dome::draw(double time)
{
   int i, j;
   GLfloat v[scalex*scaley][3];


  // call parent class to set things up
  setTransformation();

   if (defaultTexture != 0) {
     texturesOn(defaultTexture);
   }

   for (i=0; i<scalex; ++i) {
     for (j=0; j<scaley; ++j) {
       v[i*scaley+j][0]=radius*cos(j*2*M_PI/scaley)*cos(i*M_PI/(2*scalex));
       v[i*scaley+j][1]=radius*sin(i*M_PI/(2*scalex));
       v[i*scaley+j][2]=radius*sin(j*2*M_PI/scaley)*cos(i*M_PI/(2*scalex));
     }
   }

   // The normals should be the vector from the center to the point on the dome.
   // that happens to be the same as the point itself

   // Map the texture around the circle and from bottom to top.

   glBegin(GL_QUADS);
     for (i=0; i<scalex-1; ++i) {
       for (j=0; j<scaley; ++j) {

	 glTexCoord2f((double) j / (double) scaley,
		      (double) i / (double) (scalex-1));
	 glNormal3fv(v[i*scaley+j]);
         glVertex3fv(v[i*scaley+j]);

	 glTexCoord2f((double) (j+1) / (double) scaley,
		      (double) i / (double) (scalex-1));
		      
         glNormal3fv(v[i*scaley+(j+1)%scaley]);
         glVertex3fv(v[i*scaley+(j+1)%scaley]);

	 glTexCoord2f((double) (j+1) / (double) scaley,
		      (double) (i+1) / (double) (scalex-1));
         glNormal3fv(v[(i+1)*scaley+(j+1)%scaley]);
         glVertex3fv(v[(i+1)*scaley+(j+1)%scaley]);

	 glTexCoord2f((double) (j) / (double) scaley,
		      (double) (i+1) / (double) (scalex-1));
		      
         glNormal3fv(v[(i+1)*scaley+j]);
         glVertex3fv(v[(i+1)*scaley+j]);
       }
     }
   glEnd();

   if (defaultTexture != 0) {
     texturesOff();
   }
}

