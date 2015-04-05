// "CameraSet" handles 4 different "Camera" objects

#include "CameraSet.h"

CameraSet::CameraSet(Car *bindedCar) {
	car = bindedCar; // Camera system follows a car

	// NORMAL views the car from behind
	cameras[NORMAL] = new Camera();
	cameras[NORMAL]->SetPosition(new Vector3D(car->GetPosition().x, car->GetPosition().y, car->GetPosition().z));
	cameras[NORMAL]->MoveBackward(4.5);
	cameras[NORMAL]->MoveUpward(5);
	cameras[NORMAL]->RotateX(-35 * 3.1415 / 180);

	// NOSE shows the headlights perspective
	cameras[NOSE] = new Camera();
	cameras[NOSE]->SetPosition(new Vector3D(car->GetPosition().x, car->GetPosition().y, car->GetPosition().z));
	cameras[NOSE]->MoveUpward(1);

	// TOP follows the car like a bird
	cameras[TOP] = new Camera();
	cameras[TOP]->SetPosition(new Vector3D(car->GetPosition().x, 15, car->GetPosition().z));
	cameras[TOP]->SetForwardVector(new Vector3D(0, -1, 0));
	cameras[TOP]->SetRightVector(new Vector3D(1, 0, 0));
	cameras[TOP]->SetUpVector(new Vector3D(0, 0, -1));

	// FREE goes anywhere
	cameras[FREE] = new Camera();
	cameras[FREE]->SetPosition(new Vector3D(-10, 2, 0));
	cameras[FREE]->SetForwardVector(new Vector3D(1, 0, 0));
	cameras[FREE]->SetRightVector(new Vector3D(0, 0, 1));
	cameras[FREE]->SetUpVector(new Vector3D(0, 1, 0));

	activeCamera = NORMAL;
}

CameraSet::~CameraSet() {
	delete [] &cameras;
}

void CameraSet::render() {
	cameras[activeCamera]->Render();
}

void CameraSet::update() {
	switch(activeCamera) {
		case NORMAL: {
			// We give the camera a small delay,
			float newRot = (car->GetRotation().y * 3.1415 / 180 + cameras[NORMAL]->rotation.y) / 8;
			cameras[NORMAL]->RotateX(35 * 3.1415 / 180);
			cameras[NORMAL]->SetPosition(new Vector3D(car->GetPosition().x, car->GetPosition().y, car->GetPosition().z));
			cameras[NORMAL]->RotateY(-newRot);
			cameras[NORMAL]->MoveBackward(4);
			cameras[NORMAL]->MoveUpward(5);
			cameras[NORMAL]->RotateX(-35 * 3.1415 / 180);
			cameras[NORMAL]->rotation.y -= newRot;

		} break;
		case TOP: {
			cameras[TOP]->SetPosition(new Vector3D(car->GetPosition().x, 15, car->GetPosition().z));
		} break;
		case NOSE: {
			cameras[NOSE]->SetPosition(new Vector3D(car->GetPosition().x, car->GetPosition().y, car->GetPosition().z));
			cameras[NOSE]->RotateY(-cameras[NOSE]->rotation.y);
			cameras[NOSE]->RotateY(-car->GetRotation().y * PI / 180);
			cameras[NOSE]->rotation.y = -car->GetRotation().y * PI / 180;
			cameras[NOSE]->MoveUpward(0.65);
			cameras[NOSE]->MoveForward(-0.4);
		} break;
	}
}

// We move the free camera.
void CameraSet::updateFreeCam(int key) {
	switch(key) {
		case 'w':
		case 'W':
			cameras[FREE]->MoveForward(0.5);
			break;
		case 's':
		case 'S':
			cameras[FREE]->MoveBackward(0.5);
			break;
		case 'a':
		case 'A':
			cameras[FREE]->MoveRight(0.5);
			break;
			break;
		case 'd':
		case 'D':
			cameras[FREE]->MoveLeft(0.5);
			break;
			break;
	}
}

void CameraSet::nextCamera() {
	if(++activeCamera > 3)
		activeCamera = 0;
}

bool CameraSet::isFreeCamActive() {
	return (activeCamera == FREE);
}

int CameraSet::getCameraType() {
	return activeCamera;
}

Camera* CameraSet::getFreeCam() {
	return cameras[FREE];
}

Vector3D CameraSet::getPosition() {
	return cameras[activeCamera]->GetPosition();
}
