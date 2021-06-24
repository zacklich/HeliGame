
# so much magic from random internet searches here.

# Put object files in a subdirectory.
OBJ = obj
SRC = src
INC = include

CPPSRCS = HeliGame.cpp GenObject.cpp Helicopter.cpp Mountains.cpp Helipad.cpp \
	STL_Object.cpp Scene.cpp \
	Cylinder.cpp TrapezoidBox.cpp TexturedRoof.cpp CityMaze.cpp HeliRotors.cpp \
	FlatArea.cpp TexturedBox.cpp SkyBox.cpp OfficeBuilding.cpp Courthouse.cpp \
	Dome.cpp OracleCampus.cpp Mosque.cpp CubeTower.cpp TextureMap.cpp 

CSRCS = loadtexbmp.c fatal.c errcheck.c

OBJS = $(patsubst %.cpp,$(OBJ)/%.o,$(CPPSRCS)) $(patsubst %.c,$(OBJ)/%.o,$(CSRCS))

CC = gcc
CFLAGS = -I$(INC)
CPPFLAGS = 

all : $(OBJ) HeliGame parse_stl
	echo done

$(OBJ) :
	mkdir $(OBJ)

$(OBJ)/%.o : $(SRC)/%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

parse_stl : $(SRC)/parse_stl.c
	gcc -Wno-address-of-packed-member -o parse_stl $(SRC)/parse_stl.c -lm

HeliGame : $(OBJS)
	gcc -o HeliGame $(OBJS) -lm -lglut -lGLU -lGL -lstdc++

$(OBJS) : $(INC)/GenObject.h $(INC)/CSCIx229.h $(INC)/Scene.h $(INC)/CityMaze.h $(INC)/TextureMap.h



clean :
	rm -f HeliGame *.o parse_stl *~ $(OBJ)/*


