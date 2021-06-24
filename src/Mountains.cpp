
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


double mountainPeaks[10] = {
    0, 10.0, 5.0, 2.0, 6.0, 10.0, 6.0, 9.0, 2.0, 0
    
};



void Mountain :: draw(double time)
{
    int i;
    double mountainDepth = 15;
    double width = 100;
    double height = 30;
    
    setTransformation();

    glColor3f(0.05, 0.2, 0);
    // glTranslatef(x-width/2,0,-y);

    // Draw mountains as smaller polygons so we can get normals
    for (i = 0; i < 9; i++) {

      //      glMatrixMode(GL_TEXTURE);
      //      glLoadIdentity();
      //      glScalef(2,2,2);
      //      glMatrixMode(GL_MODELVIEW);

      //glEnable(GL_TEXTURE_2D);

      //glColor3f(1,1,1);
      //glBindTexture(GL_TEXTURE_2D,mountain_texture);

      glBegin(GL_POLYGON);
      glNormal3f(0,0,-1);

      glTexCoord2f(0, 0);
      glVertex3f(width*((double)i)/10.0, 0, 0);

      glTexCoord2f(1, 0);
      glVertex3f(width*((double)(i+1))/10.0, 0, 0);

      glTexCoord2f(1, mountainPeaks[i+1]/10.0);
      glVertex3f(width*((double)(i+1))/10.0, mountainPeaks[i+1], 0);

      glTexCoord2f(0, mountainPeaks[i]/10.0);
      glVertex3f(width*((double)(i))/10.0, mountainPeaks[i], 0);

      glEnd();
      //      glDisable(GL_TEXTURE_2D);
    }

    // in the Flat Earth, There's no texture on the back side of the mountain.  

    for (i = 0; i < 9; i++) {
      glBegin(GL_POLYGON);
      glNormal3f(0,0,1);
      glVertex3f(width*((double)i)/10.0, 0, mountainDepth);
      glVertex3f(width*((double)(i+1))/10.0, 0, mountainDepth);
      glVertex3f(width*((double)(i+1))/10.0, mountainPeaks[i+1], mountainDepth);
      glVertex3f(width*((double)(i))/10.0, mountainPeaks[i], mountainDepth);
      glEnd();
    }


    // Top of the mountain

    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D,mountain_texture);

    glBegin(GL_QUADS);
    glColor3f(1, 1, 0);
    glNormal3f(0,1,0);

    for (i = 0; i < 9; i++) {
      triangle_t tri;
      vector_t normal;

      // Use three of the four points to calculate the normal
      tri.v1.x = width*((double) i) / 10.0;
      tri.v1.y = mountainPeaks[i];
      tri.v1.z = 0;

      tri.v2.x = width*((double) i) / 10.0;
      tri.v2.y = mountainPeaks[i];
      tri.v2.z = mountainDepth;

      tri.v3.x = width*((double)(i+1))/10.0;
      tri.v3.y = mountainPeaks[i+1];
      tri.v3.z = mountainDepth;

      calc_normal(&tri,&normal);
      glNormal3f(normal.x,normal.y,normal.z);

      glTexCoord2f(0, 0);
      glVertex3f(width*((double) i)/10.0, mountainPeaks[i], 0);

      glTexCoord2f(0, 1);
      glVertex3f(width*((double) i)/10.0, mountainPeaks[i], mountainDepth);

      glTexCoord2f(1, 1);
      glVertex3f(width*((double) (i+1))/10.0, mountainPeaks[i+1], mountainDepth);

      glTexCoord2f(1, 0);
      glVertex3f(width*((double) (i+1))/10.0, mountainPeaks[i+1], 0);
    }
    glEnd();
    //glDisable(GL_TEXTURE_2D);
    
}
