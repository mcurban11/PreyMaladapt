#include "Population.h"
#include "Individuals.h"


Population::Population(int ID, int xx, int yy) {
	spID = ID;
	x = xx;
	y = yy;
	N = 0;
	Noffs = 0;
	def_mean = 0.0;
	def_sd = 0.0;
}
//----------------------------------------
void Population::initialise_pop(Parameters para, Landscape ***arena, std::normal_distribution<> distrDef, std::normal_distribution<> distrAdaptEnv, std::normal_distribution<> distrEm,
	std::uniform_int_distribution<> poly) {

	if (spID == 0) { //prey
		N = (int)para.preyK;
		Noffs = 0;
		for (int i = 0; i < N; i++) {
			Inds.push_back(Individuals(spID, x, y));
			Inds[i].initialise(para, arena, distrDef, distrAdaptEnv, distrEm, poly);
		}

	}
	else { //predator
		N = (int)(para.preyK * para.pred_prop);
		Noffs = 0;
		for (int i = 0; i < N; i++) {
			Inds.push_back(Individuals(spID, x, y));
			Inds[i].initialise(para, arena, distrDef, distrAdaptEnv, distrEm, poly);
		}
	}
}

//----------------------------------------
void Population::deletePop(void) {
	Inds.clear();
	Offs.clear();
}