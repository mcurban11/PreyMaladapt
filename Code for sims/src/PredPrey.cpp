#include "PredPrey.h"


//---------------------------------------------------------------------------
// Main function
#if CLUSTER
int main(int argc, char* argv[])
{
	experimentName = argv[1];
	dirOut = "Outputs/";
	dirOut = prepOutputFolder(dirOut, experimentName);

	para.adaptation_evol = std::atoi(argv[2]);
	para.dist[0] = std::atof(argv[3]);
	para.dist[1] = std::atof(argv[4]);
	para.width[0] = std::atof(argv[5]);
	para.defence_evol = std::atoi(argv[6]);
	para.env_variance = std::atof(argv[7]);
	para.env_variation = std::atoi(argv[8]);
	para.env_mut_sd = std::atof(argv[9]);

	//para.grad_shifts = std::atoi(argv[4]);
	//para.def_mean = std::atof(argv[6]);
	// para.c = std::atof(argv[5]); // 0.5
	//para.max_def = std::atof(argv[9]);
	//para.min_def = std::atof(argv[10]);
	//para.polymorphism = std::atoi(argv[11]);
	// para.prey_tradeoff = std::atoi(argv[6]); // true
	// para.maladapt_ratio = std::atof(argv[7]); // 1.0

	RunModel();

	cout << "Simulation completed" << endl;

	return 0;
}
#else
int _tmain(int argc, _TCHAR* argv[])
{

	// Get the current directory.
	char* buffer = _getcwd(NULL, 0);
	dir = buffer;
	free(buffer);
	dir = dir + "\\"; //Current directory path
	dirOut = dir + "Outputs\\"; //Outpus folder path
	//cout << dirOut <<endl;

	RunModel();

	cout << "Simulation completed" << endl;

	return 0;
}
#endif

