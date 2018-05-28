//Troy Moench
#include "ATime.h"
#include "Movable.h"
#include "Order.h"

Order::Order() {}

Order::Order(ATime a, std::string s) {
	ID = s;
	extime = a;
}

bool Order::operator==(const Order& o) const {
	return (extime == o.extime);
}
bool Order::operator<(const Order& o) const {
	return (extime < o.extime);
}
bool Order::operator>(const Order& o) const {
	return (extime > o.extime);
}
bool Order::operator!=(const Order& o) const {
	return (extime != o.extime);
}

void Order::print() {
	std::cout << ID << " " << extime << std::endl;
}

DeployShip::DeployShip() {
	start_x = 0;
	start_y = 0;
	heading = -1;
	speed = -1;
}

DeployShip::DeployShip(ATime a, std::string s, double x, double y, double head, double spd) : Order(a, s) {
	start_x = x;
	start_y = y;
	heading = head;
	speed = spd;
}

bool DeployShip::Execute(Movable* mp, ATime t) {
	return (mp->deploy(start_x, start_y, heading, speed, t));
}

DeployAircraft::DeployAircraft() {
	heading = -1;
	speed = -1;
	altitude = -1;
}

DeployAircraft::DeployAircraft(ATime a, std::string s, double head, double spd, double alt) : Order(a, s) {
	heading = head;
	speed = spd;
	altitude = alt;
}

bool DeployAircraft::Execute(Movable* mp, ATime t) {
	Fighter* f = dynamic_cast <Fighter*> (mp);
	if (f) {
		f->deploy(altitude, heading, speed, t);
		return true;
	}
	else return false;
}

ChangeShip::ChangeShip() {
	heading = -1;
	speed = -1;
}

ChangeShip::ChangeShip(ATime a, std::string s, double head, double spd) : Order(a, s) {
	heading = head;
	speed = spd;
}

bool ChangeShip::Execute(Movable* mp, ATime t) {
	return (mp->change(heading, speed, 0, t));
}

ChangeAircraft::ChangeAircraft() {
	heading = -1;
	speed = -1;
	altitude = -1;
}

ChangeAircraft::ChangeAircraft(ATime a, std::string s, double head, double spd, double alt) : Order(a, s){
	heading = head;
	speed = spd;
	altitude = alt;
}

bool ChangeAircraft::Execute(Movable* mp, ATime t) {
	return (mp->change(altitude, heading, speed, t));
}

LandAircraft::LandAircraft() {
	shipID = NULL;
}

LandAircraft::LandAircraft(ATime a, Movable* b, std::string s) : Order(a, s)  {
	shipID = b;
}

bool LandAircraft::Execute(Movable* mp, ATime t) {
	Fighter* f = dynamic_cast <Fighter*> (mp);
	if (f) {
		f->land(shipID, t);
		return true;
	}
	else return false;
}
