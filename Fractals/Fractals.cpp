#include "ecrireimage.h"
#include <iostream>

/* Headers */ 
void test();
double borneDansIntervalle(const double valeurABorner, const double borneInferieure, const double borneSuperieure);

int main()
{	
	std::cout << "Fractales par IFS" << std::endl;

	test();	

	std::cin.get();
}

/* Function that calls all other tests */
void test()
{
	std::cout << "Starting tests..." << std::endl;

	int valeurABornerInferieure = -1;
	int valeurABornerSuperieure = 4;
	int borneInferieure = 1;
	int borneSuperieure = 3;

	std::cout << "Testing borneDansIntervalle: " << std::endl;
	std::cout << "Expected values: 1 1 1 2 3 3" << std::endl;
	std::cout << "Test values    : ";
	for (int i = valeurABornerInferieure; i <= valeurABornerSuperieure; i++)
	{
		std::cout << borneDansIntervalle(static_cast<double>(i), borneInferieure, borneSuperieure) << " ";
	}

}

//! If the valeurABorner is inside the given interval, then returns the unchanged `valeurABorner`, otherwise return the closest limit (borne)
	/*!	  
	  \param valeurABorner the value to limit in the interval.
	  \param borneInferieure interval inclusive lower limit.
	  \param borneSuperieure interval inclusive upper limit.
      \return value that was capped, between this interval.
	*/
double borneDansIntervalle(const double valeurABorner,const double borneInferieure, const double borneSuperieure)
{
	if (valeurABorner < borneInferieure) return borneInferieure;
	if (valeurABorner > borneSuperieure) return borneSuperieure;
	return valeurABorner;	
}
