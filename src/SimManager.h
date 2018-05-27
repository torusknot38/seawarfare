#ifndef SIMMANAGER_H
#define SIMMANAGER_H

#include <sstream>
#include <iostream>
#include <map>
#include <deque>
#include <fstream>
#include <algorithm>
#include "Order.h"
typedef std::map<std::string, Movable*> NavyMap;
typedef std::deque<Order*> OrderQueue;

class SimulationMgr {
public:
	bool simInit(std::string orderFile);
	void simDoUpdate(ATime);
	ATime getStart() const { return start; }
	ATime getStop() const { return stop; }
	NavyMap* getNavy() { return &nm; }
	void printOrders();
	void printNavy();
	void printHistList();
private:
	ATime start;
	ATime stop;
	NavyMap nm;
	OrderQueue oq;
};

#endif