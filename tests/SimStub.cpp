
#if defined(WIN32)
#include <Windows.h>
#endif
#include <iostream>
#include "SimManager.h"

using namespace std;

SimulationMgr sm;
ATime simClock;

typedef map<string, int> NameColorMap;
NameColorMap gNCmap;

// this is a wrapper to nanosleep which sleeps for a certain
// number of milliseconds
void delay(int ms) {
#if defined(WIN32)
	Sleep(ms);
#else
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = 1000000 * (ms % 1000);
	nanosleep(&ts, NULL);
#endif
}

void getNavyInfo() {
	NavyMap* nm = sm.getNavy();
	NavyMap::iterator nmi;
	HistoryList* hlp;
	HistoryList::iterator hli;
	for (nmi = nm->begin(); nmi != nm->end(); nmi++) {
		cout << nmi->first << endl;
		hlp = (nmi->second)->getHistory();
		for (hli = hlp->begin(); hli != hlp->end(); hli++) {
			double x, y, z;
			hli->getXYZ(x, y, z);
			cout.precision(3);
			cout.setf(ios::fixed);
			cout << x << " " << y << " " << z << endl;
		}
	}
}

int
main(int ac, char* av[]) {

	if (ac != 2) {
		cout << "Usage: " << av[0] << " <orders>" << endl;
		return 1;
	}
	if (sm.simInit(av[1])) {
		sm.printOrders();
		sm.printNavy();
		simClock = sm.getStart();
		while (simClock < sm.getStop()) {
			sm.simDoUpdate(simClock);
			simClock += 60;  // add one minute
		}

		getNavyInfo();
	}
	else {
		cout << "Init failed" << endl;
	}
	return 0;
}