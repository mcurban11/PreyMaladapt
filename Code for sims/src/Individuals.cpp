#include "Individuals.h"

std::random_device rd2;
std::mt19937 gen(rd2());

Individuals::Individuals(int ID, int xx, int yy) {
	alive = true;
	spID = ID;
	x = xx; y = yy;
	xnatal = xx; ynatal = yy;
}
//--------------------------------------------------------------------------
void Individuals::initialise(Parameters para, Landscape ***arena, std::normal_distribution<> distrDef, std::normal_distribution<> distrAdaptEnv, std::normal_distribution<> distrEm, std::uniform_int_distribution<> poly) {
	if (spID == 0) { // prey
		if (para.defence_evol) {
			if (para.polymorphism) {
				traits.defence_poly = new int(poly(gen));
			}
			else {
				//for the defence genetics
				traits.defence = new double[2 * para.L];
				traits.g_defence = new double(0.0);
				traits.p_defence = new double(0.0);

				for (int i = 0; i < (2 * para.L); i++) {
					traits.defence[i] = distrDef(gen);
					*traits.g_defence += traits.defence[i];
				}
				*traits.p_defence = *traits.g_defence;
				if (*traits.p_defence < para.min_def) *traits.p_defence = para.min_def;
				if (*traits.p_defence > para.max_def) *traits.p_defence = para.max_def;

			}
		}
		else {
			if (para.polymorphism)  traits.defence_poly = new int(poly(gen));
			else traits.p_defence = new double(para.def_mean_fixed);
		}

		if (para.adaptation_evol){
			//for the adaptation to the environment genetics
				traits.adapt_env = new double[2 * para.L];
				traits.g_adapt_env = new double(0.0);
				traits.p_adapt_env = new double(0.0);
				traits.fitClim = new double(0.0);

				for (int i = 0; i < (2 * para.L); i++) {
					traits.adapt_env[i] = distrAdaptEnv(gen);
					*traits.g_adapt_env += (traits.adapt_env[i]) ;
				}
				*traits.p_adapt_env = *traits.g_adapt_env;
				//calculate fitness for adaptation to climate
				*traits.fitClim = exp(-pow( ((*traits.p_adapt_env)-(*arena[x][y]->theta)), 2.0) / (2.0 * para.width[0])); 
				
		}
		else{
			traits.p_adapt_env = new double(*arena[x][y]->theta);
			traits.fitClim = new double(0.0);
			//*traits.fitClim = exp(-pow( ((*traits.p_adapt_env)-(*arena[x][y]->theta)), 2.0) / (2.0 * para.width[0])); 
			traits.fitClim = new double(1.0); // is this what Mark wants? I think so, perfect adaptation
		}

		if (para.disp_evol[0]) {
			traits.em = new double[2 * para.L];
			traits.g_em = new double(0.0);
			traits.p_em = new double(0.0);
			for (int i = 0; i < (2 * para.L); i++) {
				traits.em[i] = distrEm(gen);
				*traits.g_em += traits.em[i];
			}
			*traits.p_em = *traits.g_em;
			if (*traits.p_em < 0.0) *traits.p_em = 0.0;
			if (*traits.p_em > 1.0) *traits.p_em = 1.0;
		}
		else traits.p_em = new double(para.em_mean[0]);
	}
	else { //predator
		if (para.disp_evol[1]) {
			traits.em = new double[2 * para.L];
			traits.g_em = new double(0.0);
			traits.p_em = new double(0.0);

			for (int i = 0; i < (2 * para.L); i++) {
				traits.em[i] = distrEm(gen);
				*traits.g_em += traits.em[i];
			}
			*traits.p_em = *traits.g_em;
			if (*traits.p_em < 0.0) *traits.p_em = 0.0;
			if (*traits.p_em > 1.0) *traits.p_em = 1.0;
		}
		else traits.p_em = new double(para.em_mean[1]);
	}
}
//--------------------------------------------------------------------------
void Individuals::mutation(Parameters para, Landscape ***arena, int trait, int allele, std::normal_distribution<> distr) {

	//Pery defence 
	if (trait == 0) {
		if (para.polymorphism) {
			if (*traits.defence_poly == 0) *traits.defence_poly = 1;
			else *traits.defence_poly = 0;
		}
		else {
			//for defence mutation
			*traits.g_defence -= traits.defence[allele];
			traits.defence[allele] += distr(gen);
			*traits.g_defence += traits.defence[allele];
			*traits.p_defence = *traits.g_defence;
			if (*traits.p_defence < para.min_def) *traits.p_defence = para.min_def;
			if (*traits.p_defence > para.max_def) *traits.p_defence = para.max_def;

			
			
		}
	}

	else if (trait==2) //for adaptation to the environment mutation
	{   
		//not assuming polymorphism
		*traits.g_adapt_env -= (traits.adapt_env[allele]);
		traits.adapt_env[allele] += distr(gen);
		*traits.g_adapt_env += traits.adapt_env[allele]; 
		*traits.p_adapt_env = *traits.g_adapt_env;
		//updating fitness for adaptation to climate
		*traits.fitClim = exp(-pow( ((*traits.p_adapt_env)-(*arena[x][y]->theta)), 2.0) / (2.0 * para.width[0])); 
	}
	
	else if(trait==1) { //emigration probability
		*traits.g_em -= traits.em[allele];
		traits.em[allele] += distr(gen);
		*traits.g_em += traits.em[allele];
		*traits.p_em = *traits.g_em;
		if (*traits.p_em < 0.0) *traits.p_em = 0.0;
		if (*traits.p_em > 1.0) *traits.p_em = 1.0;
	}
}
//--------------------------------------------------------------------------
void Individuals::outInd(Parameters para, int r, int g,  std::ofstream *out) {

	*out << r << "\t" << g << "\t" << x << "\t" << y << "\t" << spID;
	if (spID == 0) {
		if (para.polymorphism) *out << "\t" << *traits.defence_poly;
		else *out << "\t" << *traits.p_defence << "\t" << *traits.p_adapt_env << "\t" << *traits.fitClim; 

	}
	else *out << "\t" << "NA";
	if (para.disp_evol[0] || para.disp_evol[1]) {
		if (para.disp_evol[spID]) *out << "\t" << *traits.p_em;
		else  *out << "\t" << "NA";
	}
	*out << endl;
}

//--------------------------------------------------------------------------
void Individuals::deleteInd(void) {

	traits.deleteTraits();

}
