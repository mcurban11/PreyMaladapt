#pragma once

#include <stdio.h>
#include <fstream>
#include <math.h>
#include <random>

#include "Traits.h"
#include "Parameters.h"
#include "Landscape.h"

using namespace std;


class Individuals {
public:
	Individuals(int, int, int);
	bool alive;
	int spID;
	int x, y;    //cell coordinates
	int xnatal, ynatal; //coordinates of natal cell

	Traits traits;

	//float *adapt_gen; //two alleles

	void initialise(Parameters para, Landscape ***arena, std::normal_distribution<> distrDef, std::normal_distribution<> distrAdaptEnv, std::normal_distribution<> distrEm, std::uniform_int_distribution<>);
	void mutation(Parameters para, Landscape ***arena, int trait, int allele, std::normal_distribution<>);
	void outInd(Parameters para, int r, int g, std::ofstream *out); //individual output
	void deleteInd(void);

private:
};

