#include "ATime.h"
#include "Location.h"

Location::Location() {
	x = 0;
	y = 0;
	z = 0;
}

Location::Location(double a, double b, ATime at) {
	x = a;
	y = b;
	z = 0;
	t = at;
}

Location::Location(double a, double b, double c, ATime at) {
	x = a;
	y = b;
	z = c;
	t = at;
}

void Location::getXY(double& x_pos, double& y_pos) const {
	x_pos = x;
	y_pos = y;
}

void Location::getXYZ(double& x_pos, double& y_pos, double& z_pos) const {
	x_pos = x;
	y_pos = y;
	z_pos = z;
}

void Location::print() {
	std::cout.setf(std::ios::fixed);
	std::cout.setf(std::ios::showpoint);
	std::cout.precision(2);
	std::cout << "(" << x << ", " << y << ", " << z << ")" << " ATime = " << t << std::endl;
}
