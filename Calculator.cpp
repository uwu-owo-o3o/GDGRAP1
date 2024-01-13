#include "Calculator.hpp"

using namespace calculator;

Calculator::Calculator() {
	this->calculatePoints();
};

// converts the polar point to cartesian //
float Calculator::getCartesianX(float magnitude, float angle, int side_num) {
	float tetha = (angle * 3.14) / 180; // converts angle from degree to radian //

	float x = magnitude * cos(tetha * side_num); // multiply tetha with the side number until it reaches 360 //
	
	return x;
}

// converts the polar point to cartesian //
float Calculator::getCartesianY(float magnitude, float angle, int side_num) {
	float tetha = (angle * 3.14) / 180; // converts angle from degree to radian //

	float y = magnitude * sin(tetha * side_num); // multiply tetha with the side number until it reaches 360 //
	
	return y;
}

// used to double check if distance difference between points is big //
void Calculator::checkDistance(float point1X, float point1Y, float point2X, float point2Y, float point3X, float point3Y, float point4X, float point4Y) {

	float distance1 = sqrt( (pow(point2X - point1X, 2) + pow(point2Y - point1Y, 2)) );
	float distance2 = sqrt( (pow(point4X - point3X, 2) + pow(point4Y - point3Y, 2)) );

	if (distance1 == distance2) {
		std::cout << "Distance is equal!" << std::endl;
	}
	else {
		std::cout << "Distance is not equal" << std::endl;
		float difference = distance1 - distance2;
		std::cout << "difference between point distances is: " << difference << std::endl;
	}

}

// used to calculate the points for the pentagon //
void Calculator::calculatePoints() {
	int index;
	int side_num = 1;

	for (index = 0; index < this->x_points.size(); index++) {
		
		// 0.5 is used for radius, 72 degrees for angle -> derived from 360 divided by 5 //
		float pointx = this->getCartesianX(0.5, 72, side_num); 
		float pointy = this->getCartesianY(0.5, 72, side_num); 
		
		// push value into array //
		this->x_points[index] = pointx;
		this->y_points[index] = pointy;

		side_num = side_num + 1;
	}

	side_num = 0;
}

std::array<float, 5>* Calculator::getXPointsArray() {
	return &this->x_points;
}

std::array<float, 5>* Calculator::getYPointsArray() {
	return &this->y_points;
}