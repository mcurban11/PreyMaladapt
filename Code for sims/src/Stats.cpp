#include "Stats.h"


trait_stats::trait_stats(Parameters para) {

	if (para.defence_evol) {
		//for defence trait
		defence_mean = new double(0.0);
		defence_sd = new double(0.0);
		defence_min = new double(1.0);
		defence_max = new double(0.0);
		
	}
	else {
		if (para.polymorphism) defence_mean = new double(0.0);
	}
	if (para.adaptation_evol){
		//for adapt to environment trait
		adapt_env_mean = new double(0.0);
		adapt_env_sd = new double(0.0);
		adapt_env_min = new double(1.0);
		adapt_env_max = new double(0.0);
		fitClim_mean = new double(0.0);
		fitClim_sd = new double(0.0);
	}
	else{
		adapt_env_mean = new double(0.0);
		fitClim_mean = new double(0.0);
	}

	if (para.disp_evol[0]) {
		prey_emig_mean = new double(0.0);
		prey_emig_sd = new double(0.0);
		prey_emig_min = new double(1.0);
		prey_emig_max = new double(0.0);
	}
	if (para.disp_evol[1]) {
		pred_emig_mean = new double(0.0);
		pred_emig_sd = new double(0.0);
		pred_emig_min = new double(1.0);
		pred_emig_max = new double(0.0);
	}
}
//--------------------------------------------
void trait_stats::compute_sums(Parameters para, Individuals ind) {

	if (ind.spID == 0) {
		if (para.defence_evol) {
			if (para.polymorphism) {
				*defence_mean += (double)(*ind.traits.defence_poly); //sum
			}
			else {
				//for the defence trait
				*defence_mean += (*ind.traits.p_defence); //sum
				*defence_sd += (*ind.traits.p_defence * (*ind.traits.p_defence)); //sum of square
				if (*ind.traits.p_defence < *defence_min) *defence_min = *ind.traits.p_defence;
				if (*ind.traits.p_defence > *defence_max) *defence_max = *ind.traits.p_defence;
				
				
			}
		}
		else {
			if (para.polymorphism) {
				*defence_mean += (double)(*ind.traits.defence_poly); //sum
			}
		}
		if (para.adaptation_evol){
			//for adaptation to the environment trait
			*adapt_env_mean += (*ind.traits.p_adapt_env); //sum
			//*adapt_env_sd += (*ind.traits.p_adapt_env * (*ind.traits.p_adapt_env)); //sum of square
			//for fitness
			*fitClim_mean += (*ind.traits.fitClim);
			*fitClim_sd += (*ind.traits.fitClim * (*ind.traits.fitClim));
		}
	}

	if (ind.spID == 0 && para.disp_evol[0]) {
		*prey_emig_mean += (*ind.traits.p_em);
		*prey_emig_sd += (*ind.traits.p_em * (*ind.traits.p_em));
		if (*ind.traits.p_em < *prey_emig_min) *prey_emig_min = *ind.traits.p_em;
		if (*ind.traits.p_em > *prey_emig_max) *prey_emig_max = *ind.traits.p_em;
	}
	if (ind.spID == 1 && para.disp_evol[1]) {
		*pred_emig_mean += (*ind.traits.p_em);
		*pred_emig_sd += (*ind.traits.p_em * (*ind.traits.p_em));
		if (*ind.traits.p_em < *pred_emig_min) *pred_emig_min = *ind.traits.p_em;
		if (*ind.traits.p_em > *pred_emig_max) *pred_emig_max = *ind.traits.p_em;
	}
}
//-------------------------------------------
void trait_stats::compute_stats(Parameters para, Population pop) {
	if (pop.spID == 0) {
		if (para.defence_evol) {
			if (para.polymorphism) {
				*defence_mean /= (double)pop.N;
			}
			else {
				//stats for defence
				*defence_sd = sqrt((*defence_sd - (*defence_mean * (*defence_mean)) / pop.N) / pop.N);
				*defence_mean /= pop.N;
				
			}
		}
		else {
			if (para.polymorphism) {
				*defence_mean /= (double)pop.N;
			}
		}
		if(para.adaptation_evol){
			//stats for adaptation to environment
			//*adapt_env_sd = sqrt((*adapt_env_sd - (*adapt_env_mean * (*adapt_env_mean)) / pop.N) / pop.N);
			*adapt_env_mean /= pop.N;
			//for fitness
			*fitClim_sd = sqrt((*fitClim_sd - (*fitClim_mean * (*fitClim_mean)) / pop.N) / pop.N);
			*fitClim_mean /= pop.N;  
		}
	}
	if (pop.spID == 0 && para.disp_evol[0]) {
		*prey_emig_sd = sqrt((*prey_emig_sd - (*prey_emig_mean * (*prey_emig_mean)) / pop.N) / pop.N);
		*prey_emig_mean /= pop.N;
	}
	if (pop.spID == 1 && para.disp_evol[1]) {
		*pred_emig_sd = sqrt((*pred_emig_sd - (*pred_emig_mean * (*pred_emig_mean)) / pop.N) / pop.N);
		*pred_emig_mean /= pop.N;
	}
}
//--------------------------------------------
void trait_stats::outStats(int r, int g, int x, int y, Parameters para, std::ofstream *out) {
	*out << r << "\t" << g << "\t" << x << "\t" << y; 
	if (para.defence_evol) {
		if (para.polymorphism) *out << "\t" << *defence_mean;
		else *out << "\t" << *defence_mean << "\t" << *defence_sd << "\t" << *defence_min << "\t" << *defence_max;
	}
	else {
		if (para.polymorphism) *out << "\t" << *defence_mean;
	}
	if (para.adaptation_evol){
		*out << "\t" << *adapt_env_mean << "\t" << *fitClim_mean << "\t" << *fitClim_sd;
	}
	else{
		*out << "\t" << *adapt_env_mean;
	}
	if (para.disp_evol[0]) *out << "\t" << *prey_emig_mean << "\t" << *prey_emig_sd << "\t" << *prey_emig_min << "\t" << *prey_emig_max;
	if (para.disp_evol[1]) *out << "\t" << *pred_emig_mean << "\t" << *pred_emig_sd << "\t" << *pred_emig_min << "\t" << *pred_emig_max;
	*out << endl;
}
//--------------------------------------------
void trait_stats::set2zero(Parameters para) {
	if (para.defence_evol) {
		*defence_mean = 0.0;
		*defence_sd = 0.0;
		*defence_min = 0.0;
		*defence_max = 0.0;

		
	}
	else {
		if (para.polymorphism) *defence_mean = 0.0;
	}
	if (para.adaptation_evol){
		*adapt_env_mean = 0.0;
		*adapt_env_sd = 0.0;
		*adapt_env_min = 0.0;
		*adapt_env_max = 0.0;
		*fitClim_mean = 0.0;
		*fitClim_sd = 0.0;
	}

	if (para.disp_evol[0]) {
		*prey_emig_mean = 0.0;
		*prey_emig_sd = 0.0;
		*prey_emig_min = 0.0;
		*prey_emig_max = 0.0;
	}
	if (para.disp_evol[1]) {
		*pred_emig_mean = 0.0;
		*pred_emig_sd = 0.0;
		*pred_emig_min = 0.0;
		*pred_emig_max = 0.0;
	}
}

