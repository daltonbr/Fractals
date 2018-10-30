#include "ecrireimage.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <cstdlib>

using namespace std;

/* Headers - entête*/
void test();
double borneDansIntervalle(const double valeurABorner, const double borneInferieure, const double borneSuperieure);
bool estDansIntervalle(const double valeurABorner, const double borneInferieure, const double borneSuperieure);
void dessinePoint(const double x, const double y, const Pixel intensite, Pixel image[][tailleY]);
double aleatoireZeroUn();
int aleatoireSelonDistribution(const double *probabilitesCumulatives, int size);

int main()
{	
	cout << "Fractales par IFS" << endl;

	test();	

	cin.get();
}

/* Function that calls all other tests */
void test()
{
	cout << "Starting tests..." << endl << endl;

	int valeurABornerInferieure = -1;
	int valeurABornerSuperieure = 4;
	int borneInferieure = 1;
	int borneSuperieure = 3;

	cout << "Testing borneDansIntervalle: " << endl;
	cout << "Expected values: 1 1 1 2 3 3" << endl;
	cout << "Test values    : ";
	for (int i = valeurABornerInferieure; i <= valeurABornerSuperieure; i++)
	{
		cout << borneDansIntervalle(static_cast<double>(i), borneInferieure, borneSuperieure) << " ";
	}
	cout << endl << endl;

	cout << "Testing estDansIntervalle: " << endl;
	cout << "Expected values: false false true true true false or 0 0 1 1 1" << endl;
	cout << "Test values    : ";
	for (int i = valeurABornerInferieure; i <= valeurABornerSuperieure; i++)
	{
		cout << boolalpha << estDansIntervalle(static_cast<double>(i), borneInferieure, borneSuperieure) << " ";
	}
	cout << endl << endl;

	cout << "Testing dessinePoint" << endl;

	/* Fill the image with blank pixels (255) */
	Pixel image[tailleX][tailleY];
	for (unsigned i = 0; i < tailleX; i++)
	{
		for (unsigned j = 0; j < tailleY; j++)
		{
			image[i][j] = 255;
		}
	}

	dessinePoint(10, 10, 255, image);	
	dessinePoint(20, 15, 100, image);
	dessinePoint(30, 20, 128, image);
	dessinePoint(-1, 1, 255, image);
	dessinePoint(500, 1, 255, image);
	dessinePoint(1, -1, 255, image);
	dessinePoint(1, 500, 255, image);
	dessinePoint(30, 20, 128, image);

	ecrireImage(image, "imageTest.bmp");

	// Test aleatoireSelonDistribution
	srand(time(nullptr));
	double probabilitesCumulatives[] = { 0.1, 0.35, 1.0 };
	int size = 3;
	int occurrencesOfOne = 0;
	int occurrencesOfZero = 0;
	int occurrencesOfTwo = 0;
	for (int i = 0; i < 1000; i++)
	{
		int interval = aleatoireSelonDistribution(probabilitesCumulatives, size);
		if (interval == 1)
		{
			occurrencesOfOne++;
		}		
	}	
	cout << "Occurrences of the range tested (0.1, 0.35) : " << occurrencesOfOne << endl;	
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

//! Check if the valeurABorner is inside the given interval
	/*!
	  \param valeurABorner the value to verify if are contained in the interval.
	  \param borneInferieure interval inclusive lower limit.
	  \param borneSuperieure interval inclusive upper limit.
	  \return true when the valeurABorner is inside the given interval, false otherwise.
	*/
bool estDansIntervalle(const double valeurABorner, const double borneInferieure, const double borneSuperieure)
{
	if (valeurABorner < borneInferieure) return false;
	if (valeurABorner > borneSuperieure) return false;
	return true;
}

void dessinePoint(const double x, const double y, const Pixel intensite, Pixel image[][tailleY])
{
	if (x < 0 || x > tailleX - 1) return;
	if (y < 0 || y > tailleY - 1) return;

	image[(unsigned)x][(unsigned)y] = borneDansIntervalle(image[(unsigned)x][(unsigned)y]-intensite, 0, 255);
}

/* returns an uniform distribution between 0.0 and 1.0 (excluded) */
double aleatoireZeroUn()
{	
	//TODO: could be improved using Mersene Twister
	//https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
	return (double)rand() / ((double)RAND_MAX + 1);
}

//! fits our random number according to ranges
	/*!
	  \param *probabilitesCumulatives a pointer to our tableau de probabilités cumulatives
	  \param size the size of our tableau (array)
	  \return returns the index of our interval, -1 for error when you provide values outside of rande 0.0 and 1.0 (excluded)
	*/
int aleatoireSelonDistribution(const double *probabilitesCumulatives, int size)
{
	double aleatoire = aleatoireZeroUn();

	if (aleatoire < probabilitesCumulatives[0])
	{
		return 0;
	}

	for (int i = 1; i < size; i++)
	{	
		if (estDansIntervalle(aleatoire, probabilitesCumulatives[i-1], probabilitesCumulatives[i]))
		{
			return i;
		}
	}
	// error, we should get here	
	return -1;
}