
#include <vector>

using namespace std;

class Scene {

 private:
  vector<GenObject *> objects;
  double now;


 public:
  Scene();
  void addObject(GenObject *obj);
  void setTime(double t);
  void draw();

};