//---------------------------------------------------------------------------
const string Int2Str(const int x)
{
	ostringstream o;
	if (!(o << x)) return "ERROR";
	return o.str();
}
//---------------------------------------------------------------------------
const string Float2Str(const double x)
{
	ostringstream o;
	if (!(o << x)) return "ERROR";
	return o.str();
}
//---------------------------------------------------------------------------
void RunModel(void) {
	vector<Individuals>::iterator iter;

	cout << "Experiment Name: " << experimentName << endl;

	string name = dirOut + "Para.txt";
	para.outPara(name);
	if (para.ind_out) outInd_header();
	outPop_header();
	if (para.defence_evol || para.adaptation_evol ||para.disp_evol[0] || para.disp_evol[1] || para.polymorphism) outStat_header();
	outOccupancy_header();

	//Loop through REPLICATES --------------------------------------------
	for (r = 0; r < para.rep; r++) {
		cout << "rep = " << r << endl;
		// r = para.SimNr; potentially use this to generate differently numbered files with correct replicate number in first column
		//create a new landscape
		landscape(para.x_max, para.y_max, para.prop_suitable);

		//create the environmental gradient
		envGradient(para.x_max, para.y_max);

		//Initialisation
		initialisation(para.x_max, para.y_max, para.min_seedX, para.min_seedY, para.max_seedX, para.max_seedY);

		//Loop through GENERATIONS ------------------------------------
		for (g = 0; g < para.gen; g++) {

			cout << "gen = " << g << endl;
			//Gradient shifting
			if (para.grad_shifts && g >= para.shift_start && g <= para.shift_end) envGradientShift(para.x_max, para.y_max);

			//Reproduction
			reproduction();

			//Dispersal and viability selection
			dispersal();
		}

		//Delete stats array
		if (para.defence_evol || para.adaptation_evol || para.disp_evol[0] || para.disp_evol[1] || para.polymorphism) {
			if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0))) {
				for (int x = 0; x < para.x_max; x++) {
					for (int y = 0; y < para.y_max; y++) {
						if (land[x][y]->suitable) stats[x][y]->delete_stats(para);
						delete stats[x][y];
						stats[x][y] = NULL;
					}
					delete[] stats[x];
					stats[x] = NULL;
				}
				delete[] stats;
				stats = NULL;
			}
		}

		//Delete individuals, populations, communities and the landscape
		for (int x = 0; x < para.x_max; x++) {
			for (int y = 0; y < para.y_max; y++) {
				if (land[x][y]->suitable) {
					if (land[x][y]->pres) {
						if (community[x][y]->pops[0]->N > 0) {
							for (iter = community[x][y]->pops[0]->Inds.begin(); iter != community[x][y]->pops[0]->Inds.end(); iter++) {
								iter->deleteInd();
							}
							community[x][y]->pops[0]->deletePop();
						}
						if (community[x][y]->pops[1]->N > 0) {
							for (iter = community[x][y]->pops[1]->Inds.begin(); iter != community[x][y]->pops[1]->Inds.end(); iter++) {
								iter->deleteInd();
							}
							community[x][y]->pops[1]->deletePop();
						}
					}
					community[x][y]->delete_comm();
					delete community[x][y];
					community[x][y] = NULL;

					land[x][y]->deleteLand();
					delete land[x][y];
					land[x][y] = NULL;
				}
			}
			delete[] community[x]; 
			community[x] = NULL;
			delete[] land[x];
			land[x] = NULL;
		}
		delete[] community;
		community = NULL;
		delete[] land;
		land = NULL;
	}

	//para.close();
	if (inds.is_open()) inds.close();
	if (pops.is_open()) pops.close();
	if (trait.is_open()) trait.close();
	if (occup.is_open()) occup.close();
}
//---------------------------------------------------------------------------
void landscape(int xmax, int ymax, double prop) {

	int x, y, i, ncells;
	std::uniform_int_distribution<> ranx(0, xmax - 1);
	std::uniform_int_distribution<> rany(0, ymax - 1);

	land = new Landscape **[xmax];
	for (int j = 0; j < xmax; j++) {
		land[j] = new Landscape *[ymax];
		for (int jj = 0; jj < ymax; jj++) {
			land[j][jj] = new Landscape();
		}
	}


	ncells = (int)(xmax * ymax * prop); //nr. of suitable cells 

	i = 0;
	do {
		do {
			x = ranx(rdgen); 
			y = rany(rdgen);
		} while (land[x][y]->suitable == 1);
		land[x][y]->suitable = 1;
		land[x][y]->local_K = para.preyK; //prey K
		i++;
	} while (i < ncells);

}
//---------------------------------------------------------------------------
void envGradient(int xmax, int ymax) {
	if (para.env_variation){
		for (int y = 0; y < ymax; y++) {
			double env_mean = para.grad_inc*(ymax-y-1);
			std::normal_distribution<> distrEnvironmentalVariation(env_mean, para.env_variance);
			for (int x = 0; x < xmax; x++) {
				land[x][y]->theta = new double(distrEnvironmentalVariation(rdgen));
				if(para.static_environment_for_the_predator){
					land[x][y]->theta_predator = new double(para.grad_inc*(ymax-y-1));
				}
				else{
					land[x][y]->theta_predator = new double(distrEnvironmentalVariation(rdgen));
				}
			}
		}
	}
	else{
		for (int y = 0; y < ymax; y++) {
			for (int x = 0; x < xmax; x++) {
				land[x][y]->theta = new double(para.grad_inc*(ymax-y-1));
				land[x][y]->theta_predator = new double(para.grad_inc*(ymax-y-1));
			}
		}
	}
}
//---------------------------------------------------------------------------
void envGradientShift(int xmax, int ymax) {
	for (int i = 0; i < 2; i++) SppOpt[i] += para.shift_rate;
	for (int y = 0; y < ymax; y++) {
		for (int x = 0; x < xmax; x++) {
			*land[x][y]->theta += para.shift_rate;
			*land[x][y]->theta_predator += para.shift_rate;
		}
	}
}
//---------------------------------------------------------------------------
void initialisation(int xmax, int ymax, int minSx, int minSy, int maxSx, int maxSy) {

	Total_Cells = 0;
	for (int i = 0; i < 2; i++) {
		Nspecies[i] = 0;
		SppOpt[i] = para.opt[i];
	}

	//Create Community array 
	community = new Community **[xmax];
	for (int j = 0; j < xmax; j++) {
		community[j] = new Community *[ymax];
		for (int jj = 0; jj < ymax; jj++) {
			if (land[j][jj]->suitable) community[j][jj] = new Community(j, jj);
		}
	}
	//Create stats array
	if (para.defence_evol || para.adaptation_evol || para.disp_evol[0] || para.disp_evol[1] || para.polymorphism) {
		if (para.stat_int != 0) {
			stats = new trait_stats **[xmax];
			for (int j = 0; j < xmax; j++) {
				stats[j] = new trait_stats *[ymax];
				for (int jj = 0; jj < ymax; jj++) {
					if (land[j][jj]->suitable) stats[j][jj] = new trait_stats(para);
				}
			}
		}
	}

	// normal distributions for allele initialisation
	if (para.defence_evol){
		mean_defence = para.def_mean;
	}
	else{
		mean_defence = para.def_mean_fixed;
	}
	std::normal_distribution<> distrDef(mean_defence / (2.0 * (double)para.L), para.def_sd / sqrt(2.0 * (double)para.L));
	//std::normal_distribution<> distrAdaptEnv(para.AdaptEnv_mean / (2.0 * (double)para.L), para.AdaptEnv_sd / sqrt(2.0 * (double)para.L));
	std::normal_distribution<> distrEmPrey(para.em_mean[0] / (2.0 * (double)para.L), para.em_sd[0] / sqrt(2.0 * (double)para.L));
	std::normal_distribution<> distrEmPred(para.em_mean[1] / (2.0 * (double)para.L), para.em_sd[1] / sqrt(2.0 * (double)para.L));

	//Inintialise all the suitable cells in the defined area
	for (int x = minSx; x < maxSx; x++) {
		for (int y = minSy; y < maxSy; y++) {
			if (land[x][y]->suitable) {
				std::normal_distribution<> distrAdaptEnv(*land[x][y]->theta / (2.0 * (double)para.L), para.AdaptEnv_sd / sqrt(2.0 * (double)para.L));
				community[x][y]->initialise_comm(para, land, distrDef, distrAdaptEnv, distrEmPrey, distrEmPred, unint);
				land[x][y]->pres = 1;
				for (int i = 0; i < 2; i++) Nspecies[i] += community[x][y]->pops[i]->N;
				Total_Cells++;
			}
		}
	}
}
//---------------------------------------------------------------------------
void reproduction(void) {
	int offs;
	int dad; //index referring to the position of the "dad" in the Inds vector
	int prey_deaths;
	double e, E; //expected number of offspring
	double lambda; //growth rate
	double a; //predator attack rate
	Individuals *ind;
	vector<Individuals>::iterator iter;

	for (int i = 0; i < 2; i++) Nspecies[i] = 0;
	Total_Cells = 0;

	double cost = para.min_lambda - para.max_lambda; //to prey growth rate (eqn 11)
	double b = para.min_a - para.max_a; //to attack rate (eqn 10)

	for (int x = 0; x < para.x_max; x++) {
		for (int y = 0; y < para.y_max; y++) {
			if (land[x][y]->suitable && land[x][y]->pres == 1) {

				prey_deaths = 0;

				//Prey
				if (community[x][y]->pops[0]->N > 0) {

					std::uniform_int_distribution<> getDad(0, community[x][y]->pops[0]->N - 1);

					for (iter = community[x][y]->pops[0]->Inds.begin(); iter != community[x][y]->pops[0]->Inds.end(); iter++) {

						if (para.polymorphism) {
							if (para.prey_tradeoff) {
								if (*iter->traits.defence_poly) {
									//31/12/18
									//lambda = para.min_lambda;
									//attack rate
									a = para.min_a;
									//lambda
									lambda = para.min_lambda;
								}
								else {
									//31/12/18
									//lambda = para.max_lambda;
									//attack rate 
									a = para.maladapt_ratio * para.min_a;
									//lambda
									lambda = para.tradeConst * para.maladapt_ratio * para.min_lambda;
								}
							}
							else {
								lambda = para.lambda0;
								if (*iter->traits.defence_poly) a = para.min_a;
								else a = para.maladapt_ratio * para.min_a;
							}
						}
						else { //no polymorphism
							//this assumes both tradeoff for defense and adaptation
							//cout << "fitness climate " << *iter->traits.fitClim << " adaptation to env " << *iter->traits.p_adapt_env;
							//lambda = (*iter->traits.fitClim) * para.max_lambda + (para.min_lambda -  para.max_lambda) * (*iter->traits.p_defence);//TEST ON MY OWN ACCOUNT
							//NEW EQUATION FOR GROWTH RATE WHERE W MULTIPLIES EVERYTHING BELOW
							if (para.prey_tradeoff && para.defence_environmental_adaptation_tradeoff == false) lambda = (*iter->traits.fitClim) * (para.max_lambda + (para.min_lambda -  para.max_lambda) * (*iter->traits.p_defence)); //MODIFIED growth rate (eqn.11)
							else if (para.prey_tradeoff && para.defence_environmental_adaptation_tradeoff == true) lambda = (1 - (*iter->traits.p_defence)) * (*iter->traits.fitClim) * (para.max_lambda + (para.min_lambda -  para.max_lambda) * (*iter->traits.p_defence));
							else lambda = para.lambda0;

							//attack rate
							a = para.max_a + b * (*iter->traits.p_defence); //linear
							//a = 0.4 / (1.0 + exp(20.0 * ((*iter->traits.p_defence) - 0.3))) + 0.1; // logistic
							//a = 0.4 * exp(-para.k * (*iter->traits.p_defence)) + 0.1; // exponential
						}

						e = exp(-a * pow(community[x][y]->pops[1]->N, 1.0 - para.m)); //eqn.12

						//Prey survival & reproduction
						if (unireal(rdgen) < e) {
							//expected number of offspring
							E = exp(lambda * (1.0 - community[x][y]->pops[0]->N / land[x][y]->local_K));
							//generate offspring
							if (E > 0.0) {
								std::poisson_distribution<> Offsp(E);
								offs = Offsp(rdgen);
								if (offs > 0) {
									for (int i = 0; i < offs; i++) {
										//select a "dad"
										dad = getDad(rdgen); //any individual (ermaphrodites + selfing allowed)
										//create new individual
										ind = new Individuals(0, x, y);

										inheritance(0, ind, *iter, community[x][y]->pops[0]->Inds[dad]);
#if LOCALMAL == 1
										if (g > para.evo_stop) {
											if (para.var_phen) {
												std::normal_distribution<> phenotype(community[x][y]->pops[0]->def_mean, community[x][y]->pops[0]->def_sd);
												sample_def_phenotype(*ind, phenotype);
											}
											else *ind->traits.p_defence = community[x][y]->pops[0]->def_mean;
										}
#endif

										community[x][y]->pops[0]->Offs.push_back(*ind);

										community[x][y]->pops[0]->Noffs++;

										delete ind;
									}
								}
							}
						}
						else prey_deaths++;
					}
					//All adults die
					community[x][y]->pops[0]->N = 0;
					for (iter = community[x][y]->pops[0]->Inds.begin(); iter != community[x][y]->pops[0]->Inds.end(); iter++) iter->deleteInd();
					community[x][y]->pops[0]->Inds.clear();

					Nspecies[0] += community[x][y]->pops[0]->Noffs;
				}
				//Predator
				if (community[x][y]->pops[1]->N > 0) {

					std::uniform_int_distribution<> getDad(0, community[x][y]->pops[1]->N - 1);

					//expected number of offspring (the same for each predator)
					E = ((double)prey_deaths * para.c) / community[x][y]->pops[1]->N;

					if (E > 0.0) {
						std::poisson_distribution<> Offsp(E);

						for (iter = community[x][y]->pops[1]->Inds.begin(); iter != community[x][y]->pops[1]->Inds.end(); iter++) {
							//generate offspring
							offs = Offsp(rdgen);
							if (offs > 0) {
								for (int i = 0; i < offs; i++) {
									//select a "dad"
									dad = getDad(rdgen); //any individual (ermaphrodites + selfing allowed)
									//create new individual
									ind = new Individuals(1, x, y);

									inheritance(1, ind, *iter, community[x][y]->pops[1]->Inds[dad]);

									community[x][y]->pops[1]->Offs.push_back(*ind);

									community[x][y]->pops[1]->Noffs++;

									delete ind;
								}
							}
						}
					}

					//All adults die
					community[x][y]->pops[1]->N = 0;
					for (iter = community[x][y]->pops[1]->Inds.begin(); iter != community[x][y]->pops[1]->Inds.end(); iter++) iter->deleteInd();
					community[x][y]->pops[1]->Inds.clear();

					Nspecies[1] += community[x][y]->pops[1]->Noffs;
				}

				if (community[x][y]->pops[0]->Noffs == 0 && community[x][y]->pops[1]->Noffs == 0) land[x][y]->pres = 0;
				else {
#if LOCALMAL == 0
					mutations(x, y);
#else
					if (g < para.evo_stop + 1) mutations(x, y);
#endif
					Total_Cells++;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void inheritance(int id, Individuals *pup, Individuals mom, Individuals dad) {
	int rdn, rdn2;

	//Prey
	if (id == 0) {
		if (para.defence_evol) {

			if (para.polymorphism) {
				rdn = unint(rdgen);
				if (rdn) pup->traits.defence_poly = new int(*mom.traits.defence_poly);
				else pup->traits.defence_poly = new int(*dad.traits.defence_poly);
			}
			else {
				//for defence inheritance
				pup->traits.defence = new double[2 * para.L];
				pup->traits.g_defence = new double(0.0);
				pup->traits.p_defence = new double(0.0);

				for (int i = 0; i < para.L; i++) {
					rdn = unint(rdgen);
					rdn2 = unint(rdgen);
					pup->traits.defence[i * 2] = mom.traits.defence[i * 2 + rdn];
					pup->traits.defence[i * 2 + 1] = dad.traits.defence[i * 2 + rdn2];
					*pup->traits.g_defence += pup->traits.defence[i * 2] + pup->traits.defence[i * 2 + 1];
				}
				//Phenotypes
				*pup->traits.p_defence = *pup->traits.g_defence;
				if (*pup->traits.p_defence < para.min_def) *pup->traits.p_defence = para.min_def;
				if (*pup->traits.p_defence > para.max_def) *pup->traits.p_defence = para.max_def;

				

			}
		}
		else {
			if (para.polymorphism) {
				rdn = unint(rdgen);
				if (rdn) pup->traits.defence_poly = new int(*mom.traits.defence_poly);
				else pup->traits.defence_poly = new int(*dad.traits.defence_poly);
			}
			else {
				pup->traits.p_defence = new double(para.def_mean_fixed);
			
			}
		}

		if (para.adaptation_evol){
			//for adaptation to environment
			pup->traits.adapt_env = new double[2 * para.L];
			pup->traits.g_adapt_env = new double(0.0);
			pup->traits.p_adapt_env = new double(0.0);
			pup->traits.fitClim =  new double(0.0);

			for (int i = 0; i < para.L; i++) {
				rdn = unint(rdgen);
				rdn2 = unint(rdgen);
				pup->traits.adapt_env[i * 2] = mom.traits.adapt_env[i * 2 + rdn];
				pup->traits.adapt_env[i * 2 + 1] = dad.traits.adapt_env[i * 2 + rdn2];
				*pup->traits.g_adapt_env += (pup->traits.adapt_env[i * 2] + pup->traits.adapt_env[i * 2 + 1]); 
			}
			//Phenotypes
			*pup->traits.p_adapt_env = *pup->traits.g_adapt_env;
			*pup->traits.fitClim = exp(-pow( ((*pup->traits.p_adapt_env)-(*land[(pup -> x)][(pup -> y)]->theta)), 2.0) / (2.0 * para.width[0])); 
		}
		else{
			pup->traits.p_adapt_env = new double(*land[(pup -> x)][(pup -> y)]->theta);
			pup->traits.fitClim =  new double(1.0); //I believe this is correct, so line above is actually useless
			//*pup->traits.fitClim = exp(-pow( ((*pup->traits.p_adapt_env)-(*land[(pup -> x)][(pup -> y)]->theta)), 2.0) / (2.0 * para.width[0])); 
		}

		if (para.disp_evol[0]) {
			pup->traits.em = new double[2 * para.L];
			pup->traits.g_em = new double(0.0);
			pup->traits.p_em = new double(0.0);

			for (int i = 0; i < para.L; i++) {
				rdn = unint(rdgen);
				rdn2 = unint(rdgen);
				pup->traits.em[i * 2] = mom.traits.em[i * 2 + rdn];
				pup->traits.em[i * 2 + 1] = dad.traits.em[i * 2 + rdn2];
				*pup->traits.g_em += pup->traits.em[i * 2] + pup->traits.em[i * 2 + 1];
			}

			*pup->traits.p_em = *pup->traits.g_em;
			if (*pup->traits.p_em < 0.0) *pup->traits.p_em = 0.0;
			if (*pup->traits.p_em > 1.0) *pup->traits.p_em = 1.0;
		}
		else pup->traits.p_em = new double(para.em_mean[0]);
	}
	else { //Predator
		if (para.disp_evol[1]) {
			pup->traits.em = new double[2 * para.L];
			pup->traits.g_em = new double(0.0);
		}
		pup->traits.p_em = new double(0.0);

		if (para.disp_evol[1]) {
			for (int i = 0; i < para.L; i++) {
				rdn = unint(rdgen);
				rdn2 = unint(rdgen);
				pup->traits.em[i * 2] = mom.traits.em[i * 2 + rdn];
				pup->traits.em[i * 2 + 1] = dad.traits.em[i * 2 + rdn2];
				*pup->traits.g_em += pup->traits.em[i * 2] + pup->traits.em[i * 2 + 1];
			}
			//Phenotype
			*pup->traits.p_em = *pup->traits.g_em;
			if (*pup->traits.p_em < 0.0) *pup->traits.p_em = 0.0;
			if (*pup->traits.p_em > 1.0) *pup->traits.p_em = 1.0;
		}
		else *pup->traits.p_em = para.em_mean[1];
	}
}
//---------------------------------------------------------------------------
//only for prey
void sample_def_phenotype(Individuals ind, std::normal_distribution<> dist) {

	//for defence
	*ind.traits.p_defence = dist(rdgen);
	if (*ind.traits.p_defence < para.min_def) *ind.traits.p_defence = para.min_def;
	if (*ind.traits.p_defence > para.max_def) *ind.traits.p_defence = para.max_def;



}
//---------------------------------------------------------------------------
void sample_adapt_phenotype(Individuals ind, std::normal_distribution<> dist) {

	//for adaptation to environment
	*ind.traits.p_adapt_env = dist(rdgen);
	*ind.traits.fitClim = exp(-pow( ((*ind.traits.p_adapt_env)-(*land[(ind.x)][(ind.y)]->theta)), 2.0) / (2.0 * para.width[0]));

}

void mutations(int x, int y) {
	int N_mu, N;
	int ind, allele;

	//Prey
	if (community[x][y]->pops[0]->Noffs > 0 && (para.defence_evol || para.adaptation_evol || para.disp_evol[0])) {
		N = community[x][y]->pops[0]->Noffs;
		std::uniform_int_distribution<> unif(0, N - 1);
		std::poisson_distribution<> pois(2.0*(double)para.L * para.mu * (double)N);
		
		if (para.defence_evol) {
			//sample total number of mutations for the defence trait
			N_mu = pois(rdgen);
			for (int i = 0; i < N_mu; i++) {
				ind = unif(rdgen); //sample the individual
				allele = uni_allele(rdgen); //sample the allele

				community[x][y]->pops[0]->Offs[ind].mutation(para, land, 0, allele, mutDef);
			}

			
		}

		if (para.adaptation_evol){
			//sample total number of mutations for adaptation to environment trait
			N_mu = pois(rdgen);
			for (int i = 0; i < N_mu; i++) {
				ind = unif(rdgen); //sample the individual
				allele = uni_allele(rdgen); //sample the allele

				community[x][y]->pops[0]->Offs[ind].mutation(para, land, 2, allele, mutEnv);
			}
		}

		if (para.disp_evol[0]) {
			//sample total number of mutations for the emigration probability trait
			N_mu = pois(rdgen);
			for (int i = 0; i < N_mu; i++) {
				ind = unif(rdgen); //sample the individual
				allele = uni_allele(rdgen); //sample the allele

				community[x][y]->pops[0]->Offs[ind].mutation(para, land, 1, allele, mutEm);
			}
		}
	}

	//Predator
	if (community[x][y]->pops[1]->Noffs > 0 && para.disp_evol[1]) {
		N = community[x][y]->pops[1]->Noffs;
		std::uniform_int_distribution<> unif(0, N - 1);
		std::poisson_distribution<> pois(2.0*(double)para.L * para.mu * (double)N);

		//sample total number of mutations for the emigration probability trait
		N_mu = pois(rdgen);
		for (int i = 0; i < N_mu; i++) {
			ind = unif(rdgen); //sample the individual
			allele = uni_allele(rdgen); //sample the allele

			community[x][y]->pops[1]->Offs[ind].mutation(para, land, 1, allele, mutEm);
		}

	}
}
//---------------------------------------------------------------------------
//Dispersal and viability selection
void dispersal(void) {
	int new_x, new_y;
	double x_rand, y_rand;
	double R1, dist, rndAngle;
	double surv; //survival probability
	vector<Individuals>::iterator iter;

	std::uniform_real_distribution<> unireal_disp(0.0, 0.999);
	std::uniform_real_distribution<> unireal_dispB(0.0000001, 1.0);

	for (int x = 0; x < para.x_max; x++) {
		for (int y = 0; y < para.y_max; y++) {
			if (land[x][y]->suitable && land[x][y]->pres == 1) {
				//Prey
				if (community[x][y]->pops[0]->Noffs > 0) {

					//Loop through offspring
					for (iter = community[x][y]->pops[0]->Offs.begin(); iter != community[x][y]->pops[0]->Offs.end(); iter++) {
						//individual disperses
						if (unireal(rdgen) < *iter->traits.p_em) {
							//sample new location
							x_rand = unireal_disp(rdgen);
							y_rand = unireal_disp(rdgen);

							do {
								do {
									R1 = unireal_dispB(rdgen);
									dist = (-1.0 * para.dist[0]) * std::log(R1);
									rndAngle = unireal(rdgen) * 2.0 * PI;
									new_x = (int)(dist * cos(rndAngle) / (double)para.resol + x_rand + x);
									new_y = (int)(dist * sin(rndAngle) / (double)para.resol + y_rand + y);
								} while (new_x == x && new_y == y);
							} while (new_x < 0 || new_x >(para.x_max - 1) || new_y < 0 || new_y >(para.y_max - 1));

							if (land[new_x][new_y]->suitable) {
								iter->x = new_x;
								iter->y = new_y;

#if LOCALMAL == 1
								if (g > para.evo_stop) {
									if (para.var_phen) {
										std::normal_distribution<> phenotype(community[x][y]->pops[0]->def_mean, community[x][y]->pops[0]->def_sd);
										sample_def_phenotype(*iter, phenotype);
									}
									else *iter->traits.p_defence = community[new_x][new_y]->pops[0]->def_mean;
								}
#endif

#if LOCALENV == 1
								if(g > para.evo_stop){
									if(para.var_phen){
										std::normal_distribution<> phenotype(*land[x][y]->theta, community[x][y]->pops[0]->AdaptEnv_sd);
										sample_adapt_phenotype(*iter, phenotype);
									}
									else {
										*iter->traits.p_adapt_env = community[new_x][new_y]->pops[0]->*land[new_x][new_y]->theta;
										*iter.traits.fitClim = exp(-pow( ((*iter.traits.p_adapt_env)-(*land[(new_x)][(new_y)]->theta)), 2.0) / (2.0 * para.width[0]));
									}
								}
#endif
								//individual viability 
								if (para.niche[0]) {
								//if (para.adaptation_evol == false){
									surv = exp(-pow(para.opt[0] - (*land[new_x][new_y]->theta), 2.0) / (2.0 * para.width[0]));
									if (unireal(rdgen) < surv) {

										community[new_x][new_y]->pops[0]->Inds.push_back(*iter);
										community[new_x][new_y]->pops[0]->N++;
//#if LOCALMAL == 0
//										//update trait sums
//										if (para.defence_evol || para.disp_evol[0] || para.disp_evol[1]) {
//											if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
//												stats[new_x][new_y]->compute_sums(para, *iter);
//										}
//#else
//										if (g < para.evo_stop + 1) {
											//update trait sums
											if (para.defence_evol || para.adaptation_evol || para.disp_evol[0] || para.disp_evol[1] || para.polymorphism) {
												if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)) && y>=200 && y<=500)
													stats[new_x][new_y]->compute_sums(para, *iter);
											}
//										}
//#endif
										//output individual
										if (para.ind_out && (g % para.ind_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0))) 
											iter->outInd(para, r, g, &inds);
									}
									else {
										iter->deleteInd();
										Nspecies[0]--;
									}
								}
								else {
									community[new_x][new_y]->pops[0]->Inds.push_back(*iter);
									community[new_x][new_y]->pops[0]->N++;
									//update trait sums
									if (para.defence_evol || para.adaptation_evol ||para.disp_evol[0] || para.disp_evol[1] || para.polymorphism) {
										if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)) && y>=200 && y<=500)
											stats[new_x][new_y]->compute_sums(para, *iter);
									}
									//output individual
									if (para.ind_out && (g % para.ind_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
										iter->outInd(para, r, g, &inds);
								}
							}
							else {
								iter->deleteInd();
								Nspecies[0]--;
							}
						}
						else { //individual does not disperse
							//individual viability
							if (para.niche[0]) {
							//if (para.adaptation_evol == false){	
								surv = exp(-pow(para.opt[0] - (*land[x][y]->theta), 2.0) / (2.0 * para.width[0]));
								if (unireal(rdgen) < surv) {
									community[x][y]->pops[0]->Inds.push_back(*iter);
									community[x][y]->pops[0]->N++;
									//update trait sums
									if (para.defence_evol || para.disp_evol[0] || para.disp_evol[1] || para.polymorphism || para.adaptation_evol) {
										if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)) && y>=200 && y<=500)
											stats[x][y]->compute_sums(para, *iter);
									}
									//output individual
									if (para.ind_out && (g % para.ind_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
										iter->outInd(para, r, g, &inds);
								}
								else {
									iter->deleteInd();
									Nspecies[0]--;
								}
							}
							else {
								community[x][y]->pops[0]->Inds.push_back(*iter);
								community[x][y]->pops[0]->N++;	
//#if LOCALMAL == 0
//								//update trait sums
//								if (para.defence_evol || para.disp_evol[0] || para.disp_evol[1]) {
//									if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
//										stats[x][y]->compute_sums(para, *iter);
//								}
//#else
//								if (g > para.evo_stop + 1) {
									//update trait sums
									if (para.defence_evol || para.disp_evol[0] || para.disp_evol[1] || para.polymorphism || para.adaptation_evol) {
										if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)) && y>=200 && y<=500)
											stats[x][y]->compute_sums(para, *iter);
									}
//								}
//#endif
								//output individual
								if (para.ind_out && (g % para.ind_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
									iter->outInd(para, r, g, &inds);
							}
						}
					}
				}
				//Predator
				if (community[x][y]->pops[1]->Noffs > 0) {
					//Loop through offspring
					for (iter = community[x][y]->pops[1]->Offs.begin(); iter != community[x][y]->pops[1]->Offs.end(); iter++) {
						//individual disperses
						if (unireal(rdgen) < *iter->traits.p_em) {
							//sample new location
							x_rand = unireal_disp(rdgen);
							y_rand = unireal_disp(rdgen);

							do {
								do {
									R1 = unireal_dispB(rdgen);
									dist = (-1.0 * para.dist[1]) * std::log(R1);
									rndAngle = unireal(rdgen) * 2.0 * PI;
									new_x = (int)(dist * cos(rndAngle) / (double)para.resol + x_rand + x);
									new_y = (int)(dist * sin(rndAngle) / (double)para.resol + y_rand + y);
								} while (new_x == x && new_y == y);
							} while (new_x < 0 || new_x >(para.x_max - 1) || new_y < 0 || new_y >(para.y_max - 1));

							if (land[new_x][new_y]->suitable) {
								iter->x = new_x;
								iter->y = new_y;
								//individual viability 
								if (para.niche[1]) {
									
									surv = exp(-pow(para.opt[1] - (*land[new_x][new_y]->theta_predator), 2.0) / (2.0 * para.width[1]));
								
									if (unireal(rdgen) < surv) {
										community[new_x][new_y]->pops[1]->Inds.push_back(*iter);
										community[new_x][new_y]->pops[1]->N++;
										//update trait sums
										if (para.defence_evol || para.adaptation_evol || para.disp_evol[0] || para.disp_evol[1] || para.polymorphism) {
											if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)) && y>=200 && y<=500)
												stats[new_x][new_y]->compute_sums(para, *iter);
										}
										//output individual
										//Not outputting predators for now as they are not evolving
										//if (para.ind_out && (g % para.ind_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
											//iter->outInd(para, r, g, &inds);
									}
									else {
										iter->deleteInd();
										Nspecies[1]--;
									}
								}
								else {
									community[new_x][new_y]->pops[1]->Inds.push_back(*iter);
									community[new_x][new_y]->pops[1]->N++;
									//update trait sums
									if (para.defence_evol || para.adaptation_evol || para.disp_evol[0] || para.disp_evol[1] || para.polymorphism) {
										if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)) && y>=200 && y<=500)
											stats[new_x][new_y]->compute_sums(para, *iter);
									}
									//output individual
									//Not outputting predators for now as they are not evolving
									//if (para.ind_out && (g % para.ind_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
										//iter->outInd(para, r, g, &inds);
								}
							}
							else {
								iter->deleteInd();
								Nspecies[1]--;
							}
						}
						else { //individual does not disperse
							//individual viability
							if (para.niche[1]) {
								surv = exp(-pow(para.opt[1] - (*land[x][y]->theta_predator), 2.0) / (2.0 * para.width[1]));
								if (unireal(rdgen) < surv) {
									community[x][y]->pops[1]->Inds.push_back(*iter);
									community[x][y]->pops[1]->N++;
									//update trait sums
									if (para.defence_evol || para.adaptation_evol ||para.disp_evol[0] || para.disp_evol[1] || para.polymorphism) {
										if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0))&& y>=200 && y<=500)
											stats[x][y]->compute_sums(para, *iter);
									}
									//output individual
									//Not outputting predators for now as they are not evolving
									//if (para.ind_out && (g % para.ind_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
										//iter->outInd(para, r, g, &inds);
								}
								else {
									iter->deleteInd();
									Nspecies[1]--;
								}
							}
							else {
								community[x][y]->pops[1]->Inds.push_back(*iter);
								community[x][y]->pops[1]->N++;
								//update trait sums
								if (para.defence_evol || para.adaptation_evol || para.disp_evol[0] || para.disp_evol[1] || para.polymorphism) {
									if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)) && y>=200 && y<=500)
										stats[x][y]->compute_sums(para, *iter);
								}
								//output individual
								//Not outputting predators for now as they are not evolving
								//if (para.ind_out && (g % para.ind_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
									//iter->outInd(para, r, g, &inds);
							}
						}
					}
				}
				community[x][y]->pops[0]->Noffs = 0;
				community[x][y]->pops[0]->Offs.clear();
				community[x][y]->pops[1]->Noffs = 0;
				community[x][y]->pops[1]->Offs.clear();
			}
		}
	}

	//update species presence
	for (int x = 0; x < para.x_max; x++) {
		for (int y = 0; y < para.y_max; y++) {

			if (land[x][y]->suitable) {

				//sum up suitable and occupied cells
				if (para.occ_int != 0 && (g % para.occ_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
					occ_stats.sum_suitable(x, y, land, community[x][y]->pops[0]->N, community[x][y]->pops[1]->N, para);

				if (community[x][y]->pops[0]->N == 0 && community[x][y]->pops[1]->N == 0) {
					land[x][y]->pres = 0;
					Total_Cells--;
				}
				else {
					land[x][y]->pres = 1;
//#if LOCALMAL == 0
//					//update and output trait stats
//					if (para.defence_evol || para.disp_evol[0] || para.disp_evol[1]) {
//						if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0))){
//							if (community[x][y]->pops[0]->N > 0) stats[x][y]->compute_stats(para, *community[x][y]->pops[0]);
//							if (community[x][y]->pops[1]->N > 0) stats[x][y]->compute_stats(para, *community[x][y]->pops[1]);
//							stats[x][y]->outStats(r, g, x, y, para, &trait);
//						}
//					}
//#else
//					if (g < para.evo_stop + 1) {
						//update and output trait stats
						if (para.defence_evol || para.disp_evol[0] || para.disp_evol[1] || para.polymorphism || para.adaptation_evol) {
							//ADDED Y CONDITION HERE TOO
							if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)) && y<=500 && y>=200) {
								if (community[x][y]->pops[0]->N > 0) stats[x][y]->compute_stats(para, *community[x][y]->pops[0]);
								if (community[x][y]->pops[1]->N > 0) stats[x][y]->compute_stats(para, *community[x][y]->pops[1]);
								stats[x][y]->outStats(r, g, x, y, para, &trait);
							}
							//if (g == para.evo_stop) {
							//	community[x][y]->pops[0]->def_mean = *stats[x][y]->defence_mean;
							//	community[x][y]->pops[0]->def_sd = *stats[x][y]->defence_sd;
							//}
						}
//					}
//
//#endif
					//output populations
					//if (para.pop_int != 0 && (g % para.pop_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0))) I CHANGED HERE, RIFLETTICI
					if (para.pop_int != 0 && (g % para.pop_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)) && y<=500 && y>=200)
						community[x][y]->outPop(r, g, *land[x][y]->theta, &pops);
				}
//#if LOCALMAL == 0
//				//set cell stats to zero
//				if (para.defence_evol || para.disp_evol[0] || para.disp_evol[1]) {
//					if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
//						stats[x][y]->set2zero(para);
//				}
//#else
//				if (g < para.evo_stop + 1) {
					//set cell stats to zero
					if (para.defence_evol || para.disp_evol[0] || para.disp_evol[1] || para.polymorphism || para.adaptation_evol) {
						if (para.stat_int != 0 && (g % para.stat_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0)))
							stats[x][y]->set2zero(para);
					}
