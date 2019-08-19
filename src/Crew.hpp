// Kenny Park

#ifndef CREW_HPP
#define CREW_HPP

#include <iostream>
#include <sstream>
#include <random>
#include <functional>
class Train;

class Crew {

		friend class Dock;
		
	public:
		Crew();
		Crew(double pickWorkTime, double pickArrivalTime, int pickID);
		Crew(int pickID);
		Crew(double pickArrivalTime, int pickID);

		double getWorkTime() const;
		double getArrivalTime() const;
		double getStartTime() const;
		double getRemainingWork() const;
		int getCrewID() const;
		bool getHoggedOut() const;
		bool hasDeparted() const;
		Train* getTrain() const;
		

		void generateWorkTime();
		void generateArrivalTime();
		void setCrewID(int value);
		void setArrivalTime(double a);
		void setWorkTime(double w);
		void updateRemainingWork(double currentTime);
		void setStartTime(double s);
		void setHoggedOutStatus(bool status);
		void setDeparted();
		void setTrain(Train& newTrain);

		std::string toString(void) const;

	private:
		bool is_hogged_out;
		int id;
		double work_time;
		double remaining_work_time;
		double arrival_time;
		double start_time;
		bool departed = false;
		Train *currentTrain = nullptr;
		std::default_random_engine generator;
};

#endif
