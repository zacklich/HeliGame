/*
 * 3-D helicopter "game"
 *
 * Zachariah Lichtenberg
 *
 */



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

#include <vector>

#include "GenObject.h"
#include "Scene.h"
#include "TextureMap.h"
#include "CityMaze.h"


static GLfloat fps = -1;
static GLint Frames = 0;
static GLint autoexit = 0;
static GLint win = 0;
double flyingRadius = 40;
double sunRadius = 15;

double viewScale = 1.0;

double altitude = 1;
int localViewer = 0;
int auto_sun = 1;
int sun_motion = 0;
int sun_display = 0;
int axis_display = 0;
int grid_display = 0;

int test_mode = 0;

int skybox_size = 200;
GLfloat skybox_level = -80;

#define FPS 60

int width = 16*50;
int height = 9*50;

// lighting info
static GLfloat light_pos[4] = {0.0, 10.0, 0.0, 1.0};
static GLfloat light_diffuse[4] = {1.0, 1.0, 1.0, 1.0};
static GLfloat light_ambient[4] = {0.1, 0.1, 0.1, 1.0};
static GLfloat light_specular[4] = {0.0, 0.0, 0.0, 1.0};
int lighting_on = 1;

struct Motion
{
    bool Forward,Backward,Left,Right;
};

struct Motion motion = {false,false,false,false};


#define MODE_TOP  0
#define MODE_ISO  1
#define MODE_FP   2
#define MODE_END  2

int mode = MODE_FP;

// Objects.

CityMaze *maze = NULL;
Scene scene;
TextureMap textureMap;
SkyBox *skybox = NULL;
STLObject *playerHeli = NULL;
STLObject *robotHeli = NULL;
STLObject *lorenzHeli = NULL;

double stationaryX = 0;
double stationaryY = 0;
// Textures
int dirt;
int grass;
int camo1;
int asphalt;
int roof;
int roof2;
int bricks;
int glass1,glass2,glass3;
int marble;
int marbledoor;
int sky[6];


// City info
double cityBlockSize = 20;
int cityWidth,cityHeight;
int citySize = 4;

// Time
double currentTime = 0;


// Camera
float pitch = 0.0, yaw= 0.0;
float camX=0.0,camZ=5.0;

// Mouse status
bool mouseIsDown = false;
int mouse_prev_x = 0;
int mouse_prev_y = 0;



// Create a vector by subtracting two vertices
void make_vector(vector_t *vect, vertex_t *v1, vertex_t *v2)
{
  vect->x = v1->x - v2->x;
  vect->y = v1->y - v2->y;
  vect->z = v1->z - v2->z;
}

