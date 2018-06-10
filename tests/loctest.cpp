#include <iostream>
#include "../src/ATime.h"
#include "../src/Location.h"


using namespace std;

int main() {
  ATime t;
  double ax, ay, bx, by;
  while (true) {
    cout << "a: ";
    cin >> ax >> ay;
    cout << "b: ";
    cin >> bx >> by;

    Location a(ax, ay, t);
    Location b(bx, by, t);
    a.print();
    b.print();
    cout << getDistance(a, b) << endl << endl;
  }

  return 0;
}
