// Kenny Park


#ifndef DOCK_HPP
#define DOCK_HPP

#include <iostream>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <random>
#include <functional>
#include <map>
#include <queue>
#include <vector>
#include <set>
#include "Train.hpp"
#include "Crew.hpp"
#include "Event.hpp"


struct ComparePQ {
	bool operator()(const Event& e1, const Event& e2) 
	{
		return (e1.getTime() < e2.getTime());
	}


};

class Dock {
	public:
		typedef std::pair<std::string, Train> trainEntry;
		typedef std::map<int, Train*> trainMap;
		typedef std::queue<Train*> trainQueue;

		typedef std::vector<int> hogoutCount;

		typedef std::multimap<double, Event> eventMap;

		typedef std::pair<double, Event> eventPair;

		typedef std::vector<double> trainVector;
		typedef std::queue<double> trainArrivalQueue;
		typedef std::queue<double> trainUnloadQueue;
		typedef std::queue<double> crewHourQueue;
		typedef std::queue<double> crewTravelQueue;

		typedef std::map<int, Crew*> crewMap;
		typedef std::map<int, Train*> testTrain;
		typedef std::queue<Train*> loadingDock;
		
		typedef std::priority_queue<Event, std::vector<Event>, ComparePQ> eventPQ;
		typedef std::set<Event, ComparePQ> eventSet;

		Dock();
		Dock(double avg_arrival);
		Dock(std::queue<double> a, std::queue<double> u, std::queue<double> h, std::queue<double> t);
	
		int crewIndex = 0;
		int trainIndex = 0;
		int numCrew = 0;
		int numTrains = 0;
		
		int events = 0;
		int max_trains = 0;	
		
		int getQueueSize() const;
		bool isFinished() const;
		bool isLoadingDockEmpty() const;
		bool isLoadingDockIdle() const;
		bool isLoadingDockBusy() const;
		bool isLoadingDockHogged() const;
		int getNumTrains() const;
		int getNumCrew() const;
		double getTime() const;
		int getNumTrainsDeparted() const;

		
		void printTrains() const;
		
		void announceUnloadStart() const;
		void announceUnloadEnd() const;
		void announceHoggedInFront() const;
		void announceUnHoggedInFront() const;
		void announceHoggedInQueue(const Train& t) const;
		void announceUnHoggedInQueue(const Train& t) const;
		void announceHoggedInLoading() const;
		void announceUnHoggedInLoading() const;

		void processEvent() const;

		void loadArrivalTimes(std::queue<double>& a);
		void loadUnloadTimes(std::queue<double>& u);
		void loadCrewHoursTimes(std::queue<double>& h);
		
		void loadCrewTravelTimes(std::queue<double>& t);

		void generateTrainArrivalTimes(double sim_time, double avg_train_arrival_time);

		void generateTrainUnloadTimes();
		void generateCrewArrivalTimes();
		void generateCrewWorkTimes();
		void summonCrew(Train& ghost_train);

		void generateEvents(Event::events_t e, Train& train);

		void attachCrew(Train& ghost_train);
		void assembleGhostTrain();
		void assembleTrain();
		void addTrain(const Train& t);
		void printTime() const;
		void printStats();
		void passTime(double newT);
		void addEvent(double event_time, const Event &e);
		void resetTime();
		void setLoadingDockIdle(bool s);
		void setLoadingDockBusy(bool s);
		void setLoadingDockHogged(bool s);
		void platform();
		eventPair nextEvent();


		void pre_SummonCrew(Train& ghost_train);
		void pre_GenerateEvents(Event::events_t e, Train& train);
		void pre_AttachCrew(Train& ghost_train);
		void pre_AssembleGhostTrain();
		void pre_AssembleTrain();
		void pre_Platform();	

	private:
		trainEntry train_entry;
		trainMap train_map;
		trainQueue train_queue;
		hogoutCount hog_count;
		eventMap event_map;
		
		trainArrivalQueue train_arrival_times;
		trainUnloadQueue train_unload_times;
		crewHourQueue crew_hours;
		crewTravelQueue crew_travel;
		crewTravelQueue crew_travel_extra;
		crewMap crew_map;
		
		eventPQ event_pq;
		eventSet event_set;

		testTrain test_train_map;

		loadingDock loading_dock;
		int loading_dock_recently_loaded = 0;
		bool finished = false;
		double avg_train_arrival;
		double time = 0;
		int num_trains_departed = 0;
		double max_recorded_time_in_system = 0;
		double sum_recorded_time_in_system = 0;
		double avg_recorded_time_in_system = 0;

		double sum_time_in_queue = 0;
		double avg_time_in_queue = 0;

		double dock_idle_start = 0;
		bool dock_idle_recording = false;
		double dock_idle_time = 0;

		double dock_busy_time = 0;
		bool dock_busy_recording = false;
		double dock_busy_start = 0;

		double dock_hogged_start = 0;
		bool dock_hogged_recording = false;
		double dock_hogged_time = 0;

		double dock_idle_percent = 0;
		double dock_busy_percent = 0;
		double dock_hogged_percent = 0;
		bool dock_idle_status = true;
		bool dock_busy_status = false;
		bool dock_hogged_status = false;

		int max_recorded_queue_size = 0;
		int max_recorded_hogged_count = 0;
		std::map<int,int> hogged_histogram;
		std::map<int,int> test_hist;
		std::map<int,int> total_hist;
		std::default_random_engine generator;
};

#endif