// Calculate the normal using the "right hand rule"
void calc_normal(triangle_t *tri, vector_t *norm)
{
  vector_t v1,v2;
  float w;

  make_vector(&v1,&tri->v1,&tri->v2);
  make_vector(&v2,&tri->v1,&tri->v3);

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




double degToRad(double deg)
{
    return (deg/180)*M_PI;
}





void drawGrid(double size, double step)
{
    double x;
    
    glBegin(GL_LINES);
    glColor3f(0.2,0.2,0.2);

    for (x = -size/2;  x <= size/2; x += step) {
        glVertex3f(-size/2.0, 0, x);
        glVertex3f(size/2.0,  0, x);
        glVertex3f(x,         0, -size/2.0);
        glVertex3f(x,         0, size/2.0);

    }
    
    glEnd();
}



static GLfloat angle = 0.0;
static GLfloat sunAngle = 0.0;

static void
cleanup(void)
{
   glutDestroyWindow(win);
}





#define LEN 8192  //  Maximum length of text string
static void
Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

//
// If the user presses and holds the mouse button down, do the pitch and yaw
// mouse_event gets called when you press and release the button.
// On the first press remember where the cursor is
// then everything is relative to that.


void mouse_event(int button, int state, int x, int y)
{

  if (button == GLUT_RIGHT_BUTTON) {
    if (state == GLUT_DOWN) {
      auto_sun = 0;
      light_pos[0] = ((float)(width/2-x))*0.2;
      light_pos[2] = ((float)(height/2-y))*0.2;
    } else {
    auto_sun = 1;
    }
  }

  // Mouse scrolling for scalem
  if ((button == 3) || (button == 4)) {
    if (state == GLUT_DOWN) {
      if (button == 3) viewScale -= 0.002;
      if (button == 4) viewScale += 0.002;
    }
  }
  //  printf("mouse_event: event=%d state=%d x=%d y=%d\n",button,state,x,y);
  if (!mouseIsDown && (button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
    mouseIsDown = true;
    mouse_prev_x = x;
    mouse_prev_y = y;
  } else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
    mouseIsDown = false;
  }
}

void mouse_motion(int x,int y)
{
    /* X and Y are relative to the window, so figure out how far it moved */

  if (auto_sun == 0) {
      light_pos[0] = ((float)(width/2-x))*0.2;
      light_pos[2] = ((float)(height/2-y))*0.2;
      return;
  }

  /* do not do anything if the mouse isn't down */
  if (!mouseIsDown) {
    return;
  }

  /* Calculate how far we moved */

    int dev_x,dev_y;
    dev_x = (mouse_prev_x - x);
    dev_y = (mouse_prev_y - y);

    /* save current coordinates, next time we are called we want to know
       how far we moved since last time */

    mouse_prev_x = x;
    mouse_prev_y = y;

    //    printf("dev_x=%d, dev_y=%d,    x=%d, y=%d\n",dev_x,dev_y,x,y); return;

    /* apply the changes to pitch and yaw*/
    yaw+=(float)dev_x/10.0;
    pitch+=(float)dev_y/10.0;
}

void camera()
{

  //    printf("Camera: X=%g Z=%g, Pitch=%g, Yaw=%g, Altitude=%f\n",camX,camZ,pitch,yaw,altitude);
    if(motion.Forward)
    {
        camX += cos((yaw+90)*TO_RADIANS)/5.0;
        camZ -= sin((yaw+90)*TO_RADIANS)/5.0;
	altitude += sin(pitch*TO_RADIANS)/2.0;
    }
    if(motion.Backward)
    {
        camX += cos((yaw+90+180)*TO_RADIANS)/5.0;
        camZ -= sin((yaw+90+180)*TO_RADIANS)/5.0;
	altitude -= sin(pitch*TO_RADIANS)/2.0;
    }
    if(motion.Left)
    {
        camX += cos((yaw+90+90)*TO_RADIANS)/10.0;
        camZ -= sin((yaw+90+90)*TO_RADIANS)/10.0;
    }
    if(motion.Right)
    {
        camX += cos((yaw+90-90)*TO_RADIANS)/10.0;
        camZ -= sin((yaw+90-90)*TO_RADIANS)/10.0;
    }

    if (altitude < 1.5)
    {
       altitude = 1.5;
    }

    /*limit the values of pitch
      between -60 and 70
    */
    //    if(pitch>=30)
    //        pitch = 30;
    //    if(pitch<=-30)
    //        pitch=-30;

    glRotatef(-pitch,1.0,0.0,0.0); // Along X axis
    glRotatef(-yaw,0.0,1.0,0.0);    //Along Y axis

    glTranslatef(-camX,-altitude,-camZ);
}

 static void
 drawAxes()
 {
   double len = 10;
   glBegin(GL_LINES);
   glVertex3d(0.0,0.0,0.0);
   glVertex3d(len,0.0,0.0);
   glVertex3d(0.0,0.0,0.0);
   glVertex3d(0.0,len,0.0);
   glVertex3d(0.0,0.0,0.0);
   glVertex3d(0.0,0.0,len);
   glEnd();
   //  Label axes
   glRasterPos3d(len,0.0,0.0);
   Print("X");
   glRasterPos3d(0.0,len,0.0);
   Print("Y");
   glRasterPos3d(0.0,0.0,len);
   Print("Z");
 }



static void update_time(void)
{
  GLint t = glutGet(GLUT_ELAPSED_TIME);
  static GLint T0 = 0;

  // Set the scene's current time from GLUT_ELAPSED_TIME
  currentTime = (double) t / 1000.0;
  scene.setTime(currentTime);

  // Compute frame rate stats

  if (t - T0 >= 5000) {
    GLfloat seconds = (t - T0) / 1000.0;
    fps = Frames / seconds;
    printf("%d frames in %6.3f seconds = %6.3f FPS\n", Frames, seconds, fps);
    T0 = t;
    Frames = 0;
  }
}

static void lorenz(int numIter,GLfloat *xpos, GLfloat *ypos, GLfloat *zpos)
{
static double s  = 10;
static double b  = 2.6666;
static double r  = 28;

    int i;
    /*  Coordinates  */
    double x = 1;
    double y = 1;
    double z = 1;
    /*  Time step  */
    double dt = 0.001;
    
    
    /*
     *  Explicit Euler integration
     */
    
    
    for (i=0;i<numIter;i++)
    {
        double dx = s*(y-x);
        double dy = x*(r-z)-y;
        double dz = x*y - b*z;
        
      
        
        x += dt*dx;
        y += dt*dy;
        z += dt*dz;
    }

    double scale = 0.25;

    *xpos = x/scale; *ypos = y/scale; *zpos = z/scale;
}

static void update_robots(void)
{
    double angle = currentTime * 20.0;
    angle = fmod(angle, 360.0);

    robotHeli->setPosition(flyingRadius * cos(degToRad(-angle)),
			   25,
			   flyingRadius * sin(degToRad(-angle)));
    robotHeli->setRotation(0,angle-90.0,30);


    GLfloat lX,lY,lZ;
    static GLfloat prev_lX,prev_lY,prev_lZ;

    lorenz(currentTime * 100,&lX,&lY,&lZ);

    angle = atan((lY-prev_lY)/(lX-prev_lX))*180*M_PI;

    lorenzHeli->setPosition(lX,30,lY);
    lorenzHeli->setRotation(0,angle,0);
    prev_lX = lX; prev_lY = lY; prev_lZ = lZ;

}


static void draw(void)
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  update_time();

  glPushMatrix();

  // Set "camera" location by placing scene where we want it
  camera();

  glScalef(viewScale,viewScale,viewScale);

  //  glPushMatrix();

  if (auto_sun) { 
    light_pos[0] = sunRadius*cos(degToRad(sunAngle));
    light_pos[2] = sunRadius*sin(degToRad(sunAngle));
  }
  light_pos[1] = 100;     // distance of light source from grid

  // Draw the "sun"
  glDisable(GL_LIGHTING);

  if (sun_display) {
    glPushMatrix();
    glColor3f(1.0,1.0,0);
    glTranslatef(light_pos[0],light_pos[1],light_pos[2]);
    glutSolidSphere(1.0, 20,20);
    glPopMatrix();
  }

  //  Location of viewer for specular calculations
  if (lighting_on) {
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,localViewer);
    //  glColor sets ambient and diffuse color materials
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
  } else {
    glDisable(GL_LIGHTING);
  }


  // Handy axes so we know which way is up
  if (axis_display) drawAxes();

  // Draw the ground (a grid)
  if (grid_display)  drawGrid(100, 1);
    
  Frames++;

  // Draw the skybox separately.  It is centered around
  // the camera position.

  // In 'test_mode' the skybox, maze, and player helicopter might not be
  // created, so test for them before drawing.

  if (skybox) {
    skybox->setPosition(camX,skybox_level, camZ);
    skybox->drawAll(0);
  }
  // Draw the city.  

  if (maze) {
    maze->draw(0);
  }

  // Draw the objects in the city.w
  scene.draw();

  // Draw the player's helicopter.  This is drawn like the skybox
  // relative to the camera.  We want the heli to appear
  // in front of the camera, 3rd person view.

  //  glPopMatrix();

  // Distance from camera to helicopter
  double heliDistance = 7.0;   

  if (playerHeli) {
    playerHeli->setPosition(camX - heliDistance*sin(yaw*TO_RADIANS),altitude-1,
			    camZ-heliDistance*cos(yaw*TO_RADIANS));
    playerHeli->setRotation(-pitch,-90+yaw,0);
    playerHeli->drawAll(currentTime);
  }

  // update animation
  if (test_mode == 0) {
    update_robots();
  }

  glPopMatrix();

  glColor3f(1,1,1);
  glWindowPos2i(5,25);
  Print("Camera: X=%g Z=%g  Pitch=%g Yaw=%g, Altitude=%g", camX, camZ, pitch, yaw, altitude);

  glutSwapBuffers();
}


