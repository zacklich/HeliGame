#include <stdio.h>
#include <string.h>

#include "CSCIx229.h"
#include "TextureMap.h"



TextureMap::TextureMap()
{
  ntextures = 0;
}


int TextureMap::loadTexture(const char *filename, unsigned int stlNum, double scale)
{
  int glid;

  if (ntextures == MAXTEXTURES) {
    return 0;
  }

  glid = LoadTexBMP(filename);
  if (glid < 0) {
    printf("Could not load texture file '%s'\n",filename);
    exit(1);
  } else {
    printf("Loaded texture '%s' with ID %d and STL number %u\n",filename, glid, stlNum);
  }

  textures[ntextures].filename = strdup(filename);
  textures[ntextures].glTexture = glid;
  textures[ntextures].stlNum = stlNum;
  textures[ntextures].scale = scale;
  ntextures++;

  return glid;
}


int TextureMap::findTexture(unsigned int stlNum)
{
  int i;
  int glid;

  if (stlNum == 0) {
    return 0;
  }

  for (i = 0; i < ntextures; i++) {
    if (textures[i].stlNum == stlNum) {
      glid = textures[i].glTexture;
    }
  }

  return glid;
}

double TextureMap::scaleForTexture(int glid)
{
  int i;

  if (glid == 0) {
    return 1.0;
  }

  for (i = 0; i < ntextures; i++) {
    if (textures[i].glTexture == glid) {
      return textures[i].scale;
    }
  }

  return 1.0;
}
