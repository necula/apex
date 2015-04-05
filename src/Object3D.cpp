#include "Object3D.h"


Object3D::Object3D() {
	defaultSettings();

}

Object3D::Object3D(Vector3D *_translation) {
	defaultSettings();
	this->SetPosition(_translation);
}

Object3D::Object3D(Vector3D *_translation, Vector3D *_rotation, Vector3D *_scale) {
	defaultSettings();
	this->SetPosition(_translation);
	this->SetRotation(_rotation);
	this->SetScale(_scale);
}

void Object3D::defaultSettings() {
	translation = Vector3D(0.0, 0.0, 0.0);
	rotation = Vector3D(0.0, 0.0, 0.0);
	scale = Vector3D(1.0, 1.0, 1.0);
	levelOfDetail = 12;
}


void Object3D::Draw() {

	glPushMatrix();

	glTranslatef(translation.x, translation.y, translation.z);

	glColor3f(color.x, color.y, color.z);
	glRotatef(rotation.x, 1.0, 0.0, 0.0);
	glRotatef(rotation.y, 0.0, 1.0, 0.0);
	glRotatef(rotation.z, 0.0, 0.0, 1.0);

	glScalef(scale.x, scale.y, scale.z);

	customDraw();

	glPopMatrix();
}

void Object3D::customDraw() {

}

void Object3D::SetColor(Vector3D *_color) {
	color = *_color;
}

void Object3D::SetPosition(Vector3D *_translation) {
	translation = *_translation;
}

void Object3D::SetRotation(Vector3D *_rotation) {
	rotation = *_rotation;
}

void Object3D::SetScale(Vector3D *_scale) {
	scale = *_scale;
}

void Object3D::SetLevelOfDetail(float _levelOfDetail) {
	if(_levelOfDetail > 0 && _levelOfDetail < MAXIMUM_LOD)
		levelOfDetail = _levelOfDetail;
}

void Object3D::Steer(float degrees) {
	rotation.y += degrees;
}

Vector3D Object3D::GetPosition() {
	return translation;
}

Vector3D Object3D::GetRotation() {
	return rotation;
}

Vector3D Object3D::GetScale() {
	return scale;
}

float Object3D::GetLevelOfDetail() {
	return levelOfDetail;
}

