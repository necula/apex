#ifndef CAR_H
#define CAR_H

#include <glut.h>
#include "Vector3D.h"
#include "Vector4D.h"
#include "Object3D.h"

#define PI 3.14159265
#define DRAG 0.98
#define FORWARDACC 1
#define BRKPOWER 1.2
#define REVERSEACC 0.6
#define MAXVEL 50
#define MAXNVEL -5
#define STEERANGLE 4


class Car : public Object3D
{

private:
    double velocity;
    float step; // Position update step

public:
    Car();
    Vector3D lastSafePos;
    Vector3D direction;
    GLuint carList;

    void Draw(int drawCar);
    void update(int st_input, int acc, int brk);
    void hasCollided();
	void slow();
	void buildCar();
};

#endif
