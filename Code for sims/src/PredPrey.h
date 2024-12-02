#pragma once

#define CLUSTER 1

#define LOCALMAL 0 //for simulations where evolution is stopped after having reached equilibrium
#define LOCALENV 0

#include <stdio.h>
#include <stdlib.h>

#if CLUSTER 
#include <unistd.h>
#include "io.h"
#else
#include <tchar.h> 
#include <direct.h>
#include <io.h>
#endif
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <numeric>
#include <time.h>
#include <random>
#include <iterator>

#include "Parameters.h"
#include "Landscape.h"
#include "Community.h"
#include "Stats.h"


using namespace std;

Parameters para;
string dir, dirOut;
string experimentName;
int r, g; // counters for replicates and generations

Landscape ***land;
Community ***community;
trait_stats ***stats;
occupancy_stats occ_stats;

const double PI = 3.141592654;

//Statistics---------------------------------
int maxRow, Total_Cells;
int Nspecies[2]; // to store the total number of inds of each species
double SppOpt[2];
double fitClim;
double mean_defence;

std::ofstream par, inds, pops, trait, occup;

// Random numbers generators
// seed random number generator
std::random_device rd;
std::mt19937 rdgen(rd());

std::uniform_real_distribution<> unireal(0.0, 1.0);
std::uniform_int_distribution<> unint(0, 1);
std::uniform_int_distribution<> uni_allele(0, 2 * para.L - 1); //for sampling alleles for mutations

//prey defence mutational effects distribution
std::normal_distribution<> mutDef(0.0, para.def_mut_sd);
//prey adaptation to environment effects distribution
std::normal_distribution<> mutEnv(0.0, para.env_mut_sd);
//emigration probablity mutational effects distribution
std::normal_distribution<> mutEm(0.0, para.em_mut_sd);



// Functions declaration
const string Int2Str(const int x);
const string Float2Str(const double x);
void landscape(int, int, double);
void envGradient(int, int);
void envGradientShift(int, int);
void initialisation(int, int, int, int, int, int); 
void RunModel(void);
void reproduction(void);
void dispersal(void);
void inheritance(int id, Individuals *pup, Individuals mom, Individuals dad);
void sample_def_phenotype(Individuals pup, std::normal_distribution<> dist);
void sample_adapt_phenotype(Individuals pup, std::normal_distribution<> dist);
void mutations(int x, int y);
void outInd_header(void);
void outPop_header(void);
void outStat_header(void);
void outOccupancy_header(void);
