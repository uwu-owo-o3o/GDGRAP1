#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include "cmath"
#include "iostream"
#include "array"

namespace calculator {
	class Calculator {
		private:
			std::array<float, 5> x_points;
			std::array<float, 5> y_points;
		public:
			Calculator();
		public:
			float getCartesianX(float magnitude, float angle, int side_num);
			float getCartesianY(float magnitude, float angle, int side_num);
			void  checkDistance(float point1X, float point1Y, float point2X, float point2Y, float point3X, float point3Y, float point4X, float point4Y);
			void  calculatePoints();
		public:
			std::array<float, 5>* getXPointsArray();
			std::array<float, 5>* getYPointsArray();
		
	};
}

#endif