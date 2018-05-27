#include "SimManager.h"
#include "Order.h"
#include "ATime.h"
#include "Movable.h"
#include <map>
#include <deque>
#include <algorithm>
typedef std::map<std::string, Movable*>::iterator MapIterator;
typedef std::deque<Order*>::iterator OrderIterator;
using namespace std;

int comp(Order* a, Order* b) {
	return (*a < *b);
}

bool SimulationMgr::simInit(string fname) {
	string s;
	string opcode;
	
	ifstream fin;
	fin.open(fname.c_str());
	if (fin.fail()) {
		cout << "Order file opening failed." << endl;
	}
	while (getline(fin, s)) {
		if (s.length() == 0 || s[0] == '#') continue;
		istringstream is(s);
		is >> opcode;
		if (opcode == "StartSim") {
			string mdy, hms;
			if (is >> mdy >> hms) {
				start = ATime(mdy, hms);
			}
			else return false;
		}
		else if (opcode == "EndSim" || opcode == "StopSim") {
			string mdy, hms;
			if (is >> mdy >> hms) {
				stop = ATime(mdy, hms);
			}
			else return false;
		}
		else if (opcode == "CreateCruiser") {
			string Id, name;
			double MaxSpeed;
			int nmissiles;
			if (is >> name >> Id >> nmissiles >> MaxSpeed) {
				Movable* mp = new Cruiser(name, Id, nmissiles, MaxSpeed);
				nm[Id] = mp;
			}
			else return false;
		}
		else if (opcode == "CreateAircraftCarrier") {
			string Id, name;
			double MaxSpeed;
			int MaxAircraft;
			if (is >> name >> Id >> MaxAircraft >> MaxSpeed) {
				Movable* mp = new Carrier(name, Id, MaxAircraft, MaxSpeed);
				nm[Id] = mp;
			}
			else return false;
		}
		else if (opcode == "CreateFighter") {
			string Id, name, ShipId;
			double MaxSpeed, MaxCeiling;
			int MaxBombs;
			if (is >> name >> Id >> ShipId >> MaxSpeed >> MaxCeiling >> MaxBombs) {
				MapIterator nmi;
				nmi = nm.find(ShipId);
				if (nmi == nm.end())
				{
					cout << "Error. Could not find Carrier!" << endl;
					return false;
				}
				Movable* sptr = nmi->second;
				Movable* mp = new Fighter(name, Id, sptr, MaxCeiling, MaxSpeed, MaxBombs);
				nm[Id] = mp;
			}
			else return false;
		}
		else if (opcode == "DeployShip") {
			string mdy, hms, Id;
			double x, y, head, spd;
			if (is >> mdy >> hms >> Id >> x >> y >> head >> spd) {
				ATime atm(mdy, hms);
				Order* op = new DeployShip(atm, Id, x, y, head, spd);
				oq.push_back(op);
			}
		}
		else if (opcode == "DeployAircraft") {
			string mdy, hms, Id;
			double z, head, spd;
			if (is >> mdy >> hms >> Id >> head >> spd >> z) {
				ATime atm(mdy, hms);
				Order* op = new DeployAircraft(atm, Id, head, spd, z);
				oq.push_back(op);
			}
		}
		else if (opcode == "ChangeShipOrders") {
			string mdy, hms, Id;
			double head, spd;
			if (is >> mdy >> hms >> Id >> head >> spd) {
				ATime atm(mdy, hms);
				Order* op = new ChangeShip(atm, Id, head, spd);
				oq.push_back(op);
			}
		}
		else if (opcode == "ChangeAircraftOrders") {
			string mdy, hms, Id;
			double z, head, spd;
			if (is >> mdy >> hms >> Id >> head >> spd >> z) {
				ATime atm(mdy, hms);
				Order* op = new ChangeAircraft(atm, Id, head, spd, z);
				oq.push_back(op);
			}
		}
		else if (opcode == "LandAircraft") {
			string mdy, hms, Id, sId;
			if (is >> mdy >> hms >> sId >> Id) {
				MapIterator nmi;
				ATime atm(mdy, hms);
				nmi = nm.find(sId);
				if (nmi == nm.end()) {
					cout << "Error. Could not find Carrier!" << endl;
					return false;
				}
				Movable* sptr = nmi->second;
				Order* op = new LandAircraft(atm, sptr, Id);
				oq.push_back(op);
			}
		}
		else return false;
	}
	sort(oq.begin(), oq.end(), comp);
	return true;
}

void SimulationMgr::printOrders() {
	OrderIterator oi;
	for (oi = oq.begin(); oi != oq.end(); oi++) {
		(*oi)->print();
	}
}

void SimulationMgr::printNavy() {
	MapIterator mi;
	for (mi = nm.begin(); mi != nm.end(); mi++) {
		(mi->second)->print();
	}
}

void SimulationMgr::printHistList() {
	MapIterator mi;
	for (mi = nm.begin(); mi != nm.end(); mi++) {
		(mi->second)->print();
		(mi->second)->printHL();
	}
}

void SimulationMgr::simDoUpdate(ATime a) {
	NavyMap::iterator nmi;
	MapIterator mi;
	Order* oP;
	while (oq.size()) {
		oP = oq.front();
		if (oP->exectime() > a) break;
		oq.pop_front();
		nmi = nm.find(oP->get_id());
		if (nmi != nm.end()) {
			oP->Execute(nmi->second, a);
		}
		delete oP;
	}
	for (mi = nm.begin(); mi != nm.end(); mi++) {
		if ((mi->second)->isDeployed()) {
			(mi->second)->updatePosition(a);
		}
	}
}