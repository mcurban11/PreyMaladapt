#include "Community.h"



Community::Community(int xx, int yy) {
	x = xx; y = yy;
	pops = new Population *[2];
	for (int i = 0; i < 2; i++) pops[i] = new Population(i, x, y);
}

//------------------------------------
void Community::initialise_comm(Parameters para, Landscape ***arena, std::normal_distribution<> distrDef,std::normal_distribution<> distrAdaptEnv,
	std::normal_distribution<> distrEmPrey, std::normal_distribution<> distrEmPred, std::uniform_int_distribution<> poly) {
	for (int i = 0; i < 2; i++) {
		if (i == 0) pops[i]->initialise_pop(para, arena,  distrDef, distrAdaptEnv, distrEmPrey, poly); //prey
		else pops[i]->initialise_pop(para, arena, distrDef, distrAdaptEnv,distrEmPred, poly); //predator
	}
}
//----------------------------------------
void Community::outPop(int r, int g, double t, std::ofstream *out) {
	*out << r << "\t" << g << "\t" << x << "\t" << y << "\t" << t << "\t" << pops[0]->N << "\t" << pops[1]->N << endl;
}
//------------------------------------
void Community::delete_comm(void) {
	for (int i = 0; i < 2; i++) {
		delete pops[i];
		pops[i] = NULL;
	}				
	delete[] pops;
	pops = NULL;
}


