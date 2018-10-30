#include "ecrireimage.h"
#include <iostream>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <fstream>

using namespace std;

/* Headers - entête*/
void test();
double borneDansIntervalle(const double valeurABorner, const double borneInferieure, const double borneSuperieure);
bool estDansIntervalle(const double valeurABorner, const double borneInferieure, const double borneSuperieure);
void dessinePoint(const double x, const double y, const Pixel intensite, Pixel image[][tailleY]);
double aleatoireZeroUn();
int aleatoireSelonDistribution(const double *probabilitesCumulatives, int size);
Point2d transformePoint(const double x, const double y, const double *transformation);
void calculerImage();

struct image
{
	// première ligne

	int nombrePointsDepart;
	int transformationsDepart;
	int transformationsAvant;
	int intensite;
	// deuxiéme ligne
	
	double transformation[6];

	// troisiéme ligne et 

	//double transformation[][7];
	//int probabiliteCumulative;	
};

int main()
{	
	cout << "Fractales par IFS" << endl;

	test();	

	calculerImage();

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

	cout << " << Testing borneDansIntervalle >>" << endl;
	cout << "Expected values: 1 1 1 2 3 3" << endl;
	cout << "Test values    : ";
	for (int i = valeurABornerInferieure; i <= valeurABornerSuperieure; i++)
	{
		cout << borneDansIntervalle(static_cast<double>(i), borneInferieure, borneSuperieure) << " ";
	}
	cout << endl << endl;

	cout << " << Testing estDansIntervalle >>" << endl;
	cout << "Expected values: false false true true true false or 0 0 1 1 1" << endl;
	cout << "Test values    : ";
	for (int i = valeurABornerInferieure; i <= valeurABornerSuperieure; i++)
	{
		cout << boolalpha << estDansIntervalle(static_cast<double>(i), borneInferieure, borneSuperieure) << " ";
	}
	cout << endl << endl;

	cout << " << Testing dessinePoint >>" << endl;

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
	cout << " 'imageTest.bmp' generated" << endl;

	// Test aleatoireSelonDistribution
	srand(time_t(NULL));
	double probabilitesCumulatives[] = { 0.1, 0.35, 1.0 };
	int size = 3;
	int occurrencesOfOne = 0;	
	for (int i = 0; i < 1000; i++)
	{
		int interval = aleatoireSelonDistribution(probabilitesCumulatives, size);
		if (interval == 1)
		{
			occurrencesOfOne++;
		}		
	}	
	cout << "Occurrences of the range tested (0.1, 0.35) : " << occurrencesOfOne << endl << endl;

	// Test transformePoint
	cout << " << Testing transformePoint >>" << endl;
	double transformation[] = { 0.5, -0.5, 0.25, 0.75, 2.0, 3.0 };
	Point2d point = transformePoint(0.2, 0.7, transformation);
	cout << "Expected values: x: 1.75, y: 3.575 " << endl;
	cout << "Test values    : x: " << point.x << ", y: " << point.y << endl;
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

//! returns a point transformed
	/*!
	  \param x point x
	  \param y point y
	  \param *transformation un tableau de transformation, it needs to be [3x2], 6 elements
	  \return returns the transformated point
	*/
Point2d transformePoint(const double x, const double y, const double *transformation)
{	
	Point2d point;
	point.x = (transformation[0] * x + transformation[1] * y) + transformation[4];
	point.y = (transformation[2] * x + transformation[3] * y) + transformation[5];
	return point;
}

void calculerImage()
{
	cout << " << Calculer Image >>" << endl;
	ifstream inFile("\\Golden\ dragon.txt");
//	inFile.open("\\Golden\ dragon.txt");

	if (!inFile)
	{
		cerr << "Unable to open file";
		//exit(1);
	}

	image image1;
	while (inFile)
	{
		cin >> image1.nombrePointsDepart;
		cin >> image1.transformationsDepart;
		cin >> image1.transformationsAvant;
		cin >> image1.intensite;
	}
}