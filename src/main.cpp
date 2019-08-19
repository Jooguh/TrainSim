// Kenny Park


#include <iostream>
#include <sstream>
#include <ratio>
#include <string.h>
#include <chrono>
#include <iomanip>
#include <random>
#include <fstream>
#include "Train.hpp"
#include "Crew.hpp"
#include "Dock.hpp"


// Estimate the next event
bool double_equals(double a, double b, double epsilon = 0.00001)
{
	//return std::abs(a - b) < epsilon;
	return std::abs(a - b) <= epsilon;
}

int main(int argc, char* argv[])
{
	double avg_train_arrival_time = 0;
	double total_simulation_time = 0;
	std::vector<std::string> s(argv, argv+argc);
	int time = 0;
	//int test = 0;
	int test = 1;
	int max_trains = 0;
	for(int i = 0; i < s.size(); ++i)
	{
		std::cout << s[i] << std::endl;
	}
	std::cout << s[1] << std::endl;
    
	if(argc == 3)
	{
		avg_train_arrival_time = atof(argv[1]);
		total_simulation_time = atof(argv[2]);

		Dock dock(avg_train_arrival_time);
		
		dock.generateTrainArrivalTimes(total_simulation_time, avg_train_arrival_time);
		dock.generateTrainUnloadTimes();
		dock.platform();
	}

	
	else if(argc == 4 && (strcmp(argv[1], "-s")))
	{
		
		std::queue<double> arrival;
		std::queue<double> unload;
		std::queue<double> hours;
		std::string train_schedule = argv[2];
		std::string crew_schedule = argv[3];
		std::string line;
		std::ifstream train_file (train_schedule);
		if(train_file.is_open())
		{
			
			double a;
			double u;
			double h;
			
			while(std::getline(train_file, line))
			{
				std::istringstream ss(line);
				ss >> a >> u >> h;
				arrival.push(a);
				unload.push(u);
				hours.push(h);
			}

			train_file.close();
		}
		std::queue<double> travel;
		
		std::ifstream crew_file (crew_schedule);
		if(crew_file.is_open())
		{
			double t;
			while(std::getline(crew_file, line))
			{
				std::istringstream ss(line);
				ss >> t;
				travel.push(t);
			}
			std::cout << "\n\n";
			crew_file.close();	
		}
		Dock dock(arrival, unload, hours, travel);

		dock.pre_Platform();

	}
	return 0;
}

