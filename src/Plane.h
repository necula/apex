#ifndef PLANE_H
#define PLANE_H

#include <glut.h>
#include "Object3D.h"


class Plane : public Object3D
{

private:
	float size;
	GLuint planeList;

public:
	Plane(float _size);

protected:
	void customDraw();
};

#endif
