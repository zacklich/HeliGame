

class CityMaze {

private:
  int w,h;
  unsigned char **cell;
  int avail;

  GenObject **city;

public:
  CityMaze(int width, int height);

  unsigned int getMazeCell(int x,int y);
  void setBlock(int x, int y, GenObject *obj);
  GenObject *getBlock(int x, int y);
  void getCitySize(int *x, int *y);

  void show();

  void draw(double now);

private:
  void makeMaze(void);
  int solve(int x, int y, int tox, int toy);
  void walk(int x, int y);
};
