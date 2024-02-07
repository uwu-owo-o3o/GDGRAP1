#include "Bunny.hpp"

using namespace models;

Bunny::Bunny() {
	this->fTranslate_X = 0.0f;
	this->fTranslate_Y = 0.0f;
	this->fTranslate_Z = -2.0f;

	this->fScale_X = 3.0f;
	this->fScale_Y = 3.0f;
	this->fScale_Z = 1.0f;

	this->fTheta = 90.0f;
	this->fAxis_X = 0.0f;
	this->fAxis_Y = 1.0f;
	this->fAxis_Z = 0.0f;

	this->fPerspectiveTheta = 60.0f;
	this->transformation_matrix = glm::mat4(1.0f);

}

// calculates for the final transformation matrix to be used in the program //
void Bunny::calculateTransformMatrix() {

	glm::mat4 identity_matrix = glm::mat4(1.0f);

	this->transformation_matrix = glm::translate(identity_matrix, glm::vec3(this->fTranslate_X, this->fTranslate_Y, this->fTranslate_Z));

	this->transformation_matrix = glm::scale(this->transformation_matrix, glm::vec3(this->fScale_X, this->fScale_Y, this->fScale_Z));

	this->transformation_matrix = glm::rotate(this->transformation_matrix, glm::radians(this->fTheta), glm::normalize(glm::vec3(this->fAxis_X, this->fAxis_Y, this->fAxis_Z)));
}

//receives the passed flag to update the values used in a transform matrix//
void Bunny::updateTransformation(CalledKey* eKey) {
	
	switch (*eKey) {
		case CalledKey::MOVE_W:
				this->fTranslate_Y += 0.1f;
				break;
			case CalledKey::MOVE_A:
				this->fTranslate_X -= 0.1f;
				break;
			case CalledKey::MOVE_S:
				this->fTranslate_Y -= 0.1f;
				break;
			case CalledKey::MOVE_D:
				this->fTranslate_X += 0.1f;
				break;
			case CalledKey::ROTATE_LEFT:
				this->fAxis_X = 0.f;
				this->fAxis_Y = 1.f;
				this->fAxis_Z = 0.f;
				this->fTheta -= 20.0f;
				break;
			case CalledKey::ROTATE_RIGHT:
				this->fAxis_X = 0.f;
				this->fAxis_Y = 1.f;
				this->fAxis_Z = 0.f;
				this->fTheta += 20.0f;
				break;
			case CalledKey::ROTATE_UP:
				this->fAxis_X = 1.f;
				this->fAxis_Y = 0.f;
				this->fAxis_Z = 0.f;
				this->fTheta -= 10.0f;
				break;
			case CalledKey::ROTATE_DOWN:
				this->fAxis_X = 1.f;
				this->fAxis_Y = 0.f;
				this->fAxis_Z = 0.f;
				this->fTheta += 10.0f;
				break;
			case CalledKey::SCALEUP_E:
				this->fScale_X += 0.1f;
				this->fScale_Y += 0.1f;
				break;
			case CalledKey::SCALEDOWN_Q:
				this->fScale_X -= 0.1f;
				this->fScale_Y -= 0.1f;
				break;
			case CalledKey::ZOOMIN_Z:
				this->fTheta -= 0.5f;
				this->fPerspectiveTheta -= 0.5f;
				break;
			case CalledKey::ZOOMOUT_X:
				this->fTheta += 0.5f;
				this->fPerspectiveTheta += 0.5f;
				break;
		}

	*eKey = CalledKey::NONE;
}

// GETTERS AND SETTERS //
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

float Bunny::getPerspectiveTheta() {
	return this->fPerspectiveTheta;
}

glm::mat4* Bunny::getMatrix() {
	return &this->transformation_matrix;
}