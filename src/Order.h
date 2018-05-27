//Troy Moench
#ifndef ORDER_H
#define ORDER_H

#include <string>
#include "Movable.h"

class Order {
public:
	Order();
	Order(ATime, std::string);
	bool operator==(const Order& o) const;
	bool operator<(const Order& o) const;
	bool operator>(const Order& o) const;
	bool operator!=(const Order& o) const;
	ATime exectime() const { return extime; }
	std::string get_id() const { return ID; }
	virtual bool Execute(Movable*, ATime) = 0;  // make pure virtual
	void print();
protected:
	std::string Name;
	std::string ID;
	ATime extime;
};

class DeployShip : public Order {
public:
	DeployShip();
	DeployShip(ATime, std::string, double, double, double, double);
	bool Execute(Movable*, ATime);
private:
	double start_x;
	double start_y;
	double heading;
	double speed;
};

class DeployAircraft : public Order {
public:
	DeployAircraft();
	DeployAircraft(ATime, std::string, double, double, double);
	bool Execute(Movable*, ATime);
private:
	double heading;
	double speed;
	double altitude;
};

class ChangeShip : public Order {
public:
	ChangeShip();
	ChangeShip(ATime, std::string, double, double);
	bool Execute(Movable*, ATime);
private:
	double heading;
	double speed;

};

class ChangeAircraft : public Order {
public:
	ChangeAircraft();
	ChangeAircraft(ATime, std::string, double, double, double);
	bool Execute(Movable*, ATime);
private:
	double heading;
	double speed;
	double altitude;
};

class LandAircraft : public Order {
public:
	LandAircraft();
	LandAircraft(ATime, Movable*, std::string);
	bool Execute(Movable*, ATime);
private:
	Movable* shipID;
};
#endif