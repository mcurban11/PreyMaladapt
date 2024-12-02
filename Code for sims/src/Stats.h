#pragma once

#include <fstream>

#include "Parameters.h"
#include "Landscape.h"
#include "Individuals.h"
#include "Population.h"


using namespace std;

class trait_stats {
public:
	trait_stats(Parameters para);
	double *defence_mean;
	double *defence_sd;
	double *defence_min;
	double *defence_max;

	double *adapt_env_mean;
	double *adapt_env_sd;
	double *adapt_env_max;
	double *adapt_env_min;
	double *fitClim_mean;
	double *fitClim_sd;

	double *prey_emig_mean;
	double *prey_emig_sd;
	double *prey_emig_min;
	double *prey_emig_max;

	double *pred_emig_mean;
	double *pred_emig_sd;
	double *pred_emig_min;
	double *pred_emig_max;

	void compute_sums(Parameters para, Individuals ind);
	void compute_stats(Parameters para, Population pop);
	void set2zero(Parameters para);
	void outStats(int r, int g, int x, int y, Parameters para, std::ofstream *out);
	void delete_stats(Parameters para);
};

//-----------------------------------------------------
class occupancy_stats {
public:
	occupancy_stats();
	int *suit; //number of suitable cells for prey and predator - if there are abiotic constraints this number considers
			   //cells where survival probability is greater than 0
	int *suit10; //number of suitable cells where survival prob. is greater than 10%
	int *suit20; //number of suitable cells where survival prob. is greater than 20%
	int *suit50; //number of suitable cells where survival prob. is greater than 50%

	int *pres; //number of OCCUPIED suitable cells for prey and predator - if there are abiotic constraints this number considers
			   //cells where survival probability is greater than 0
	int *pres10; //number of OCCUPIED suitable cells where survival prob. is greater than 10%
	int *pres20; //number of OCCUPIED suitable cells where survival prob. is greater than 20%
	int *pres50; //number of OCCUPIED suitable cells where survival prob. is greater than 50%

	double *occ; //occupancy for prey and predator - if there are abiotic constraints this number considers
			     //cells where survival probability is greater than 0
	double *occ10; //occupancy of suitable cells where survival prob. is greater than 10%
	double *occ20; //occupancy of suitable cells where survival prob. is greater than 20%
	double *occ50; //occupancy of suitable cells where survival prob. is greater than 50%

	void sum_suitable(int x, int y, Landscape ***arena, int Nprey, int Npred, Parameters para);
	void compute_occupancy(void);
	void set2zero(void);
	void outStats(int r, int g, Parameters para, std::ofstream *out);
	void delete_stats(void);
};
