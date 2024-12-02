#pragma once

#include <stdio.h>

#include "Population.h"
#include "Parameters.h"
#include "Stats.h"
#include "Landscape.h"

class Community {
public:
	Community(int, int);
	int x, y;
	Population **pops;  // for now fixed to two species

	void initialise_comm(Parameters para, Landscape ***arena, std::normal_distribution<> distrDef, std::normal_distribution<> distrAdaptEnv,
		 std::normal_distribution<> distrEmPrey, std::normal_distribution<> distrEmPred, std::uniform_int_distribution<>);
	void outPop(int r, int g, double t, std::ofstream *out); //population output
	void delete_comm(void);
};


