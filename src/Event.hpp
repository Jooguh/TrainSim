// Kenny Park


#ifndef EVENT_HPP
#define EVENT_HPP

#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <map>
#include <queue>
#include <vector>
#include "Train.hpp"
#include "Crew.hpp"

class Event {
	public:
		enum events_t {unload_start, unload_end, train_arrival, crew_summon, crew_arrival, crew_hog};
		Event();
		
		Event(events_t e, int ID, double eventTime, Train& subject);
		static int global_id;

		void setEventType(events_t e);
		void setID(int i);
		void setTime(double t);
		
		void setSubject(Train& subject);

		void announceTrainArrival() const;
		void announceCrewSummon() const;
		void announceCrewArrival() const;
		void announceCrewHog() const;
		void printEvent() const;

		Train* getSubject() const;
	
		
		int getID() const;
		double getTime() const;
		events_t getEventType() const;

	private:
		events_t eType;
		Train* eventSubject = nullptr;
		int id;
		double time;

};

#endif
