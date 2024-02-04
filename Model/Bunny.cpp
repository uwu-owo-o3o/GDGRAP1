#include "Bunny.hpp"

using namespace models;

Bunny::Bunny() {
	this->fTranslate_X = 0.0f;
	this->fTranslate_Y = 0.0f;
	this->fTranslate_Z = -2.0f;

	this->fScale_X = 5.0f;
	this->fScale_Y = 5.0f;
	this->fScale_Z = 1.0f;

	this->fTheta = 45.0f;
	this->fAxis_X = 0.0f;
	this->fAxis_Y = 1.0f;
	this->fAxis_Z = 0.0f;

	this->transformation_matrix = glm::mat4(1.0f);

}

void Bunny::calculateTransformMatrix() {

	glm::mat4 identity_matrix = glm::mat4(1.0f);

	// start with translation matrix //
	this->transformation_matrix = glm::translate(identity_matrix, glm::vec3(this->fTranslate_X, this->fTranslate_Y, this->fTranslate_Z));

	// multiply matrix with scale matrix //
	this->transformation_matrix = glm::scale(this->transformation_matrix, glm::vec3(this->fScale_X, this->fScale_Y, this->fScale_Z));

	// multiply with rotate matrix //
	this->transformation_matrix = glm::rotate(this->transformation_matrix, glm::radians(this->fTheta), glm::normalize(glm::vec3(this->fAxis_X, this->fAxis_Y, this->fAxis_Z)));
}

void Bunny::updateTransformation(CalledKey eKey) {
	switch (eKey) {
	case GLFW_KEY_W:
		this->fTranslate_Y -= 10.f;
		break;
	case GLFW_KEY_A:
		this->fTranslate_X += 0.5f;
		break;
	case GLFW_KEY_S:
		this->fTranslate_Y -= 0.5f;
		break;
	case GLFW_KEY_D:
		this->fTranslate_X += 0.5f;
		break;
	case GLFW_KEY_LEFT:
		this->fAxis_X = 0.f;
		this->fAxis_Y = 1.f;
		this->fAxis_Z = 0.f;
		this->fTheta -= 20.0f;
		break;
	case GLFW_KEY_RIGHT:
		this->fAxis_X = 0.f;
		this->fAxis_Y = 1.f;
		this->fAxis_Z = 0.f;
		this->fTheta += 20.0f;
		break;
	case GLFW_KEY_UP:
		this->fAxis_X = 1.f;
		this->fAxis_Y = 0.f;
		this->fAxis_Z = 0.f;
		this->fTheta -= 10.0f;
		break;
	case GLFW_KEY_DOWN:
		this->fAxis_X = 1.f;
		this->fAxis_Y = 0.f;
		this->fAxis_Z = 0.f;
		this->fTheta += 10.0f;
		break;
	case GLFW_KEY_E:
		this->fScale_X += 3.0f;
		this->fScale_Y += 3.0f;
		break;
	case GLFW_KEY_Q:
		this->fScale_X -= 3.0f;
		this->fScale_Y -= 3.0f;
		break;
	case GLFW_KEY_Z:
		//keyVar = CalledKey::ZOOMIN_Z;
		break;
	case GLFW_KEY_X:
		//keyVar = CalledKey::ZOOMOUT_X;
		break;
	}
}

void Bunny::setTranslateVar(char cVar, float fValue) {
	switch (cVar) {
		case 'X':
			this->fTranslate_X = fValue;
			break;
		case 'Y':
			this->fTranslate_Y = fValue;
			break;
		case 'Z':
			this->fTranslate_Z = fValue;
			break;
	}
}

float Bunny::getTranslateVar(char cVar) {
	switch (cVar) {
		case 'X':
			return this->fTranslate_X;
		case 'Y':
			return this->fTranslate_Y;
		case 'Z':
			return this->fTranslate_Z;
	}

	return 0.0f;
}

void Bunny::setScaleVar(char cVar, float fValue) {
	switch (cVar) {
	case 'X':
		this->fScale_X = fValue;
		break;
	case 'Y':
		this->fScale_Y = fValue;
		break;
	case 'Z':
		this->fScale_Z = fValue;
		break;
	}
}

float Bunny::getScaleVar(char cVar) {
	switch (cVar) {
	case 'X':
		return this->fScale_X;
	case 'Y':
		return this->fScale_Y;
	case 'Z':
		return this->fScale_Z;
	}

	return 0.0f;
}

void Bunny::setRotateVar(char cVar, float fValue) {
	switch (cVar) {
	case 'X':
		this->fAxis_X = fValue;
		break;
	case 'Y':
		this->fAxis_Y = fValue;
		break;
	case 'Z':
		this->fAxis_Z = fValue;
		break;
	case 'T':
		this->fTheta = fValue;
		break;
	}
}

float Bunny::getRotateVar(char cVar) {
	switch (cVar) {
	case 'X':
		return this->fAxis_X;
	case 'Y':
		return this->fAxis_Y;
	case 'Z':
		return this->fAxis_Z;
	case 'T':
		return this->fTheta;
	}

	return 0.0f;
}

glm::mat4* Bunny::getMatrix() {
	return &this->transformation_matrix;
}