//				}
//#endif
			}
		}
	}

	if (para.occ_int != 0 && (g % para.occ_int == 0 || (g > para.shift_start && (g < para.shift_end + 100) && g % 20 == 0))) {
		//compute occupancy
		occ_stats.compute_occupancy();
		//output occupancy
		occ_stats.outStats(r, g, para, &occup);
		//set occupancy to zero
		occ_stats.set2zero();
	}
}

//----------------------------------------------------------------------------------------
void outInd_header(void) {
	string name = dirOut + "Inds.txt";
	inds.open(name.c_str());

	inds << "rep\tgen\tx\ty\tspp\tdefence";
	if (para.disp_evol[0] || para.disp_evol[1]) inds << "\temig";
	inds << endl;
}
//----------------------------------------------------------------------------------------
void outPop_header(void) {
	string name = dirOut + "Pops.txt";
	pops.open(name.c_str());

	pops << "rep\tgen\tx\ty\ttheta\tN0\tN1" << endl;
}
//----------------------------------------------------------------------------------------
void outStat_header(void) {
	string name = dirOut + "Traits.txt";
	trait.open(name.c_str());

	trait << "rep\tgen\tx\ty";
	if (para.defence_evol) {
		if (para.polymorphism) trait << "\tmean_def";
		else trait << "\tmean_def\tsd_def\tmin_def\tmax_def";
	}
	else {
		if (para.polymorphism) trait << "\tmean_def";
	}
	if(para.adaptation_evol){
		trait << "\tmean_adapt_env\tfitClim_mean\tfitClim_sd";
	}
	else{
		trait << "\tmean_adapt_env";
	}
	if (para.disp_evol[0]) trait << "\tmean_preyEm\tsd_preyEm\tmin_preyEm\tmax_preyEm";
	if (para.disp_evol[1]) trait << "\tmean_predEm\tsd_predEm\tmin_predEm\tmax_predEm";
	trait << endl;
}
//----------------------------------------------------------------------------------------
void outOccupancy_header(void) {
	string name = dirOut + "Occupancy.txt";
	occup.open(name.c_str());

	occup << "rep\tgen\tprey_suit\tprey_occ";
	if (para.niche[0]) occup << "\tprey_occ25\tprey_occ50\tprey_occ75";
	occup << "\tpred_suit\tpred_occ";
	if (para.niche[1]) occup << "\tpred_occ25\tpred_occ50\tpred_occ75";
	occup << endl;
}