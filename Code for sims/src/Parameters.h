#pragma once

#include <fstream>

using namespace std;

class Parameters
{
public:
	Parameters();
	~Parameters();
	//Simulation
	int SimNr;
	int rep; //replicates
	int gen; //generations
	bool ind_out; //output or not individuals outputs (big files!!!)
	int ind_int; //generations interval for individual outputs (during climate change output every 20 gen)
	int pop_int; //generations interval for population outputs (during climate change output every 20 gen)
	int stat_int; //generations interval for trait stats outputs (during climate change output every 20 gen)
	int occ_int; //generations interval for trait stats outputs
	//Landscape
	int resol; //resolution (m)
	int x_max;
	int y_max;
	double prop_suitable; //proportion of suitable cells 
	double preyK; //prey-carrying capacity (individuals/ha)
	//Environmental gradient
	bool static_environment_for_the_predator;
	bool env_variation; 
	double env_variance;
	bool grad_shifts;
	bool *niche; //for each species indicates if they have a niche or not
	int shift_start, shift_end;
	double grad_inc;
	double shift_rate;
	double *opt; // environmental optima for prey and predator
	double *width; //niche widths for prey and predator
	//initialisation
	int min_seedX;
	int min_seedY;
	int max_seedX;
	int max_seedY;
	double pred_prop; //initial proportion of predator (relative to preyK)
	//Traits
	int L; //number of loci for each trait
	bool defence_evol; //false = prey defence not evolving
	bool adaptation_evol; //false = prey adaptation not evolving
	bool polymorphism; //true = prey defence is represented by a discrte polymorphism
	bool *disp_evol; //dispersal evolving for prey and predator	
	double def_mean; //initial genotypic mean for defense trait
	double def_mean_fixed; //initial genotypic mean if defence is not evolving
	double AdaptEnv_mean; //INITIAL GENOTYPIC MEAN FOR ADAPTATION TO THE ENVIRONMENT
	double def_sd; //initial genotypic standard deviation for defense trait
	double AdaptEnv_sd; //initial genotypic standar devation for ADAPTATION TO THE ENVIRONMENT
	double max_def, min_def; //maximum and minimum phenotypic values allowed for prey defence
	double max_adapt_env, min_adapt_env;
	double *em_mean; //initial genotypic mean for emigration probability of prey and predator
	double *em_sd; //initial genotypic standard deviation for emigration probability of prey and predator
	double mu; //haploid per-locus mutation probabilty
	double def_mut_sd; //standard deviation for mutational effects in prey defence
	double env_mut_sd; //as above just for environment adaptation
	double em_mut_sd; //standard deviation for mutational effects in emigration probability
	//Reproduction
	bool prey_tradeoff; //trade-off between prey defense and growth rate
	bool defence_environmental_adaptation_tradeoff; //trade-off between prey defense and environmental adaptation
	double tradeConst; //constant for growth rate scaling in the model withj polymorphisms
	double lambda0; //prey growth rate in the abscence of trade-off
	double max_lambda; //max prey growth rate
	double min_lambda; //min prey growth rate
	double max_a; //max predator attack rate
	double min_a; //min predator attack rate
	double c; //predator conversion efficiency (Hassell-Varley model)
	double m; //predator interference coefficient (Hassell-Varley model) - basically defines denisity dependence for the predator 
	double k; //parameter for exponenetial attack rate
	double maladapt_ratio; //corresponding to m in the analytical model - used for the polymorphic model
	//Dispersal
	double *dist; //mean dispersal distance (negative exp.) for prey and predators (in metres)

	//Simulations #LOCALMAL 1
	int evo_stop; //generation at which evolution is stopped
	bool var_phen; //is there variance in phenotype?

	void outPara(string dir); //Parameter output

};

