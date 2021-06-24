
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "GenObject.h"
#include "TextureMap.h"

//
// These structs define the STL header.
//
typedef struct stlheader_s {
  char header[80];
  unsigned int numtri;
} stlheader_t;

// See: https://gcc.gnu.org/onlinedocs/gcc-3.3/gcc/Type-Attributes.html
typedef struct __attribute__((packed)) stltriangle_s {
  vertex_t normal;
  vertex_t vertices[3];
  unsigned char attrbytes[2];
} stltriangle_t;  

extern TextureMap textureMap;


extern void drawTriangles(triangle_t *tri, int count);

// Constructor: Load the file

STLObject :: STLObject(const char *filename, float scale)
{
  loadFile(filename, scale);
}


int STLObject :: loadFile(const char* filename, float scale)
{
  int fh;
  stlheader_t header;
  stltriangle_t triangle;
  unsigned int i;
  float minY;
  float t;

 
  fh = open(filename,O_RDONLY);
  if (fh < 0) {
    perror(filename);
    exit(1);
  }

  read(fh,&header,sizeof(header));
  numtri = header.numtri;

  printf("Loading STL '%s': # of triangles: %u\n",filename,header.numtri);
  model = (triangle_t*) malloc(header.numtri*sizeof(triangle_t));
 
  for (i = 0; i < header.numtri; i++) {
    vector_t normal;

    if (read(fh,&triangle,sizeof(triangle)) != sizeof(triangle)) {
      perror("read");
      exit(1);
    }

    model[i].textureID = (int)triangle.attrbytes[0]+(int)triangle.attrbytes[1]*256;

    model[i].v1.x = triangle.vertices[0].x;
    model[i].v1.y = triangle.vertices[0].y;
    model[i].v1.z = triangle.vertices[0].z;

    model[i].v2.x = triangle.vertices[1].x;
    model[i].v2.y = triangle.vertices[1].y;
    model[i].v2.z = triangle.vertices[1].z;

    model[i].v3.x = triangle.vertices[2].x;
    model[i].v3.y = triangle.vertices[2].y;
    model[i].v3.z = triangle.vertices[2].z;

    // Calculate the normal for this triangle.
    calc_normal(&model[i], &model[i].normal);
  }

  close(fh);


  // Scale the model linearly.

 if (scale != 0) {
    
    // scale the model
    for (i = 0; i < header.numtri; i++) {
      model[i].v1.x *= scale;
      model[i].v1.y *= scale;
      model[i].v1.z *= scale;

      model[i].v2.x *= scale;
      model[i].v2.y *= scale;
      model[i].v2.z *= scale;

      model[i].v3.x *= scale;
      model[i].v3.y *= scale;
      model[i].v3.z *= scale;
    }
  }
  
 return 0;

}

extern "C" {
static int textureGenMode = 0;
};

//
// This information was useful for understanding texture maps
//
// http://cse.csusb.edu/tongyu/courses/cs520/notes/texture.php

static void texturesOn(int texture, double scale)
{

  glBindTexture(GL_TEXTURE_2D,texture);

  // enable texture mode, set color to white
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glColor3f(1,1,1);

  // Scale the texture
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glScalef(scale, scale, scale);
  glMatrixMode(GL_MODELVIEW);

  if (textureGenMode) {
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    //glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    //glEnable(GL_TEXTURE_GEN_R);
  }

  glEnable(GL_TEXTURE_2D);

}

static void texturesOff(void)
{
  glDisable(GL_TEXTURE_2D);

  if (textureGenMode) {
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    //  glDisable(GL_TEXTURE_GEN_R);
  }
}



void STLObject::drawTrianglesWithTexture(triangle_t *tri, int count)
{
  int i;
  unsigned int curTexture = 0;
  int glTexture = 0;
  double textureScale = 1.0;

  glPushMatrix();
  glShadeModel(GL_SMOOTH);


  glBegin(GL_TRIANGLES);

  for (i = 0; i < count; i++) {
    vector_t normal;

    if (curTexture != tri->textureID) {
      glEnd();
      glTexture = textureMap.findTexture(tri->textureID);
      if (glTexture != 0) {
	textureScale = textureMap.scaleForTexture(glTexture);
	texturesOn(glTexture, textureScale);
      } else {
	// untextured.
	texturesOff();
	glColor3f(color.red,color.green,color.blue);
      }
      glBegin(GL_TRIANGLES);
      curTexture = tri->textureID;
    }

    glNormal3f(tri->normal.x, tri->normal.y, tri->normal.z);

    // Bind the three corners of the triangle to roughly the bottom left,
    // bottom right, and top middle of the texture.
    if (!textureGenMode) glTexCoord2f(0,0);
    glVertex3f(tri->v1.x, tri->v1.y, tri->v1.z);
    if (!textureGenMode)  glTexCoord2f(1,0);
    glVertex3f(tri->v2.x, tri->v2.y, tri->v2.z);
    if (!textureGenMode)  glTexCoord2f(0.5,1);
    glVertex3f(tri->v3.x, tri->v3.y, tri->v3.z);
    tri++;
  }

  glEnd();
  texturesOff();

  glPopMatrix();
}


void STLObject :: draw(double time)
{

  // Set the matrix for this object
  setTransformation();

  // Draw the object
  drawTrianglesWithTexture(model,numtri);
    
}
