#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

// See: http://www.sgh1.net/posts/read-stl-file.md

typedef struct header_s {
  char header[80];
  unsigned int numtri;
} header_t;

typedef struct vertex_s {
  float x;
  float y;
  float z;
} vertex_t;

typedef struct __attribute__((packed)) triangle_s {
  vertex_t normal;
  vertex_t vertices[3];
  unsigned char attrbytes[2];
} triangle_t;  //https://gcc.gnu.org/onlinedocs/gcc-3.3/gcc/Type-Attributes.html

typedef struct vector_s {
  float x;
  float y;
  float z;
} vector_t;


typedef struct gltriangle_s {
  vector_t normal;
  vertex_t v1;
  vertex_t v2;
  vertex_t v3;
} gltriangle_t;


gltriangle_t model[1000];


// use the "right hand rule" to choose orientation
// and calculate the vertex cross product of the 
// vertex to make its normal.

// Create a vector by subtracting two vertices
void make_vector(vector_t *vect, vertex_t *v1, vertex_t *v2)
{
  vect->x = v1->x - v2->x;
  vect->y = v1->y - v2->y;
  vect->z = v1->z - v2->z;
}

void calc_normal(triangle_t *tri, vector_t *norm)
{
  vector_t v1,v2;
  float w;

  make_vector(&v1,&tri->vertices[0],&tri->vertices[1]);
  make_vector(&v2,&tri->vertices[0],&tri->vertices[2]);

  // we now have two vectors.  Compute the cross product.
  // https://www.mathsisfun.com/algebra/vectors-cross-product.html

  norm->x = v1.y*v2.z - v1.z*v2.y;
  norm->y = v1.z*v2.x - v1.x*v2.z;
  norm->z = v1.x*v2.y - v1.y*v2.x;

  // https://stackoverflow.com/questions/28490705/normalize-a-vector-from-a-struct-in-c
  w = sqrtf(norm->x*norm->x + norm->y*norm->y + norm->z*norm->z);

  if (w != 0) {
    norm->x = norm->x / w;
    norm->y = norm->y / w;
    norm->z = norm->z / w;
  }

}

int main(int argc,char *argv[])
{
  int fh;
  header_t header;
  triangle_t triangle;
  unsigned int i;
  float minY;
  float t;
  unsigned int attrID;

  if (argc < 2) {
    printf("Usage: parse_stl filename\n");
    exit(1);
  }

  printf("Sizeof(triangle_t) = %ld\n",sizeof(triangle_t));

  if (0) {
    triangle_t tri;
    vector_t norm;
    tri.vertices[0].x = -1; tri.vertices[0].y = -1; tri.vertices[0].z = 0;
    tri.vertices[1].x =  1; tri.vertices[1].y = -1; tri.vertices[1].z = 0;
    tri.vertices[2].x =  0; tri.vertices[2].y =  1; tri.vertices[2].z = 0;
    calc_normal(&tri,&norm);
    printf("Normal = (%f,%f,%f)\n",norm.x,norm.y,norm.z);
    exit(1);
  }

  fh = open(argv[1],O_RDONLY);
  if (fh < 0) {
    perror(argv[1]);
    exit(1);
  }

  read(fh,&header,sizeof(header));

  printf("# of triangles: %u\n",header.numtri);

  for (i = 0; i < header.numtri; i++) {
    vector_t normal;

    if (read(fh,&triangle,sizeof(triangle)) != sizeof(triangle)) {
      perror("read");
      exit(1);
    }

    attrID = ((unsigned int) triangle.attrbytes[0]) + ((unsigned int ) triangle.attrbytes[1])*256;
    printf("[%u]  V1=(%f,%f,%f)  V2=(%f,%f,%f)  V3=(%f,%f,%f)  Norm=(%f,%f,%f)  [%02X %02X]\n",
	   attrID,
	   triangle.vertices[0].x,triangle.vertices[0].y,triangle.vertices[0].z,
	   triangle.vertices[1].x,triangle.vertices[1].y,triangle.vertices[1].z,
	   triangle.vertices[2].x,triangle.vertices[2].y,triangle.vertices[2].z,
	   triangle.normal.x, triangle.normal.y, triangle.normal.z,
	   triangle.attrbytes[0], triangle.attrbytes[1]);

    calc_normal(&triangle, &normal);
    printf("  --> Calculated Normal (%f,%f,%f)\n\n",normal.x,normal.y,normal.z);


    model[i].normal.x = normal.x;
    model[i].normal.y = normal.y;
    model[i].normal.z = normal.z;

    model[i].v1.x = triangle.vertices[0].x;
    model[i].v1.y = triangle.vertices[0].y;
    model[i].v1.z = triangle.vertices[0].z;

    model[i].v2.x = triangle.vertices[1].x;
    model[i].v2.y = triangle.vertices[1].y;
    model[i].v2.z = triangle.vertices[1].z;

    model[i].v3.x = triangle.vertices[2].x;
    model[i].v3.y = triangle.vertices[2].y;
    model[i].v3.z = triangle.vertices[2].z;

  }

  close(fh);

  // Translate the model upwards such that the min coordinate of Y is 0

  minY = 1.0E10;


  for (i = 0; i < header.numtri; i++) {
    if (model[i].v1.y < minY) minY = model[i].v1.y;
    if (model[i].v2.y < minY) minY = model[i].v2.y;
    if (model[i].v3.y < minY) minY = model[i].v3.y;
  }

  // move the model
  for (i = 0; i < header.numtri; i++) {
    model[i].v1.y -= minY;
    model[i].v2.y -= minY;
    model[i].v3.y -= minY;
  }

  if (argc > 3) {
    float scale;
    scale = atof(argv[3]);

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

  // move the model

  if (argc > 2) {
    FILE *str;
    str = fopen(argv[2],"w");

    for (i = 0; i < header.numtri; i++) {
      fprintf(str,"    {{%f,%f,%f},  {%f,%f,%f},  {%f,%f,%f}},\n",
	      model[i].v1.x, model[i].v1.y, model[i].v1.z, 
	      model[i].v2.x, model[i].v2.y, model[i].v2.z, 
	      model[i].v3.x, model[i].v3.y, model[i].v3.z);
    }

    fclose(str);
  }


  exit(0);
}
