#include "Camera.h"
#include "math.h"
#include <stdio.h>

Camera::Camera() {
	// Some default values
	Position = Vector3D(0.0, -1.0, 8);
	ForwardVector = Vector3D(0.0, 0, -1.0);
	RightVector = Vector3D(1.0, 0.0, 0.0);
	UpVector = Vector3D(0.0, 1.0, 0.0);
	rotation = Vector3D(0, 0, 0);
}

void Camera::RotateX(GLfloat Angle) {
	rotation.x += Angle; // We need to remember how much has the camera rotated off axis.

	Vector3D FwdNew, UpNew;

	FwdNew = ForwardVector * cos(Angle) + UpVector * sin(Angle);
	UpNew = UpVector * cos(Angle) - ForwardVector * sin(Angle);

	ForwardVector = FwdNew;
	UpVector = UpNew;
}

void Camera::RotateY(GLfloat Angle) {
	Vector3D FwdNew, RightNew;

	FwdNew = ForwardVector * cos(Angle) + RightVector * sin(Angle);
	RightNew = RightVector * cos(Angle) - ForwardVector * sin(Angle);

	ForwardVector = FwdNew;
	RightVector = RightNew;
}

void Camera::RotateZ(GLfloat Angle) {
	Vector3D UpNew, RightNew;

	UpNew = UpVector * cos(Angle) + RightVector * sin(Angle);
	RightNew = RightVector * cos(Angle) - UpVector * sin(Angle);

	UpVector = UpNew;
	RightVector = RightNew;
}

void Camera::RotateXCenter(GLfloat Angle, float distance) {
	MoveForward(distance);
	RotateX(Angle);
	MoveBackward(distance);
}

void Camera::RotateYCenter(GLfloat Angle, float distance) {
	MoveForward(distance);
	RotateY(Angle);
	MoveBackward(distance);
}

void Camera::RotateZCenter(GLfloat Angle, float distance) {
	MoveForward(distance);
	RotateZ(Angle);
	MoveBackward(distance);
}

void Camera::Render(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector3D ViewPoint = Position + ForwardVector;

	gluLookAt(Position.x, Position.y, Position.z, ViewPoint.x, ViewPoint.y, ViewPoint.z, UpVector.x, UpVector.y,
				UpVector.z);
}

void Camera::MoveForward(GLfloat Distance) {
	Position = Position + (ForwardVector * Distance);
}

void Camera::MoveBackward(GLfloat Distance) {
	Position = Position + (ForwardVector * -Distance);
}

void Camera::MoveRight(GLfloat Distance) {
	Position = Position + (RightVector * -Distance);
}

void Camera::MoveLeft(GLfloat Distance) {
	Position = Position + (RightVector * Distance);
}

void Camera::MoveUpward(GLfloat Distance) {
	Position = Position + (UpVector * Distance);
}

void Camera::MoveDownward(GLfloat Distance) {
	Position = Position + (UpVector * -Distance);
}

void Camera::SetPosition(Vector3D *value) {
	Position = *value;
}

void Camera::SetForwardVector(Vector3D *value) {
	ForwardVector = *value;
}

void Camera::SetRightVector(Vector3D *value) {
	RightVector = *value;
}

void Camera::SetUpVector(Vector3D *value) {
	UpVector = *value;
}

Vector3D Camera::GetPosition() {
	return Position;
}
