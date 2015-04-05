#ifndef MAP_H_
#define MAP_H_

#include <glut.h>

#include "Vector3D.h"
#include "Vector4D.h"
#include "Object3D.h"
#include "Car.h"
#include "stdio.h"
#include "stdlib.h"

class Map
{
private:
    Vector3D translation;
    Vector3D rotation;
    Vector3D scale;
    Vector3D color;
    int **levelData;
    int levelSize;
    GLuint level;
    int lastHitX, lastHitY;
    FILE* dataFile;

public:
    Map();
    virtual ~Map();

    void readLevelData();
    void buildLevel();
    bool checkCollision(Car *car);
    void Draw();

};

#endif /* MAP_H_ */
