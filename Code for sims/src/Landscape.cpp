#include "Landscape.h"

Landscape::Landscape() {
	pres = 0;
	suitable = 0;
	local_K = 0.0; 
	local_K0 = NULL;  
	r0 = NULL;
	r = 0.0;
	eps = 0.0;
	dev = NULL;
	ext = NULL;
	theta = NULL; 
	theta_predator = NULL;
}

//------------------------------------------------------------------
void Landscape::deleteLand(void) {
	delete local_K0; local_K0 = NULL;
	delete r0; r0 = NULL;
	delete dev; dev = NULL;
	delete ext; ext = NULL;
	delete theta; theta = NULL; 
	delete theta_predator; theta_predator = NULL; 
}