//--------------------------------------------
void trait_stats::delete_stats(Parameters para) {
	if (para.defence_evol) {
		delete defence_mean;
		delete defence_sd;
		delete defence_min;
		delete defence_max;

		
	}
	else {
		if (para.polymorphism) delete defence_mean;
	}
	if(para.adaptation_evol){
		delete adapt_env_mean;
		delete adapt_env_sd;
		delete adapt_env_min;
		delete adapt_env_max;
		delete fitClim_mean;
		delete fitClim_sd;
	}

	if (para.disp_evol[0]) {
		delete prey_emig_mean;
		delete prey_emig_sd;
		delete prey_emig_min;
		delete prey_emig_max;
	}
	if (para.disp_evol[1]) {
		delete pred_emig_mean;
		delete pred_emig_sd;
		delete pred_emig_min;
		delete pred_emig_max;
	}
}

//--------------------------------------------
occupancy_stats::occupancy_stats(void) {
	suit = new int[2]; suit[0] = 0; suit[1] = 0;
	suit10 = new int[2]; suit10[0] = 0; suit10[1] = 0;
	suit20 = new int[2]; suit20[0] = 0; suit20[1] = 0;
	suit50 = new int[2]; suit50[0] = 0; suit50[1] = 0;

	pres = new int[2]; pres[0] = 0; pres[1] = 0;
	pres10 = new int[2]; pres10[0] = 0; pres10[1] = 0;
	pres20 = new int[2]; pres20[0] = 0; pres20[1] = 0;
	pres50 = new int[2]; pres50[0] = 0; pres50[1] = 0;

	occ = new double[2]; occ[0] = 0.0; occ[1] = 0.0;
	occ10 = new double[2]; occ10[0] = 0.0; occ10[1] = 0.0;
	occ20 = new double[2]; occ20[0] = 0.0; occ20[1] = 0.0;
	occ50 = new double[2]; occ50[0] = 0.0; occ50[1] = 0.0;
}

