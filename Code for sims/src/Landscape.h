#pragma once

#include <stdio.h>

class Landscape {
public:
	Landscape();
	int pres;
	bool suitable;
	double local_K, r;
	double *local_K0, *r0; //in case of stochasticity
	double *dev; //Environmental gradient local deviation
	double *ext; //local extinction probability
	double eps; //env. stochasticity
	double *theta; //environmental gradient value  (Atkins & Travis 2010)
	double *theta_predator; //environmental gradient value for the case of the predator responding to a different environment

	void deleteLand(void);
private:
};