static void
idle(void)
{
  static double t0 = -1.;
  double dt, t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
  if (t0 < 0.0)
    t0 = t;
  dt = t - t0;
  t0 = t;

  angle += 70.0 * dt;  /* 70 degrees per second */
  angle = fmod(angle, 360.0); /* prevents eventual overflow */

  if (sun_motion) {
    sunAngle += 20 * dt;
    sunAngle = fmod(sunAngle, 360.0); /* prevents eventual overflow */
  }

}


/*this function is used to keep calling the display function periodically
  at a rate of FPS times in one second. The constant FPS is defined above and
  has the value of 60
*/
void timer(int x)
{
    glutPostRedisplay();
    glutTimerFunc(1000/FPS,timer,0);
}

static void update_mode(int mode)
{
  switch (mode) {
  case MODE_TOP:
    camX = 0;
    camZ = 0;
    pitch = -90;
    yaw = 0;
    altitude = 100;
    break;
  case MODE_ISO:
    pitch = -33;
    yaw = -142;
    altitude = 100;
    camX = -15;
    camZ = -15;
    break;
  case MODE_FP:
    altitude = -1;
    pitch = 0;
    yaw = 0;
    camZ = 0;
    camZ = -15;
    break;
  }
}


/* change view angle, exit upon ESC */
/* ARGSUSED1 */
static void
key_down(unsigned char k, int x, int y)
{
  switch (k) {

  case '@':
    lighting_on = !lighting_on;
    break;

  case 'v':
  case 'V':
    axis_display = !axis_display;
    break;
  case 'g':
  case 'G':
    grid_display = !grid_display;
    break;

  case '<':
    sunRadius -= 0.5;
    break;

  case '>':
    sunRadius += 0.5;
    break;

  case '.':
    sun_motion = !sun_motion;
    break;

  case ',':
    sun_display = !sun_display;
    break;

  case 'Q':
  case 'q':
    localViewer= !localViewer;
    break;

  case '[':
    light_ambient[0] = light_ambient[0] - 0.1;
    if (light_ambient[0] < 0) light_ambient[0] = 0;
    light_ambient[1] = light_ambient[0];
    light_ambient[2] = light_ambient[0];
    break;

  case ']':
    light_ambient[0] = light_ambient[0] + 0.1;
    if (light_ambient[0] > 1.0) light_ambient[0] = 1.0;
    light_ambient[1] = light_ambient[0];
    light_ambient[2] = light_ambient[0];
    break;

  case '{':
    light_diffuse[0] = light_diffuse[0] - 0.1;
    if (light_diffuse[0] < 0) light_diffuse[0] = 0;
    light_diffuse[1] = light_diffuse[0];
    light_diffuse[2] = light_diffuse[0];
    break;

  case '}':
    light_diffuse[0] = light_diffuse[0] + 0.1;
    if (light_diffuse[0] > 1.0) light_diffuse[0] = 1.0;
    light_diffuse[1] = light_diffuse[0];
    light_diffuse[2] = light_diffuse[0];
    break;


  case '(':
    light_specular[0] = light_specular[0] - 0.1;
    if (light_specular[0] < 0) light_specular[0] = 0;
    light_specular[1] = light_specular[0];
    light_specular[2] = light_specular[0];
    break;

  case ')':
    light_specular[0] = light_specular[0] + 0.1;
    if (light_specular[0] > 1.0) light_specular[0] = 1.0;
    light_specular[1] = light_specular[0];
    light_specular[2] = light_specular[0];
    break;


  case 27:  /* Escape */
    cleanup();
    exit(0);
    break;
  case 'W':
  case 'w':
    motion.Forward = true;
    break;
  case 'A':
  case 'a':
    motion.Left = true;
    break;
  case 'S':
  case 's':
    motion.Backward = true;
    break;
  case 'D':
  case 'd':
    motion.Right = true;
    break;

  case 'L':
    light_pos[1] += 1.0;
    printf("Light now at %g\n",light_pos[1]);
    break;

  case 'l':
    light_pos[1] -= 1.0;
    printf("Light now at %g\n",light_pos[1]);
    break;

  case 'M':
  case 'm':
    mode = mode + 1;
    if (mode >= MODE_END) mode = 0;
    update_mode(mode);

    break;
  case 'H':
  case 'h':
    camX = 0; 
    camZ = 5;
    pitch = 0;
    yaw = 0;
    altitude = 20;
    break;


  default:
    return;
  }
  glutPostRedisplay();
}

