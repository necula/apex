#ifndef CAMERASET_H_
#define CAMERASET_H_

#include "Camera.h"
#include "Object3D.h"
#include "Car.h"

// Camera types
#define TOP 0
#define NOSE 2
#define FREE 3
#define NORMAL 1

class CameraSet {

private:
	int activeCamera;
	Camera *cameras[4];
	Car *car;

public:
	CameraSet(Car *);
	virtual ~CameraSet();
	void nextCamera();
	bool isFreeCamActive();
	int getCameraType();
	void update();
	void updateFreeCam(int);
	void render();
	Camera* getFreeCam();
	Vector3D getPosition();

};

#endif /* CAMERASET_H_ */
