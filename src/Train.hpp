// Kenny Park
// CS 115
// Assignment 1


#ifndef TRAIN_HPP
#define TRAIN_HPP

#include <iostream>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <random>
#include <functional>
//#include "Crew.hpp"

class Crew;

class Train {

		friend class Dock;
		
	public:
		Train();
		Train(int pickID);
		//Train(double pickUnloadTime);
		Train(double pickUnloadTime, int pickID);
		Train(double pickUnloadTime, int pickID, double arrive);
		double getUnloadTime() const;
		double getArrivalTime() const;
		int getTrainID() const;
		int getHoggedCount() const;
		bool hasDeparted() const;
		bool getHoggedStatus() const;
		double getTimeInQueue(double t) const;
		Crew* getTrainCrew() const;
		

		void generateUnloadTime();
		void setUnloadTime(double new_time);
		void setTrainID(int value);
		void setDeparted();
		void setHoggedStatus(bool status);
		void incHoggedCount();
		void setTimeInQueue(double t);
		void setTrainCrew(Crew& newCrew);
		

		void toString() const;

	private:
		double unloadTime;
		double arrivalTime;
		double time_in_queue = 0;
		double time_start_in_queue = 0;
		int hogged_count = 0;
		int id;
		bool departed = false;
		bool crew_hogged = false;
		std::default_random_engine generator;
		Crew *currentCrew;


};

#endif
