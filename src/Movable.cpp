//Troy Moench
#include <iostream>
#include <string>
#include <cmath>
#include "ATime.h"
#include "Location.h"
#include "Movable.h"
typedef std::list <Location>::iterator ListIterator;

Movable::Movable() {
	isdeployed = false;
	wasdeployed = false;
	max_speed = 0;
	speed = 0;
	heading = 0;
}

Movable::Movable(std::string nm, std::string s, double mspd) {
	name = nm;
	ID = s;
	isdeployed = false;
	wasdeployed = false;
	max_speed = mspd;
	speed = 0;
	heading = 0;
}

bool Movable::deploy(double x, double y, double head, double spd, ATime t) { //deploys ship
	isdeployed = true;
	wasdeployed = true;
	loc = Location(x, y, t);
	hl.push_back(loc);
	heading = head;
	speed = spd;
	at = t;
	return true;
}

void Movable::update(ATime a) {
	if (at == a) {
		return;
	}
	double distance; //in miles
	double time; //time difference in hours
	double x, y, z, dx, dy;
	const double PI = 3.141592654;
	time = (a - at) / (60.0 * 60.0);
	distance = speed*time;
	dx = distance*sin(heading*PI / 180);
	dy = distance*cos(heading*PI / 180);
	loc.getXYZ(x, y, z);
	loc = Location(x + dx, y + dy, z, a);
	hl.push_back(loc);
	at = a;
	//this->print();
	//std::cout << "Position updated at " << at << std::endl;
}

Location Movable::getlocation() {
	return loc;
}

void Movable::print() {
	std::cout << name << " " << ID << " " << max_speed << std::endl;
}

void Movable::printHL() {
	ListIterator li;
	for (li = hl.begin(); li != hl.end(); li++) {
		li->print();
	}
}

HistoryList* Movable::getHistory() {
	return &hl;
}

Ship::Ship() {}

Ship::Ship(std::string nm, std::string s, double mspd) : Movable(nm, s, mspd) {}

bool Ship::change(double head, double spd, double alt, ATime t) { //changes ship's course
	this->updatePosition(t);
	if (head != -1) {
		heading = head;
	}
	if (spd != -1) {
		speed = spd;
	}
	return true;
}

void Ship::updatePosition(ATime a) {
	this->update(a);
}

Cruiser::Cruiser() {
	max_missiles = 0;
}

Cruiser::Cruiser(std::string nm, std::string s, int m, double mspd) : Ship(nm, s, mspd) {
	max_missiles = m;
}

Carrier::Carrier(){
	max_aircraft = 0;
}

Carrier::Carrier(std::string nm, std::string s, int m, double mspd) : Ship(nm, s, mspd) {
	max_aircraft = m;
}

Fighter::Fighter() {
	max_ceiling = 0;
	max_bombs = 0;
	max_speed = 0;
}

Fighter::Fighter(std::string nm, std::string s, Movable* id, double mc, double mspd, int mb) : Movable(nm, s, mspd) {
	max_ceiling = mc;
	max_speed = mspd;
	max_bombs = mb;
	ShipID = id;
}

bool Fighter::deploy(double z, double head, double spd, ATime t) { //deploys aircraft
	double sx, sy;
	Location ship;
	ship = ShipID->getlocation();
	ship.getXY(sx, sy);
	isdeployed = true;
	wasdeployed = true;
	loc = Location(sx, sy, z, t); //set x and y coordinates equal to the corresponding carrier's x and y coordinates?
	hl.push_back(loc);
	heading = head;
	speed = spd;
	altitude = z;
	at = t;
	return true;
}

bool Fighter::change(double alt, double head, double spd, ATime t) {
	this->updatePosition(t);
	if (spd != -1) {
		speed = spd;
	}
	if (alt != -1) {
		loc.setZ(alt);
		altitude = alt;
	}
	if (!islanding && head != -1) {
			heading = head;
	}
	return true;
}

bool Fighter::land(Movable* Id, ATime t) {
	ShipID = Id;
	this->updatePosition(t);
	islanding = true;
	return true;
}

void Fighter::updatePosition(ATime a) {
	if (isdeployed) {
		this->update(a);
		loc.setZ(altitude);
		loc.print();
		if (islanding) {
			if (this->withinrange()) {
				isdeployed = false;
				islanding = false;
				std::cout << "Landed safely." << std::endl;
			}
			else {
				this->gotocarrier();
			}
		}
	}
}

bool Fighter::withinrange() {
	double dx, dy, fx, fy, sx, sy, distance;
	Location ship, fighter;
	ship = ShipID->getlocation();
	fighter = this->getlocation();
	ship.getXY(sx, sy);
	fighter.getXY(fx, fy);
	dx = sx - fx;
	dy = sy - fy;
	distance = sqrt(pow(dx, 2) + pow(dy, 2));
	// std::cout << "Distance = " << distance << std::endl;
	if (distance <= speed / 60) {
		return true;
	}
	else return false;
}

void Fighter::gotocarrier() {
	double theta, dx, dy, fx, fy, sx, sy, distance, da, t;
	const double PI = 3.141592654;
	Location ship, fighter;
	ship = ShipID->getlocation();
	fighter = this->getlocation();
	ship.getXY(sx, sy);
	fighter.getXY(fx, fy);
	dx = sx - fx;
	dy = sy - fy;
	distance = sqrt(pow(dx, 2) + pow(dy, 2));
	// decrease altitude proportionally to time to land
	t = distance*60 / speed;
	da = altitude / t;
	altitude -= da;

	if (t < 10) {
		// decrease speed by 10%
		// arbitrarily chosen
		speed *= .90;
	}
	// std::cout << "da = " << da << std::endl;
	std::cout << "speed = " << speed << std::endl;

	if (dy == 0) {
		if (sx - fx > 0) {
			heading = 90;
		}
		if (sx - fx < 0) {
			heading = 270;
		}
	}
	else {
		theta = std::abs(atan(dx / dy)*(180 / PI));
		if (dx >= 0 && dy > 0) {
			heading = theta;
		}
		else if (dx >= 0 && dy < 0) {
			heading = 180 - theta;
		}
		else if (dx <= 0 && dy < 0) {
			heading = 180 + theta;
		}
		else if (dx <= 0 && dy > 0) {
			heading = 360 - theta;
		}
		else {
			std::cout << "Heading could not be found." << std::endl;
		}
	}
	// std::cout << "Heading = " << heading << std::endl;
	// std::cout << "Altitude = " << altitude << std::endl;
}
