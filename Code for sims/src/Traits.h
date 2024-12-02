#pragma once

#include <stdio.h>

class Traits
{
public:
	Traits();
	~Traits();
	// allele 1 and 2 for each locus are consecutive in the array
	double *defence; //prey "investment" in defence
	double *adapt_env; //prey "adaptation" to the environment
	int *defence_poly; //defence discrete polymorphism (0 or 1)

	//double *z; //gradient optima //for now treat these as population parameters
	//double *w; //gradient width

	double *em; //emigration probability

	//genotypic values
	double *g_defence; //prey "investment" in defence
	//double *g_z;
	//double *g_w;
	double *g_em;
	double *g_adapt_env;

	//phenotypic values
	double *p_defence; //prey "investment" in defence 0 <= p_defence <= 1
	//double *p_z;
	//double *p_w;
	double *p_em;
	double *p_adapt_env; //actually maybe I can leave it identical to g_adapt env?
	double *fitClim; //fitness for adaptation to climate

	void deleteTraits(void);
};


