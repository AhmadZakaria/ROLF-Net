
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

#include "Eigen/Core"
#include "rolf2.cpp"

void testROLF()
{
    ROLF roflNetwork(2, 2000);

	double** traininExamples;
	traininExamples = new double*[EXPAMPLES];
	for (int i = 0; i < EXPAMPLES; i++)
	{
		traininExamples[i] = new double[DIMENSIONS];
	}

	std::ifstream roflFile("random_train.dat");
	std::string line;
	int count = 0;
	while (std::getline(roflFile, line))
	{
		std::istringstream iss(line);

		for (int dimension = 0; dimension<2; dimension++)
		{
			iss >> traininExamples[count][dimension];

		}
		count++;
	}


	roflNetwork.Train(traininExamples, 1500);
	roflNetwork.GnuPlot();
}

int main()
{
    testROLF();
    return 0;
}

