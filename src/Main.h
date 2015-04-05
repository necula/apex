#ifndef MAIN_H_
#define MAIN_H_

#include "Car.h"
#include "CameraSet.h"
#include "World.h"
#include "Vector3D.h"
#include "Vector4D.h"


#define ESC	27
#define PI 3.14159265
#define TIMERMSECS 20

float *Vector3D::arr;
float *Vector4D::arr;

// Camera control

#define NOSECAMERA 2
#define CAMSPEED 1.5 * PI / 180
int oldX, oldY;

// Car control
int steering = 0;
int leftPressed = 0, rightPressed = 0, upPressed = 0, downPressed = 0; // Keyboard presses

// Lap timing
float lapStartTime = 0;
float currentLapTime = 0, bestLapTime = 0, lastLapTime = 0;
bool onStart = false;
int lapCount = 0;
int startTime, prevTime;

// Fps counter
int fps = 0;
int fpsTimer = 0;

void drawText(GLfloat x, GLfloat y, char *format, ...);

// Distance between 2 points
static double distTwoPoints(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}


#endif /* MAIN_H_ */
