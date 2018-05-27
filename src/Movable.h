#ifndef MOVABLE_H
#define MOVABLE_H

#include <list>
#include "ATime.h"
#include "Location.h"
typedef std::list <Location> HistoryList;

class Movable {
public:
	Movable();
	Movable(std::string, std::string, double);
	virtual void updatePosition(ATime) = 0;   // make this pure virtual
	bool isDeployed() const { return isdeployed; }
	bool wasDeployed() const { return wasdeployed; }
	HistoryList* getHistory();
	Location getlocation(); //makes location available (for aircraft to land)
	void print();
	void printHL();
	bool deploy(double, double, double, double, ATime);
	virtual bool change(double, double, double, ATime) = 0;
protected:
	ATime at;
	Location loc;
	bool isdeployed;
	bool wasdeployed;
	double heading;
	double speed;
	double max_speed;
	std::string name;
	std::string ID;
	HistoryList hl;
	void update(ATime);
};

class Ship : public Movable {
public:
	Ship();
	Ship(std::string, std::string, double);
	void updatePosition(ATime);
	bool change(double, double, double, ATime);
};

class Cruiser : public Ship {
public:
	Cruiser();
	Cruiser(std::string, std::string, int, double);
private:
	int max_missiles;
};

class Carrier : public Ship {
public:
	Carrier();
	Carrier(std::string, std::string, int, double);
private:
	int max_aircraft;
};

class Fighter : public Movable {
public:
	Fighter();
	Fighter(std::string, std::string, Movable*, double, double, int); //name, ID, ShipID, max_ceiling, max_speed, max_bombs
	bool land(Movable*, ATime);
	void updatePosition(ATime);
	bool change(double, double, double, ATime);
	bool deploy(double, double, double, ATime);
private:
	void gotocarrier(); //sets heading of fighter to a ship's location using trig
	bool withinrange(); //checks if the fighter is within range of a ship's location while landing
	bool islanding;
	Movable* ShipID;
	double max_ceiling;
	double altitude;
	int max_bombs;
};

#endif