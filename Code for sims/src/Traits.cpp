#include "Traits.h"


Traits::Traits()
{
	defence = NULL;
	adapt_env = NULL;
	defence_poly = NULL;
	//z = NULL;
	//w = NULL;
	em = NULL;
	g_defence = NULL;
	g_adapt_env = NULL;
	//g_z = NULL;
	//g_w = NULL;
	g_em = NULL;
	p_defence = NULL;
	p_adapt_env = NULL;
	fitClim = NULL;
	//p_z = NULL;
	//p_w = NULL;
	p_em = NULL;
}


Traits::~Traits()
{
}
//--------------------------------------------------------------------------------------------
void Traits::deleteTraits(void) {
	if (defence != NULL) { delete[] defence; defence = NULL; }
	if (adapt_env != NULL) {delete[] adapt_env; adapt_env = NULL;}
	if (defence_poly != NULL) { delete defence_poly; defence_poly = NULL; }
	//if (z != NULL) { delete[] z; z = NULL; }
	//if (w != NULL) { delete[] w; w = NULL; }
	if (em != NULL) { delete[] em; em = NULL; }

	if (g_defence != NULL) { delete g_defence; g_defence = NULL; }
	if (g_adapt_env != NULL) {delete g_adapt_env; g_adapt_env = NULL;}
	if (fitClim != NULL) {delete fitClim; fitClim = NULL;}
	//if (g_z != NULL) { delete g_z; g_z = NULL; }
	//if (g_w != NULL) { delete g_w; g_w = NULL; } 
	if (g_em != NULL) { delete g_em; g_em = NULL; }

	if (p_defence != NULL) { delete p_defence; p_defence = NULL; }
	if (p_adapt_env != NULL) {delete p_adapt_env; p_adapt_env = NULL;}
	//if (p_z != NULL) { delete p_z; p_z = NULL; }
	//if (p_w != NULL) { delete p_w; p_w = NULL; } 
	if (p_em != NULL) { delete p_em; p_em = NULL; }
}
