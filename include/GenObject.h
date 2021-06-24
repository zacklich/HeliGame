
#include <vector>

using namespace std;

typedef struct vertex_s {
  float x;
  float y;
  float z;
} vertex_t, vector_t;    // vertexes and vectors are the same sort of thing

typedef struct triangle_s {
  vertex_t v1;
  vertex_t v2;
  vertex_t v3;
  vector_t normal;
  int textureID;
} triangle_t;

typedef struct gltriangle_s {
  vector_t normal;
  vertex_t v1;
  vertex_t v2;
  vertex_t v3;
} gltriangle_t;

typedef struct rgbcolor_s {
  float red, green, blue;
} rgbcolor_t;

extern void calc_normal(triangle_t *tri, vector_t *norm);

class GenObject;

class GenObject  {
public:
  GenObject();

  void addSubObject(GenObject *obj);
  void drawSubObjects(double time);
  void drawAll(double time);

  void setPosition(double x,double y, double z);
  void setRotation(double pitch,double yaw, double roll);
  void setScale(double scale);
  void setTransformation(void);
  void setDefaultTexture(int texID);
  void setDefaultColor(float red, float green, float blue);

  virtual void draw(double time);

protected:
  int defaultTexture;

private:
  double xpos;
  double ypos;
  double zpos;
  double cur_pitch;
  double cur_yaw;
  double cur_roll;
  double size;

  vector<GenObject *> subObjects;

 protected:
  // Default color if not textured.
  rgbcolor_t color;

  // we need to tell if something is a Helipad so we can start on one.
  bool isHelipad;
   
};

// 
// Cylinder(diameter, height)
//

class  Cylinder : public GenObject {
 private:
  double diameter;
  double height;

 public:
  Cylinder(double d, double h);
  void draw(double time);
};


// Our favorite trapezoid box

class  TrapezoidBox : public GenObject {
 private:
  double width,length;
  double wlean,llean;
  double height;

  // There are only 12 triangles in a TrapezoidBox
  triangle_t triangles[12];
  int ntriangles;

 public:
  TrapezoidBox(double w, double l, double h, double wl, double ll);
  void draw(double time);

 private:
  void computeTriangles(void);
};

enum { TOP, BOTTOM, FRONT, RIGHT, BACK, LEFT };

class  TexturedBox : public GenObject {
 private:
  double width,length;
  double height;
  int faceTextures[6];


 public:
  TexturedBox(double w, double l, double h);
  void setTextures(int *textures);
  void draw(double time);

 private:

};

class  TexturedRoof : public GenObject {
 private:
  double width,length;
  double height;
  int faceTextures[6];


 public:
  TexturedRoof(double w, double l, double h);
  void setTextures(int *textures);
  void draw(double time);

 private:

};

class  SkyBox : public GenObject {
 private:
  double width,length;
  double height;
  int faceTextures[6];


 public:
  SkyBox(double w, double l, double h);
  void setTextures(int *textures);
  void draw(double time);

 private:

};

class  OfficeBuilding : public GenObject {
 private:
  double width,depth;
  double height;
  int floors;
  int faceTextures[6];

 public:
  OfficeBuilding(double w, double d, double h,int nfloors,int *textures);
  void setTextures(int *textures);
  void draw(double time);

 private:

};

class  Courthouse : public GenObject {
 private:
  double width,depth;
  double height;
  int faceTextures[6];

 public:
  Courthouse(double w, double d, double h,int *textures);
  void setTextures(int *textures);
  void draw(double time);

 private:

};

class CubeTower : public GenObject {
 private:
  double width,depth;
  double height;
  int faceTextures[6];

 public:
  CubeTower(double w, double d, double h, int *textures);
  void setTextures(int *textures);
  void draw(double time);

 private:
  
};

class  Mosque : public GenObject {
 private:
  double width,depth;
  double height;
  int faceTextures[6];

 public:
  Mosque(double w, double d, double h,int *textures);
  void setTextures(int *textures);
  void draw(double time);

 private:

};

class  Dome : public GenObject {
 private:
  int scalex, scaley;
  double radius;


 public:
  Dome(int sx, int sy, double r);
  void draw(double time);

};

class  OracleCampus : public GenObject {
 private:


 public:
  OracleCampus(int texture);
  void draw(double time);

};

//
// Helicopter (old style)
//

class  Helicopter : public GenObject {
 public:
  void draw(double time);
};

//
// Mountain (old style)
//


class  Mountain : public GenObject {
 public:
  void draw(double time);
};

//
// Helipads
//

class  Helipad : public GenObject {
 private:
  double diameter;

 public:
  void draw(double time);
  Helipad(double d);
};

class  FlatArea : public GenObject {
  int width;
  int height;

 public:
  void draw(double time);
  FlatArea(double w, double h, int texture);
};

class  HeliRotors : public GenObject {
  double diameter;
  double height;
  double angle;

 public:
  void draw(double time);
  HeliRotors(double d, double h, double a);
};


// 
// Generic STL object
//

class STLObject : public GenObject {
  triangle_t *model;
  int numtri;

 public:
  STLObject(const char *filename, float scale);

 public:
  void draw(double time);

 private:
  int loadFile(const char* filename, float scale);
  void drawTrianglesWithTexture(triangle_t *tri,int count);
};


//
// Misc stuff
//

#ifndef M_PI
#define M_PI 3.14159265
#endif
#define TO_RADIANS (M_PI/180.0)



double degToRad(double deg);
