#ifndef NOH_H
#define NOH_H

#include <iomanip>
#include <fstream>
#include <list>
#include <algorithm>
#include<cmath>

#include "labirinto.h"
#include "coord.h"

using std::abs;

class NoH
{

public:
	NoH() {}

	double f()
	{
		return g + h;
	}



private:
	Coord pos;
	Coord ant;

	double g;
	double h;

};


#endif