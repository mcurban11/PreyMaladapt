#pragma once

#include <stdio.h>
#include <vector>

#include "Individuals.h"
#include "Parameters.h"
#include "Landscape.h"

using namespace std;

class Population {
public:
	Population(int, int, int); //pass species ID and coordinates
	int x, y;
	int spID; //0 = prey; 1 = predator
	int N, Noffs;

	double def_mean, def_sd; //defence mean and std for simulations with #LOCALMAL 1

	vector<Individuals> Inds, Offs;

	void initialise_pop(Parameters para, Landscape ***arena, std::normal_distribution<> distrDef, std::normal_distribution<> distrAdaptEnv,std::normal_distribution<> distrEm, std::uniform_int_distribution<>);
	void deletePop(void);
};


