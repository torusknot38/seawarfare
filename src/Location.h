#ifndef LOCATION_H
#define LOCATION_H

class Location {
	friend double getDistance(const Location&, const Location&);
public:
	Location();
	Location(double, double, ATime);
	Location(double, double, double, ATime);
	void getXY(double& x_pos, double& y_pos) const;
	void getXYZ(double& x_pos, double& y_pos, double& z_pos) const;
	void setZ(double z_pos) { z = z_pos; }
	void print();
private:
	double x;
	double y;
	double z;
	ATime t;
};

#endif
