
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

#include "Eigen/Core"
#include "rolf2.cpp"

using namespace std;
namespace helper {
//  Windows
#ifdef _WIN32
#include <Windows.h>
double get_wall_time() {
    LARGE_INTEGER time, freq;
    if (!QueryPerformanceFrequency(&freq)) {
        //  Handle error
        return 0;
    }
    if (!QueryPerformanceCounter(&time)) {
        //  Handle error
        return 0;
    }
    return (double)time.QuadPart / freq.QuadPart;
}
double get_cpu_time() {
    FILETIME a, b, c, d;
    if (GetProcessTimes(GetCurrentProcess(), &a, &b, &c, &d) != 0) {
        //  Returns total user time.
        //  Can be tweaked to include kernel times as well.
        return
            (double)(d.dwLowDateTime |
                     ((unsigned long long)d.dwHighDateTime << 32)) * 0.0000001;
    } else {
        //  Handle error
        return 0;
    }
}

//  Posix/Linux
#else
#include <time.h>
#include <sys/time.h>
double get_wall_time() {
    struct timeval time;
    if (gettimeofday(&time, NULL)) {
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
double get_cpu_time() {
    return (double)clock() / CLOCKS_PER_SEC;
}
#endif
}

template<typename M>
M load_csv (const std::string& path, char del = ',') {
    std::ifstream indata;
    indata.open(path);
    std::string line;
    std::vector<double> values;
    uint rows = 0;
    while (std::getline(indata, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        while (std::getline(lineStream, cell, del)) {
            if (cell.size() > 0)
                values.push_back(std::stod(cell));
        }
        ++rows;
    }
    return Eigen::Map<const Eigen::Matrix<typename M::Scalar, M::RowsAtCompileTime, M::ColsAtCompileTime, Eigen::RowMajor>>(values.data(), rows, values.size() / rows);
}

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

