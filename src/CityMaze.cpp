#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <sys/time.h>
#include <math.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "GenObject.h"
#include "CityMaze.h"


//
// This maze generation found online at:
// https://rosettacode.org/wiki/Maze_generation#C
//
 
enum { N = 1, S = 2, W = 4, E = 8, V = 16 };
static wchar_t glyph[] = L" │││─┘┐┤─└┌├─┴┬┼ ┆┆┆┄╯╮ ┄╰╭ ┄";

 
 
#define each(i, x, y) for (i = x; i <= y; i++)
 
static int irand(int n)
{
	int r, rmax = n * (RAND_MAX / n);
	while ((r = rand()) >= rmax);
	return r / (RAND_MAX/n);
}
 
void CityMaze::show(void)
{
  int i, j, c;
  setlocale(LC_ALL, "");
  each(i, 0, 2 * h) {
    each(j, 0, 2 * w) {
      c = cell[i][j];
      if (c > V) printf("\033[31m");
      printf("%lc", glyph[c]);
      if (c > V) printf("\033[m");
    }
    putchar('\n');
  }
}
 

unsigned int CityMaze::getMazeCell(int x, int y)
{
  return cell[y][x];
}

void CityMaze::setBlock(int x, int y, GenObject *obj)
{
  city[y*(w*2+2)+x] = obj;
}

GenObject *CityMaze::getBlock(int x, int y)
{
  return city[y*(w*2+2)+x];
}

void CityMaze::getCitySize(int *x, int *y)
{
  *x = w*2+1;
  *y = h*2+1;
}


void CityMaze::draw(double now)
{
  int i,j;
  GenObject *obj;

  for (i = 0; i <= w*2+1; i++) {
    for (j = 0; j <= h*2+1; j++) {
      obj = getBlock(i,j);
      if (obj) {
	obj->drawAll(now);
      }
    }
  }
}

inline int max(int a, int b) { return a >= b ? a : b; }
inline int min(int a, int b) { return b >= a ? a : b; }
 
static int dirs[4][2] = {{-2, 0}, {0, 2}, {2, 0}, {0, -2}};
void CityMaze::walk(int x, int y)
{
	int i, t, x1, y1, d[4] = { 0, 1, 2, 3 };
 
	cell[y][x] |= V;
	avail--;
 
	for (x1 = 3; x1; x1--)
		if (x1 != (y1 = irand(x1 + 1)))
			i = d[x1], d[x1] = d[y1], d[y1] = i;
 
	for (i = 0; avail && i < 4; i++) {
		x1 = x + dirs[ d[i] ][0], y1 = y + dirs[ d[i] ][1];
 
		if (cell[y1][x1] & V) continue;
 
		/* break walls */
		if (x1 == x) {
			t = (y + y1) / 2;
			cell[t][x+1] &= ~W, cell[t][x] &= ~(E|W), cell[t][x-1] &= ~E;
		} else if (y1 == y) {
			t = (x + x1)/2;
			cell[y-1][t] &= ~S, cell[y][t] &= ~(N|S), cell[y+1][t] &= ~N;
		}
		walk(x1, y1);
	}
}
 
int CityMaze::solve(int x, int y, int tox, int toy)
{
	int i, t, x1, y1;
 
	cell[y][x] |= V;
	if (x == tox && y == toy) return 1;
 
	each(i, 0, 3) {
		x1 = x + dirs[i][0], y1 = y + dirs[i][1];
		if (cell[y1][x1]) continue;
 
		/* mark path */
		if (x1 == x) {
			t = (y + y1)/2;
			if (cell[t][x] || !solve(x1, y1, tox, toy)) continue;
 
			cell[t-1][x] |= S, cell[t][x] |= V|N|S, cell[t+1][x] |= N;
		} else if (y1 == y) {
			t = (x + x1)/2;
			if (cell[y][t] || !solve(x1, y1, tox, toy)) continue;
 
			cell[y][t-1] |= E, cell[y][t] |= V|E|W, cell[y][t+1] |= W;
		}
		return 1;
	}
 
	/* backtrack */
	cell[y][x] &= ~V;
	return 0;
}
 
void CityMaze::makeMaze(void)
{
	int i, j;
	int h2 = 2 * h + 2, w2 = 2 * w + 2;
	unsigned char **p;
 
	p = (unsigned char **) calloc(sizeof(unsigned char *) * (h2 + 2) + w2 * h2 + 1, 1);

	city = (GenObject **) calloc(h2*w2, sizeof(GenObject *));
 
	p[1] = (unsigned char*)(p + h2 + 2) + 1;
	each(i, 2, h2) p[i] = p[i-1] + w2;
	p[0] = p[h2];
	cell = &p[1];
 
	each(i, -1, 2 * h + 1) cell[i][-1] = cell[i][w2 - 1] = V;
	each(j, 0, 2 * w) cell[-1][j] = cell[h2 - 1][j] = V;
	each(i, 0, h) each(j, 0, 2 * w) cell[2*i][j] |= E|W;
	each(i, 0, 2 * h) each(j, 0, w) cell[i][2*j] |= N|S;
	each(j, 0, 2 * w) cell[0][j] &= ~N, cell[2*h][j] &= ~S;
	each(i, 0, 2 * h) cell[i][0] &= ~W, cell[i][2*w] &= ~E;
 
	avail = w * h;
	walk(irand(2) * 2 + 1, irand(h) * 2 + 1);
 
	/* reset visited marker (it's also used by path finder) */
	//each(i, 0, 2 * h) each(j, 0, 2 * w) cell[i][j] &= ~V;
	//	solve(1, 1, 2 * w - 1, 2 * h - 1);
 
	//	show();
}

CityMaze::CityMaze(int wid, int hgt)
{
  w = wid;
  h = hgt;

  makeMaze();
}

