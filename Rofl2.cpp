// ROFL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cmath>
#include <list>
#include <fstream>
#include <set>


const int NEURONS = 1000;
const int DIMENSIONS = 2;
const int EXPAMPLES = 2000;
const double INITSIGMA = 1;

class ROFL {
private:
	int CountDimensions;
	int CountNeurons;
	int p = 2.0;
	int NumberNeurons = 0;
	double StepsizeSigma = 0.01;
	double StepsizeCenter = 0.01;
	double Centers[NEURONS][DIMENSIONS];
	double Sigmas[NEURONS];
	double TrainingExamples[EXPAMPLES][DIMENSIONS];

public:
	explicit ROFL(int countDimensions, int countNeurons) {
		CountDimensions = countDimensions;
		CountNeurons = countNeurons;
	}

	double InitSigma()
	{
		return INITSIGMA;
	}

	double MeanSigma()
	{
		double sumSigma = 0;

		for (int neuron = 0; neuron < NumberNeurons; neuron++)
		{
			sumSigma += Sigmas[neuron];
		}

		return sumSigma / NumberNeurons;
	}

	double Distance(int neuron, int pattern)
	{

		double temp[10];

		for (int dimension = 0; dimension < CountDimensions; dimension++)
		{
			temp[dimension] = (Centers[neuron][dimension] - TrainingExamples[pattern][dimension])*(Centers[neuron][dimension] - TrainingExamples[pattern][dimension]);
		}

		double sum = 0;
		for (int dimension = 0; dimension < CountDimensions; dimension++)
		{
			sum += temp[dimension];
		}


		return sqrt(sum);
	}

	void AdaptNeuron(int pattern, int acceptingNeuron)
	{
		double distance = Distance(acceptingNeuron, pattern);

		for (int dimension = 0; dimension < CountDimensions; dimension++)
		{
			Centers[acceptingNeuron][dimension] = Centers[acceptingNeuron][dimension] + StepsizeCenter * (TrainingExamples[pattern][dimension] - Centers[acceptingNeuron][dimension]);
		}

		Sigmas[acceptingNeuron] = Sigmas[acceptingNeuron] + StepsizeSigma * (distance - Sigmas[acceptingNeuron]);
	}

	void CreateNewNeuron(int pattern)
	{
		for (int dimension = 0; dimension < CountDimensions; dimension++)
		{
			Centers[NumberNeurons][dimension] = TrainingExamples[pattern][dimension];
		}

		if(NumberNeurons == 0)
		{
			Sigmas[NumberNeurons] = InitSigma();
		}
		else
		{
			Sigmas[NumberNeurons] = MeanSigma();
		}


		NumberNeurons++;
	}

	std::list<int> FindAcceptingNeurons(int pattern, std::list<int> acceptingNeurons)
	{
		for (int neuron = 0; neuron < NumberNeurons; neuron++)
		{
			double radius = p * Sigmas[neuron];
			double tempSum = 0;
			for(int dimension = 0; dimension < CountDimensions;dimension++)
			{
				tempSum += pow(TrainingExamples[pattern][dimension] - Centers[neuron][dimension],2);
			}
			if(sqrt(tempSum) < radius)
			{
				acceptingNeurons.push_back(neuron);
			}
		}

		return acceptingNeurons;
	}

	int FindWinner(int pattern, std::list<int> acceptingNeurons)
	{
		int closestNeuron = 0;
				
		double minDistance = -1;

		for(int acceptingNeuron: acceptingNeurons)
		{
			double currentDistance = Distance(acceptingNeuron, pattern);

			if(currentDistance <minDistance || minDistance==-1)
			{
				minDistance = currentDistance;
				closestNeuron = acceptingNeuron;
			}
		}
		return closestNeuron;
	}

	std::list<int> FindOverlappingNeurons(int selectedNeuron)
	{
		std::list<int> overlappingNeurons;
		double radiusSelectedNeuron = p * Sigmas[selectedNeuron];

		for (int neuron = 0; neuron < NumberNeurons; neuron++)
		{
			if (neuron != selectedNeuron)
			{
				double radius = p * Sigmas[neuron];
				double tempSum = 0;
				for (int dimension = 0; dimension < CountDimensions; dimension++)
				{
					tempSum += pow(Centers[selectedNeuron][dimension] - Centers[neuron][dimension], 2);
				}
				if (sqrt(tempSum) <= radius + radiusSelectedNeuron)
				{
					overlappingNeurons.push_back(neuron);
				}
			}
		}

		return overlappingNeurons;
	}

	void Train(double trainingExamples[EXPAMPLES][DIMENSIONS], int numberExamples)
	{

		for (int i = 0; i<EXPAMPLES; i++)
		{
			memcpy(&TrainingExamples[i], &trainingExamples[i], sizeof(trainingExamples[0]));
		}

		for (int pattern = 0; pattern < numberExamples; pattern++)
		{
			std::list<int> acceptingNeurons;

			acceptingNeurons = FindAcceptingNeurons(pattern, acceptingNeurons);

			if(acceptingNeurons.size() == 0)
			{
				if(NumberNeurons < NEURONS)
				{
					CreateNewNeuron(pattern);
				}
			}
			else if(acceptingNeurons.size() == 1)
			{	
				AdaptNeuron(pattern, acceptingNeurons.back());
			}
			else
			{
				int closestNeuron = FindWinner(pattern, acceptingNeurons);
				AdaptNeuron(pattern, closestNeuron);
			}

		}
	}

	void GnuPlot()
	{
		std::ofstream file("centers.dat", std::ofstream::out | std::ofstream::trunc);
		std::ofstream fileNeighbours("Neighbours.dat", std::ofstream::out | std::ofstream::trunc);
		if (file.is_open() && fileNeighbours.is_open()) {
			for (int neuron = 0; neuron < NumberNeurons; neuron++)
			{
				double radius = p * Sigmas[neuron];

				for (int dimension = 0; dimension < CountDimensions; dimension++)
				{
					file << Centers[neuron][dimension] <<" ";
				}

				file << radius;

				file << "\n";

				std::list<int> overlappingNeurons = FindOverlappingNeurons(neuron);
				for (int overlappingNeuron : overlappingNeurons)
				{
					for (int dimension = 0; dimension < CountDimensions; dimension++)
					{
						fileNeighbours << Centers[neuron][dimension] << " ";
					}
					for (int dimension = 0; dimension < CountDimensions; dimension++)
					{
						fileNeighbours << Centers[overlappingNeuron][dimension] - Centers[neuron][dimension] << " ";
					}
					fileNeighbours << "\n";
				}
			}
		}
		file.close();
		fileNeighbours.close();

		std::ofstream filePlot("plotROFL.gnuplot", std::ofstream::out | std::ofstream::trunc);
		if (filePlot.is_open()) {
			filePlot << "set title \"ROFL\"" << "\n" << "plot \"centers.dat\"  using 1:2:3 with circles  title \"ROFL\", \\" << "\n" << "\"Neighbours.dat\"  using 1:2:3:4 with vectors, \\" << "\n" << "\"random_train.dat\" using 1:2 with points title \"input\"" ;
		}
		filePlot.close();
	}

	

};