//--------------------------------------------
void occupancy_stats::sum_suitable(int x, int y, Landscape ***arena, int Nprey, int Npred, Parameters para) {
	double surv;
	
	//if cell is suitable
	if (arena[x][y]->suitable) {
		//Prey
		suit[0]++;
		if (Nprey > 0) pres[0]++;
		if (para.niche[0]) {
			surv = exp(-pow(para.opt[0] - (*arena[x][y]->theta), 2.0) / (2.0 * para.width[0]));
			if (surv > 0.75) { 
				suit10[0]++; suit20[0]++; suit50[0]++; 
				if (Nprey > 0) { pres10[0]++; pres20[0]++; pres50[0]++; }
			}
			if (surv > 0.5 && surv < 0.75) { 
				suit10[0]++; suit20[0]++; 
				if (Nprey > 0) { pres10[0]++; pres20[0]++; }
			}
			if (surv > 0.25 && surv < 0.5) {
				suit10[0]++;
				if (Nprey > 0) pres10[0]++;
			}
		}
		//Predator
		suit[1]++;
		if (Npred > 0) pres[1]++;
		if (para.niche[1]) {
			surv = exp(-pow(para.opt[1] - (*arena[x][y]->theta), 2.0) / (2.0 * para.width[1]));
			if (surv > 0.75) { 
				suit10[1]++; suit20[1]++; suit50[1]++; 
				if (Npred > 0) { pres10[1]++; pres20[1]++; pres50[1]++; }
			}
			if (surv > 0.5 && surv < 0.75) { 
				suit10[1]++; suit20[1]++; 
				if (Npred > 0) { pres10[1]++; pres20[1]++; }
			}
			if (surv > 0.25 && surv < 0.5) {
				suit10[1]++;
				if (Npred > 0) pres10[1]++;
			}
		}
	}
}

//--------------------------------------------
void occupancy_stats::compute_occupancy(void) {
	//Prey
	if (suit[0] > 0) occ[0] = (double)pres[0] / (double)suit[0];
	if (suit10[0] > 0) occ10[0] = (double)pres10[0] / (double)suit10[0];
	if (suit20[0] > 0) occ20[0] = (double)pres20[0] / (double)suit20[0];
	if (suit50[0] > 0) occ50[0] = (double)pres50[0] / (double)suit50[0];
	//Predator
	if (suit[1] > 0) occ[1] = (double)pres[1] / (double)suit[1];
	if (suit10[1] > 0) occ10[1] = (double)pres10[1] / (double)suit10[1];
	if (suit20[1] > 0) occ20[1] = (double)pres20[1] / (double)suit20[1];
	if (suit50[1] > 0) occ50[1] = (double)pres50[1] / (double)suit50[1];
}

//--------------------------------------------
void occupancy_stats::set2zero(void) {
	suit[0] = 0; suit[1] = 0;
	suit10[0] = 0; suit10[1] = 0;
	suit20[0] = 0; suit20[1] = 0;
	suit50[0] = 0; suit50[1] = 0;

	pres[0] = 0; pres[1] = 0;
	pres10[0] = 0; pres10[1] = 0;
	pres20[0] = 0; pres20[1] = 0;
	pres50[0] = 0; pres50[1] = 0;

	occ[0] = 0.0; occ[1] = 0.0;
	occ10[0] = 0.0; occ10[1] = 0.0;
	occ20[0] = 0.0; occ20[1] = 0.0;
	occ50[0] = 0.0; occ50[1] = 0.0;
}

//--------------------------------------------
void occupancy_stats::outStats(int r, int g, Parameters para, std::ofstream *out) {
	*out << r << "\t" << g << "\t" << suit[0] << "\t" << occ[0];
	if (para.niche[0]) *out << "\t" << occ10[0] << "\t" << occ20[0] << "\t" << occ50[0];
	*out << "\t" << suit[1] << "\t" << occ[1];
	if (para.niche[1]) *out << "\t" << occ10[1] << "\t" << occ20[1] << "\t" << occ50[1];
	*out << endl;
}

//--------------------------------------------
void occupancy_stats::delete_stats(void) {
	delete[] suit;
	delete[] suit10;
	delete[] suit20;
	delete[] suit50;

	delete[] pres;
	delete[] pres10;
	delete[] pres20;
	delete[] pres50;

	delete[] occ;
	delete[] occ10;
	delete[] occ20;
	delete[] occ50;
}