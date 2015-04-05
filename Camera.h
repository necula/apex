#ifndef CAMERA_H
#define CAMERA_H

#include <glut.h>
#include "Vector3D.h"
#include "Object3D.h"
#include "Car.h"

class Camera
{
	
private:
	Vector3D ForwardVector;
	Vector3D RightVector;	
	Vector3D UpVector;
	Vector3D Position;

public:

	Vector3D rotation; // !! Write Setter/Getter
	Camera();				
	void Render ( void );	

	// Axis relative rotation
	void RotateX ( GLfloat Angle );
	void RotateY ( GLfloat Angle );
	void RotateZ ( GLfloat Angle );

	// Point relative rotation
	void RotateXCenter ( GLfloat Angle, float distance );
	void RotateYCenter ( GLfloat Angle, float distance );
	void RotateZCenter ( GLfloat Angle, float distance );

	// Basic camera movement
	void MoveForward ( GLfloat Distance );
	void MoveBackward ( GLfloat Distance );
	void MoveUpward ( GLfloat Distance );
	void MoveDownward ( GLfloat Distance );
	void MoveRight ( GLfloat Distance );
	void MoveLeft ( GLfloat Distance );


	void SetPosition(Vector3D *value);
	void SetForwardVector(Vector3D *value);
	void SetRightVector(Vector3D *value);
	void SetUpVector(Vector3D *value);

	Vector3D GetPosition();
};

#endif