void keyboard_up(unsigned char key,int x,int y)
{
    switch(key)
    {
    case 'W':
    case 'w':
        motion.Forward = false;
        break;
    case 'A':
    case 'a':
        motion.Left = false;
        break;
    case 'S':
    case 's':
        motion.Backward = false;
        break;
    case 'D':
    case 'd':
        motion.Right = false;
        break;
    }
}


/* change view angle */
/* ARGSUSED1 */
static void
special(int k, int x, int y)
{

  switch (k) {
  case GLUT_KEY_PAGE_UP:
    altitude = altitude + 1;
    break;
  case GLUT_KEY_PAGE_DOWN:
    altitude = altitude - 1;
    break;

  case GLUT_KEY_UP:
    //    view_rotx += 5.0;
    break;
  case GLUT_KEY_DOWN:
    //    view_rotx -= 5.0;
    break;
  case GLUT_KEY_LEFT:
    yaw += 1.0;
    break;
  case GLUT_KEY_RIGHT:
    yaw -= 1.0;
    break;
  default:
    return;
  }

  glutPostRedisplay();
}

/* new window size or exposure */
static void
reshape(int _width, int _height)
{
  GLfloat h = (GLfloat) _height / (GLfloat) _width;
  GLfloat aspect = (GLfloat) _width / (GLfloat) _height;

  printf("Reshaped w=%d, h=%d, Aspect Ratio %f\n",_width,_height,aspect);

  width = _width;
  height = _height;

  glViewport(0, 0, (GLint) _width, (GLint) _height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //  gluPerspective(60,aspect,1,75);
  gluPerspective(60,aspect,1,skybox_size*2);
  //glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}


static void
init(int argc, char *argv[])
{
  GLint i;


}

static void
visible(int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc(idle);
  else
    glutIdleFunc(NULL);
}

static OfficeBuilding *office(void)
{
  int bldgTextures[6];
  int sides = (rand() % 2) ? glass1 : glass2;
  int height = (rand() % 8) + 1;

  double heightVariance = (double) (rand() % 10) * 0.1;
  double width = (double) (rand() % 8) + 10;
  double length = (double) (rand() % 8) + 10;

  bldgTextures[TOP] = roof;
  bldgTextures[BOTTOM] = 0;
  bldgTextures[FRONT] = sides;
  bldgTextures[RIGHT] = sides;
  bldgTextures[BACK] = sides;
  bldgTextures[LEFT] = sides;

  OfficeBuilding *bldg = new OfficeBuilding(width,length,5+heightVariance,height,bldgTextures);

  return bldg;
}

static GenObject *building(int *isHelipad)
{
  GenObject *block = new FlatArea(cityBlockSize, cityBlockSize, dirt);

  *isHelipad = 0;

  switch (rand() % 10) {
  case 0:
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
    {
      OfficeBuilding *bldg = office();
      block->addSubObject(bldg);
    }
    break;
  case 6:
    {
      OracleCampus *oracle = new OracleCampus(glass3);
      block->addSubObject(oracle);
    }
    break;
  case 7:
    {
      int ctexts[6] = {marble,marble, marble, marble, marble, marble};
      Courthouse *chouse = new Courthouse(12,14,5,ctexts);
      block->addSubObject(chouse);
    }
    break;
  case 8:
    {
      int ctexts[6] = {roof2,marble, marble, marble, marble, marble};
      Mosque *mosque = new Mosque(12,14,5,ctexts);
      block->addSubObject(mosque);
    }
    break;
  case 9:
    {
      if (rand()%4==0){
	int marbles[6] = {marble,marble,marble,marble,marble,marble};
	CubeTower *ct = new CubeTower(5,5,0.5,marbles);
	block->addSubObject(ct);
      }
      else{
	Helipad *pad = new Helipad(12);
	block->addSubObject(pad);
	*isHelipad = 1;
      }
    }
    break;

  default:
    break;
  }

  return block;
}


static void getBlockCoordinates(int x, int y, GLfloat *bx, GLfloat *by)
{
  *bx = (GLfloat)(x-cityWidth/2) * cityBlockSize;
  *by = (GLfloat)(y-cityHeight/2) * cityBlockSize;

}

static void loadTextures(void)
{
  grass = textureMap.loadTexture("Textures/grass.bmp",10, 10000.0);
  camo1 = textureMap.loadTexture("Textures/camo1.bmp",1882, 0.1);
  asphalt = textureMap.loadTexture("Textures/Asphalt1.bmp",11, 16.0);
  dirt = textureMap.loadTexture("Textures/Dirt.bmp",12, 16.0);
  textureMap.loadTexture("Textures/window.bmp",7768, 1.0);
  bricks = textureMap.loadTexture("Textures/Seamless_Red_Brick_Texture.bmp",28165, 2.0);
  textureMap.loadTexture("Textures/MatteBlack.bmp",5430, 1.0);
  textureMap.loadTexture("Textures/MatteBlack.bmp",21340, 1.0);
  roof2 = textureMap.loadTexture("Textures/Colorful-roof-Texture.bmp",30653, 5.0);
  roof = textureMap.loadTexture("Textures/OfficeRoof.bmp",13, 1.0);
  glass1 = textureMap.loadTexture("Textures/OfficeFacade2.bmp",14, 1.0);
  glass2 = textureMap.loadTexture("Textures/GlassOfficeFloor.bmp",15, 1.0);
  glass3 = textureMap.loadTexture("Textures/Glass3.bmp",16, 10.0);
  marble = textureMap.loadTexture("Textures/Marble.bmp",17, 1.0);
  marbledoor = textureMap.loadTexture("Textures/MarbleDoor.bmp",17, 1.0);
  textureMap.loadTexture("Textures/Glass1.bmp",24165,1.0);


  // Create the sky box

  sky[TOP] = textureMap.loadTexture("Textures/skyrender0003.bmp",20, 1.0);
  sky[BOTTOM] = textureMap.loadTexture("Textures/skyrender0006.bmp",21, 1.0);
  sky[FRONT] = textureMap.loadTexture("Textures/skyrender0005.bmp",22, 1.0);
  sky[RIGHT] = textureMap.loadTexture("Textures/skyrender0001.bmp",23, 1.0);
  sky[BACK] = textureMap.loadTexture("Textures/skyrender0002.bmp",24, 1.0);
  sky[LEFT] = textureMap.loadTexture("Textures/skyrender0004.bmp",25, 1.0);

}

static void testModeObjects(void)
{
    int ctexts[6] = {marble,marble, marble, marble, marble, marble};
    CubeTower *c = new CubeTower(5,5,.2,ctexts);
    scene.addObject(c);

}


static void createObjects(void)
{
  int x,y;
  GenObject *obj;
  HeliRotors *rotors;
  STLObject *h4;

  // Player's helicopter.

  playerHeli = new STLObject("STLs/Heli4.stl",0.1);
  rotors = new HeliRotors(4,0.02,0);
  rotors->setPosition(0,0.65,0);
  playerHeli->addSubObject(rotors);

  // Robot helicopter - flies in circles

  robotHeli = new STLObject("STLs/Heli4.stl",0.1);
  rotors = new HeliRotors(4,0.02,0);
  rotors->setPosition(0,0.65,0);
  robotHeli->addSubObject(rotors);
  scene.addObject(robotHeli);

  // Lorenz Helicopter - flies the lorenz pattern (very silly)

  lorenzHeli = new STLObject("STLs/Heli4.stl",0.1);
  rotors = new HeliRotors(4,0.02,0);
  rotors->setPosition(0,0.65,0);
  lorenzHeli->addSubObject(rotors);
  scene.addObject(lorenzHeli);

  // City maze.

  maze = new CityMaze(citySize,citySize);
  maze->show();
  maze->getCitySize(&cityWidth,&cityHeight);

  skybox = new SkyBox(skybox_size,skybox_size,skybox_size);
  skybox->setTextures(sky);
  skybox->setPosition(0,skybox_level,0);

  // Create the city

  for (y = 0; y < cityHeight; y++) {
    for (x = 0; x < cityWidth; x++) {
      GenObject *obj;
      GLfloat blockX, blockY;
      int isHelipad;

      getBlockCoordinates(x,y,&blockX,&blockY);

      if (maze->getMazeCell(x,y) & 0x0F) {
       obj = building(&isHelipad);
       // make sure stationary copter goes on a helipad
       if (isHelipad) {
	 stationaryX = blockX;
	 stationaryY = blockY;
       }
      } else {
	  obj = new FlatArea(cityBlockSize, cityBlockSize, asphalt);
	  obj->setDefaultColor(0.2,0.2,0.2);
      }

      obj->setPosition(blockX,0,blockY);

      maze->setBlock(x,y,obj);
    }
  }

  // Force the middle to be a helipad.
  Helipad *pad;

  obj = new FlatArea(cityBlockSize, cityBlockSize, dirt);
  pad = new Helipad(12);
  obj->addSubObject(pad);
  maze->setBlock(cityWidth/2, cityHeight/2, obj);
  

  // Stationary helicopter.
  h4 = new STLObject("STLs/Heli4.stl",0.1);
  h4->setPosition(stationaryX,1,stationaryY);
  h4->setDefaultColor(0,0,0);
  rotors = new HeliRotors(4,0.02,0);
  rotors->setPosition(0,0.65,0);
  h4->addSubObject(rotors);
  scene.addObject(h4);



}


int main(int argc, char *argv[])
{
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowPosition(0, 0);
  glutInitWindowSize(1000, 1000);
  win = glutCreateWindow("HeliGame");
  glEnable(GL_DEPTH_TEST);
  //  glEnable(GL_CULL_FACE);

  glutDisplayFunc(draw);
  glutMotionFunc(mouse_motion);
  glutMouseFunc(mouse_event);

  glutReshapeFunc(reshape);
    glutTimerFunc(0,timer,0);    //more info about this is given below at definition of timer()
  glutKeyboardFunc(key_down);
  glutKeyboardUpFunc(keyboard_up);

  glutSpecialFunc(special);
  glutVisibilityFunc(visible);

  // Check command line

  int i = 1;
  while (i < argc) {
    if (strcmp(argv[i],"-test") == 0) {
      test_mode = 1;
    }
    if (strcmp(argv[i],"-size") == 0) {
      i++;
      if (i < argc) {
	citySize = atoi(argv[i]);
	if (citySize > 10) citySize = 10;
	if (citySize < 4) citySize = 4;
	skybox_size = (200 + 20*(citySize-4));

      }
    }
    i++;
  }

  loadTextures();
  
  // We can put just one object to test it.
  if (test_mode) {
    grid_display = 1;
    axis_display = 1;
    testModeObjects();
  } else {
    createObjects();
  }

  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}
