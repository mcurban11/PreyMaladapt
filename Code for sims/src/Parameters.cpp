#include "Parameters.h"

Parameters::Parameters() {
	//Simulation
	SimNr = 42;
	rep = 1; //replicates
	gen = 5101; //generations
	ind_out = false; //output or not individuals outputs (big files!!!)
	ind_int = 100; //generations interval for individual outputs (during climate change output every 20 gen)
	pop_int = 100; //generations interval for population outputs (during climate change output every 20 gen)
	stat_int = 100; //generations interval for trait stats outputs (during climate change output every 20 gen)
	occ_int = 0;
	//Landscape
	resol = 100; //resolution (m)
	x_max = 200;
	y_max = 700; //was 300 before, I have put 200 cells on both sides
	prop_suitable = 0.2; //proportion of suitable cells 
	preyK = 100; //prey-carrying capacity (individuals/ha)
	//Environmental gradient
	static_environment_for_the_predator = false;
	env_variation = true;
	env_variance = 0.001; //variance for local variation in environmental gradient
	grad_shifts = true;
	niche = new bool[2] { false, true }; //for each species indicates if they have a niche or not 1)prey and 2) predator
	shift_start = 2500;
	shift_end = 2600;
	grad_inc = 0.075; //in degrees Celsius
	shift_rate = 0.0375; //in degrees Celsius
	//opt = new double[2]{ 15.0, 15.0 }; // environmental optima for 1) prey and 2) predator //WHAT DO I PUT HERE FOR PREY
	opt = new double[2]{ 30.0, 30.0 }; //new optimum for the predator with bigger landscape
	width = new double[2]{ 5.0, 5.0 }; //niche widths for 1) prey and 2) predator //IDEM AS ABOVE
	//initialisation
	min_seedX = 0;
	min_seedY = 0;
	max_seedX = x_max;
	max_seedY = y_max;
	pred_prop = 0.1; //initial proportion of predator (relative to preyK)
	//Traits
	L = 10; //number of loci for each trait
	defence_evol = true;
	adaptation_evol = false; 
	polymorphism = false;
	disp_evol = new bool[2]{ true, false }; //dispersal evolving for prey and predator	
	def_mean = 0.5; //initial genotypic mean for defense trait 
	def_mean_fixed = 1.0; //(defence value if defence is not evoving)
	AdaptEnv_mean = 15; //INITIAL GENOTYPIC MEAN FOR ADAPTATION TO THE ENVIRONMENT.this is useless
	def_sd = 0.1; //initial genotypic standard deviation for defense trait
	AdaptEnv_sd = 0.5; //initial geneotypic sd for adaptation (VALORE TOTALMENTE INVENTATO?)
	max_def = 1.0; //maximum phenotypic value allowed for prey defence
	min_def = 0.0; //minimum phenotypic value allowed for prey defence
	min_adapt_env = 0.0;
	max_adapt_env = 1.0;
	em_mean = new double[2]{ 0.2, 0.2 }; //initial genotypic mean for emigration probability of prey and predator (emigration p. if dispersal is not evolving)
	em_sd = new double[2]{ 0.1, 0.1 }; //initial genotypic standard deviation for emigration probability of prey and predator
	mu = 0.001; //haploid per-locus mutation probabilty
	def_mut_sd = 0.0005; //standard deviation for mutational effects in prey defence
	env_mut_sd = 0.03; //as above but just for environment adaptation
	em_mut_sd = 0.0005;//standard deviation for mutational effects in emigration probability
	//Reproduction
	prey_tradeoff = true; //trade-off between prey defense and growth rate
	defence_environmental_adaptation_tradeoff = false;
	tradeConst = 1.0; //constant for growth rate scaling in the model withj polymorphisms
	lambda0 = 1.6; //prey growth rate in the abscence of trade-off
	max_lambda = 2.0; //max prey growth rate
	min_lambda = 1.2; //min prey growth rate
	max_a = 0.5; //max predator attack rate
	min_a = 0.1; //min predator attack rate
	c = 0.5; //predator conversion efficiency (Hassell-Varley model)
	m = 0.85; //predator interference coefficient (Hassell-Varley model) - basically defines denisity dependence for the predator 
	k = 5.0; //parameter for exponenetial attack rate
	maladapt_ratio = 1.0; //CHE FO QUI
	//Dispersal
	dist = new double[2] { 100.0 , 100.0 }; //mean dispersal distance (negative exp.) for prey and predators (in metres)

	evo_stop = 100; //generation at which evolution is stopped
	var_phen = true; //is there variance in phenotype?
}
//------------------------------
void Parameters::outPara(string name) {

	ofstream out;
	
	out.open(name.c_str());

	//Simulation
	out << "SimNr\t" << SimNr << endl;
	out << "rep\t" << rep << endl; //replicates
	out << "gen\t" << gen << endl; //generations
	out << "out_ind_int\t" << ind_int << endl; //generations interval for individual outputs
	out << "out_pop_int\t" << pop_int << endl; //generations interval for population outputs
	//Landscape
	out << "resolution\t" << resol << endl; //resolution (m)
	out << "x_max\t" << x_max << endl;
	out << "y_max\t" << y_max << endl;
	out << "proportion_suitable_cells\t" << prop_suitable << endl; //proportion of suitable cells 
	out << "prey_K\t" << preyK << endl; //prey-carrying capacity (individuals/ha)
	//Environmental gradient
	out << "gradient_shift\t" << grad_shifts << endl;
	out << "prey_niche\t" << niche[0] << endl; //for each species indicates if they have a niche or not
	out << "predator_niche\t" << niche[1] << endl;
	out << "shift_start\t" << shift_start << endl;
	out << "shift_end\t" << shift_end << endl;
	out << "gradient_increment\t" << grad_inc << endl; //in degrees Celsius
	out << "shift_rate\t" << shift_rate << endl; //in degrees Celsius
	out << "prey_optimum\t" << opt[0] << endl; // environmental optima for prey and predator
	out << "predator_optimum\t" << opt[1] << endl;
	out << "prey_width\t" << width[0] << endl; //niche widths for prey and predator
	out << "predator_width\t" << width[1] << endl;
	//initialisation
	out << "min_seedX\t" << min_seedX << endl;
	out << "min_seedY\t" << min_seedY << endl;
	out << "max_seedX\t" << max_seedX << endl;
	out << "max_seedY\t" << max_seedY << endl;
	out << "predator_initial_proportion\t" << pred_prop << endl; //initial proportion of predator (relative to preyK)
	//Traits
	out << "L\t" << L << endl; //number of loci for each trait
	out << "defence_evol\t" << defence_evol << endl;
	out << "adaptation_evol\t" << adaptation_evol << endl;
	out << "defence_polymorphism\t" << polymorphism << endl;
	out << "dispersal_evol_prey\t" << disp_evol[0] << endl;
	out << "dispersal_evol_predator\t" << disp_evol[1] << endl;
	out << "defence_initial_genot_mean\t" << def_mean << endl; //initial genotypic mean for defense trait
	out << "defence_initial_genot_sd\t" << def_sd << endl; //initial genotypic standard deviation for defense trait
	out << "prey_emig_initial_genot_mean\t" << em_mean[0] << endl; //initial genotypic mean for emigration probability of prey and predator
	out << "pred_emig_initial_genot_mean\t" << em_mean[1] << endl;
	out << "prey_emig_initial_genot_sd\t" << em_sd[0] << endl; //initial genotypic standard deviation for emigration probability of prey and predator
	out << "pred_emig_initial_genot_sd\t" << em_sd[1] << endl;
	out << "mutation_prob\t" << mu << endl; //haploid per-locus mutation probabilty
	out << "defence_mutat_sd\t" << def_mut_sd << endl; //standard deviation for mutational effects in prey defence
	out << "emig_mutat_sd\t" << em_mut_sd << endl;//standard deviation for mutational effects in emigration probability
	//Reproduction
	out << "prey_tradeoff\t" << prey_tradeoff << endl; //trade-off between prey defense and growth rate
	out << "prey_max_lambda\t" << max_lambda << endl;//max prey growth rate
	out << "prey_min_lambda\t" << min_lambda << endl; //min prey growth rate
	out << "lambda\t" << lambda0 << endl;
	out << "pred_max_attackrate\t" << max_a << endl; //max predator attack rate
	out << "pred_min_attackrate\t" << min_a << endl; //min predator attack rate
	out << "pred_c\t" << c << endl;//predator conversion efficiency (Hassell-Varley model)
	out << "pred_m\t" << m << endl; //predator interference coefficient (Hassell-Varley model) - basically defines denisity dependence for the predator 
	out << "maladaptation_ratio\t" << maladapt_ratio << endl;
	//Dispersal
	out << "prey_mean_disp_dist\t" << dist[0] << endl; //mean dispersal distance (negative exp.) for prey and predators (in metres)
	out << "pred_mean_disp_dist\t" << dist[1] << endl;

	out.close();
}
//------------------------------
Parameters::~Parameters()
{
}
