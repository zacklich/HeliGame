
#define MAXTEXTURES 100

typedef struct textureMap_s {
  char *filename;
  int glTexture;
  unsigned int stlNum;
  double scale;
} textureMap_t;


class TextureMap {
 public:
  TextureMap();
  int loadTexture(const char *filename, unsigned int stlNum, double scale);
  int findTexture(unsigned int stlNum);
  double scaleForTexture(int glid);

 private:
  textureMap_t textures[MAXTEXTURES];
  int ntextures;
  
};

