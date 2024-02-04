#ifndef BUNNY_HPP
#define BUNNY_HPP

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace models {

	enum CalledKey {
		NONE = 0,
		MOVE_W,
		MOVE_A,
		MOVE_S,
		MOVE_D,
		ROTATE_LEFT,
		ROTATE_RIGHT,
		ROTATE_UP,
		ROTATE_DOWN,
		SCALEUP_E,
		SCALEDOWN_Q,
		ZOOMIN_Z,
		ZOOMOUT_X
	};

	class Bunny {
		private:
			float fTranslate_X;
			float fTranslate_Y;
			float fTranslate_Z;

			float fScale_X;
			float fScale_Y;
			float fScale_Z;

			float fTheta;
			float fAxis_X;
			float fAxis_Y;
			float fAxis_Z;
			
			glm::mat4 transformation_matrix;
		public:
			Bunny();
		public:
			void calculateTransformMatrix();
			void updateTransformation(CalledKey eKey);

		public:
			void setTranslateVar(char cVar, float fValue);
			float getTranslateVar(char cVar);
			void setScaleVar(char cVar, float fValue);
			float getScaleVar(char cVar);
			void setRotateVar(char cVar, float fValue);
			float getRotateVar(char cVar);
			glm::mat4* getMatrix();
	};
}
















#endif