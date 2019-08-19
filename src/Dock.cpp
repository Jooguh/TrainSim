// Kenny Park

#include "Dock.hpp"

// Default arrival time is set to 1 arrival every 10 hours on average
Dock::Dock() : generator(std::random_device()())
{
	double avg = 10;
	double lambda = 1 / avg;
	std::uniform_real_distribution<double> dist(0,1);
	
	avg_train_arrival = ((-1/lambda) * log(dist(generator)));
	
}

Dock::Dock(double avg_arrival) : generator(std::random_device()()), avg_train_arrival(avg_arrival)
{
	double avg = avg_arrival;
	double lambda = 1 / avg;
	std::uniform_real_distribution<double> dist(0,1);
	
	avg_train_arrival = ((-1/lambda) * log(dist(generator)));
}

Dock::Dock(std::queue<double> a, std::queue<double> u, std::queue<double> h, std::queue<double> t)
{
	loadArrivalTimes(a);
	loadUnloadTimes(u);
	loadCrewHoursTimes(h);
	loadCrewTravelTimes(t);	
}

int Dock::getQueueSize() const
{
	return train_queue.size();
}

bool Dock::isFinished() const
{
	return finished;
}

bool Dock::isLoadingDockEmpty() const
{
	return loading_dock.empty();
}

bool Dock::isLoadingDockIdle() const
{
	return dock_idle_status;
}

bool Dock::isLoadingDockBusy() const
{
	return dock_busy_status;
}

bool Dock::isLoadingDockHogged() const
{
	return dock_hogged_status;
}

int Dock::getNumTrains() const
{
	return numTrains;
}

int Dock::getNumCrew() const
{
	return numCrew;
}

void Dock::generateTrainArrivalTimes(double simulation_time, double avg_train_arrival_time)
{
	std::uniform_real_distribution<double> dist(0,1);
	double avg = avg_train_arrival_time;
	double lambda = 1 / avg;

	avg_train_arrival = dist(generator);
	
	
	avg_train_arrival = ((-1/lambda) * log(dist(generator)));

	double sum = avg_train_arrival;
	double sumArrivalTimes = 0;
	int i = 0;
	std::cout << "\n";
	do
	{
		sum = ((-1/lambda) * log(dist(generator))); 
		sumArrivalTimes += sum;
		if(sumArrivalTimes <= simulation_time)
		{
			train_arrival_times.push(sumArrivalTimes);
			
		}
		else
		{
		}
		++i;
	
	}
	while(sumArrivalTimes < simulation_time);
	max_trains = train_arrival_times.size();
}

void Dock::generateTrainUnloadTimes()
{
	std::uniform_real_distribution<double> distribution(3.50, 4.50);
	double unloadTime;
	int i = 0;
	
	do
	{
		unloadTime = distribution(generator);
		train_unload_times.push(unloadTime);
		++i;
	}
	while(i < train_arrival_times.size());
}



void Dock::generateCrewArrivalTimes()
{
	

}

void Dock::generateCrewWorkTimes()
{

}

void Dock::summonCrew(Train& empty_train)
{
	crew_map[numCrew] = new Crew(numCrew);
	crew_map[numCrew]->setWorkTime(12);
	crew_map[numCrew]->setStartTime(getTime());

	empty_train.setTrainCrew(*crew_map[numCrew]);
	crew_map[numCrew]->setTrain(empty_train);
	generateEvents(Event::events_t::crew_arrival, empty_train);
	

	++numCrew;
	++crewIndex;
}

double Dock::getTime() const
{
	return time;
}

int Dock::getNumTrainsDeparted() const
{
	return num_trains_departed;
}

void Dock::printTrains() const
{

	for(int i = 0; i < test_train_map.size(); i++)
    {
		test_train_map.at(i)->toString();
	}
}

void Dock::generateEvents(Event::events_t e, Train& subject)
{
	if(e == Event::events_t::train_arrival)
	{
		event_set.insert(*new Event(Event::events_t::train_arrival, events, subject.getArrivalTime(), subject));
		++events;

	}
	else if(e == Event::events_t::crew_hog)
	{
		event_set.insert(*new Event(Event::events_t::crew_hog, events, getTime() + subject.getTrainCrew()->getRemainingWork(), subject));
		++events;
	}
	else if(e == Event::events_t::unload_start)
	{
		event_set.insert(*new Event(Event::events_t::unload_start, events, getTime(), subject));
		++events;
	}
	else if(e == Event::events_t::unload_end)
	{
		event_set.insert(*new Event(Event::events_t::unload_end, events, subject.getUnloadTime() + getTime(), subject));
		++events;
	}
	else if(e == Event::events_t::crew_arrival)
	{
		event_set.insert(*new Event(Event::events_t::crew_arrival, events, getTime() + subject.getTrainCrew()->getArrivalTime(), subject));
		++events;
	}
	else if(e == Event::events_t::crew_summon)
	{
		event_set.insert(*new Event(Event::events_t::crew_summon, events, getTime(), subject));	
		++events;
	}
}


// Creates and assigns new crew to ghost train
void Dock::attachCrew(Train& ghost_train)
{
	crew_map[crewIndex] = new Crew(ghost_train.getArrivalTime(), crewIndex);
	++numCrew;
	ghost_train.setTrainCrew(*crew_map[crewIndex]);
	crew_map[crewIndex]->setTrain(ghost_train);
	crew_map[crewIndex]->setStartTime(ghost_train.getArrivalTime());
	++crewIndex;
}

// Creates a train and corresponding event, but without a crew for now
void Dock::assembleGhostTrain()
{

	test_train_map[numTrains] = new Train(train_unload_times.front(), numTrains, train_arrival_times.front());
	train_unload_times.pop();
	train_arrival_times.pop();
	generateEvents(Event::events_t::train_arrival, *test_train_map[numTrains]);
	++numTrains;
}

void Dock::assembleTrain()
{
	test_train_map[numTrains] = new Train(train_unload_times.front(), numTrains, train_arrival_times.front());
	train_unload_times.pop();
	train_arrival_times.pop();
	++numTrains;


	crew_map[crewIndex] = new Crew(crewIndex);
	crew_map[crewIndex]->setArrivalTime(test_train_map[trainIndex]->getArrivalTime());
	crew_map[crewIndex]->setStartTime(crew_map[crewIndex]->getArrivalTime());

	++numCrew;
	test_train_map[trainIndex]->setTrainCrew(*crew_map[crewIndex]);
	crew_map[crewIndex]->setTrain(*test_train_map[trainIndex]);
	
	generateEvents(Event::events_t::train_arrival, *test_train_map[trainIndex]);
	++trainIndex;
	++crewIndex;
}

void Dock::addTrain(const Train &t)
{
}

void Dock::printTime() const
{
	std::cout << "Time " << std::setprecision(2) << std::fixed << getTime() <<  ": ";
}

void Dock::passTime(double newTime)
{
	time = newTime;
}

void Dock::addEvent(double event_time, const Event &e)
{
	++events;
}

void Dock::resetTime()
{
	time = 0.0;
}

void Dock::announceUnloadStart() const
{
	printTime();	
	std::cout << "train " << loading_dock.front()->getTrainID() << " entering dock for " << std::setprecision(2) << std::fixed << loading_dock.front()->getUnloadTime() << " h of unloading, ";

	double start_time = loading_dock.front()->getTrainCrew()->getStartTime();
	double work_time = loading_dock.front()->getTrainCrew()->getWorkTime();
	double time_until_hog = work_time - (getTime() - start_time);

	std::cout << "crew " << loading_dock.front()->getTrainCrew()->getCrewID() << " with " << std::setprecision(2) << std::fixed << time_until_hog << "h before hogout" << std::endl;
}

void Dock::announceUnloadEnd() const
{
	printTime();
	std::cout << "train " << loading_dock.front()->getTrainID() << " departing" << std::endl;
}

void Dock::announceHoggedInFront() const
{
	printTime();
	std::cout << "train " << train_queue.front()->getTrainID();
	std::cout << " crew " << train_queue.front()->getTrainCrew()->getCrewID() << " hasn't arrived yet, cannot enter dock (SERVER HOGGED)" << std::endl;
}

void Dock::announceUnHoggedInFront() const
{
	printTime();
	std::cout << "train " << train_queue.front()->getTrainID();
	std::cout << " replacement crew " << train_queue.front()->getTrainCrew()->getCrewID() << " arrives (SERVER UNHOGGED)" << std::endl;
}

void Dock::announceHoggedInQueue(const Train& train) const
{
	printTime();
	std::cout << "train " << train.getTrainID();
	std::cout << " crew " << train.getTrainCrew()->getCrewID() << " hogged out in queue"  << std::endl;
}

void Dock::announceUnHoggedInQueue(const Train& train) const
{
	printTime();
	std::cout << "train " << train.getTrainID();
	std::cout << " crew " << train.getTrainCrew()->getCrewID() << " arrives" << std::endl;
}

void Dock::announceHoggedInLoading() const
{
	printTime();
	std::cout << "train " << loading_dock.front()->getTrainID();
	std::cout << " crew " << loading_dock.front()->getTrainCrew()->getCrewID() << " hogged out during service (SERVER HOGGED)"  << std::endl;
}

void Dock::announceUnHoggedInLoading() const
{
	printTime();
	std::cout << "train " << loading_dock.front()->getTrainID();
	std::cout << " replacement crew " << loading_dock.front()->getTrainCrew()->getCrewID() << " arrives (SERVER UNHOGGED)" << std::endl;
}

void Dock::processEvent() const
{
	// UNUSED
}

void Dock::loadArrivalTimes(std::queue<double>& a)
{
	int s = a.size();
	for(int i = 0; i < s; ++i)
	{
		train_arrival_times.push(a.front());
		a.pop();
	}
	max_trains = train_arrival_times.size();
}

void Dock::loadUnloadTimes(std::queue<double>& u)
{
	int s = u.size();
	for(int i = 0; i < s; ++i)
	{
		train_unload_times.push(u.front());
		u.pop();
	}
}

void Dock::loadCrewHoursTimes(std::queue<double>& h)
{
	int s = h.size();
	for(int i = 0; i < s; ++i)
	{
		crew_hours.push(h.front());
		h.pop();
	}
}
void Dock::loadCrewTravelTimes(std::queue<double>& t)
{
	int s = t.size();
	for(int i = 0; i < s; ++i)
	{
		crew_travel.push(t.front());
		t.pop();
	}
	crew_travel_extra = crew_travel;
}

void Dock::setLoadingDockIdle(bool s)
{
	dock_idle_status = s;
}

void Dock::setLoadingDockBusy(bool s)
{
	dock_busy_status = s;
}

void Dock::setLoadingDockHogged(bool s)
{
	dock_hogged_status = s;
}

void Dock::platform()
{

	//while(train_arrival_times.size() != 0)
	for(int i = 0; i < max_trains; ++i)
	{
		//std::cout << "i = " << i << std::endl;
		//assembleTrain();
		if(i == 0)
		{
			assembleTrain();
		}
		else
		{
			//std::cout << "i = " << i << std::endl;
			assembleGhostTrain();
		}

	}
	resetTime();
	
	while(num_trains_departed != max_trains)
	{
		if(isLoadingDockIdle() && !dock_idle_recording)
		{
			if(isLoadingDockEmpty() || (!isLoadingDockEmpty() && loading_dock.front()->getHoggedStatus()))
			{
				dock_idle_start = getTime();
				dock_idle_recording = true;
			}
		}
		else if(isLoadingDockBusy() && !dock_busy_recording)
		{
			if(!isLoadingDockEmpty() && !loading_dock.front()->getHoggedStatus())
			{
				dock_busy_start = getTime();
				dock_busy_recording = true;
			}

		}
		if(isLoadingDockHogged() && !dock_hogged_recording)
		{
			if(!isLoadingDockEmpty() && loading_dock.front()->getHoggedStatus())
			{
				dock_hogged_start = getTime();
				dock_hogged_recording = true;
			}
		}

		if(train_queue.size() > max_recorded_queue_size)
		{
			max_recorded_queue_size = train_queue.size();
		}

		if(isLoadingDockEmpty() && !train_queue.empty())
		{
			if(train_queue.front()->getHoggedStatus())
			{

			}
			else
			{
				generateEvents(Event::events_t::unload_start, *train_queue.front());
				if(train_queue.front()->getUnloadTime() > train_queue.front()->getTrainCrew()->getRemainingWork())
				{
					generateEvents(Event::events_t::crew_hog, *train_queue.front());
				}	
			}
		}
		auto event_it = event_set.begin();

		while(event_it->getTime() < getTime())
		{
			event_set.erase(event_it);
			event_it = event_set.begin();
		}

		if(event_it->getSubject()->hasDeparted())
		{
			event_set.erase(event_it);
		}
		
		else
		{
			if(event_it->getEventType() == Event::events_t::crew_hog)
			{
				passTime(event_it->getTime());
				Train* target_train = event_it->getSubject();
				target_train->setHoggedStatus(true);
				target_train->incHoggedCount();

				test_hist[target_train->getHoggedCount()]++;
				
				if(target_train->getHoggedCount() > max_recorded_hogged_count)
				{
					max_recorded_hogged_count = target_train->getHoggedCount();
				}

				for(int i = max_recorded_hogged_count; i > 1; --i)
				{
					if(target_train->getHoggedCount() >= i)
					{
						hogged_histogram[i]++;
					}
				}
				
				if(!isLoadingDockEmpty() && (loading_dock.front()->getTrainID() == target_train->getTrainID()))
				{
					announceHoggedInLoading();
					setLoadingDockIdle(true);
					dock_idle_recording = false;;
					setLoadingDockBusy(false);
					dock_busy_time += (getTime() - dock_busy_start);

					setLoadingDockHogged(true);
					dock_hogged_recording = false;
				}
				else if(isLoadingDockEmpty() && (train_queue.front()->getTrainID() == target_train->getTrainID()))
				{
					announceHoggedInFront();
					setLoadingDockIdle(true);
					dock_idle_recording = false;;
				}
				else
				{
					announceHoggedInQueue(*target_train);
				}
                
				event_set.erase(event_it);
				generateEvents(Event::events_t::crew_summon, *target_train);
			}
			else if(event_it->getEventType() == Event::events_t::train_arrival)
			{
				passTime(event_it->getTime());
				if(event_it->getSubject()->getTrainID() != 0)
				{
					attachCrew(*event_it->getSubject());
				}
				Train* target_train = event_it->getSubject();
				event_it->announceTrainArrival();
				event_set.erase(event_it);
				target_train->setTimeInQueue(getTime());
				train_queue.push(target_train);
				target_train->getTrainCrew()->updateRemainingWork(getTime());
				generateEvents(Event::events_t::crew_hog, *target_train);
			}
			
			else if(event_it->getEventType() == Event::events_t::unload_start)
			{
				passTime(event_it->getTime());	
				setLoadingDockIdle(false);
				dock_idle_time += (getTime() - dock_idle_start);
				setLoadingDockBusy(true);
				dock_busy_recording = false;	
				Train* target_train = event_it->getSubject();
				event_set.erase(event_it);
				loading_dock.push(train_queue.front());
				sum_time_in_queue += target_train->getTimeInQueue(getTime());
				announceUnloadStart();
				generateEvents(Event::events_t::unload_end, *target_train);
				train_queue.pop();
			}
			
			else if(event_it->getEventType() == Event::events_t::unload_end)
			{
				passTime(event_it->getTime());
				setLoadingDockIdle(true);
				dock_idle_recording = false;
				setLoadingDockBusy(false);

				dock_busy_time += (getTime() - dock_busy_start);
				Train* target_train = event_it->getSubject();
				event_set.erase(event_it);
				target_train->setDeparted();
				target_train->getTrainCrew()->setDeparted();
				announceUnloadEnd();
				loading_dock.pop();
				++num_trains_departed;
				sum_recorded_time_in_system += (getTime() - target_train->getArrivalTime());
				if(getTime() - target_train->getArrivalTime() > max_recorded_time_in_system)
				{
					max_recorded_time_in_system = (getTime() - target_train->getArrivalTime());
				}
				for(auto d : event_set)
				{
					if(d.getSubject() == target_train)
					{
						Event* e = &d;
						event_set.erase(*e);
					}
				}
			}
			
			else if(event_it->getEventType() == Event::events_t::crew_summon)
			{
				passTime(event_it->getTime());
				Train* target_train = event_it->getSubject();
				event_set.erase(event_it);

				summonCrew(*target_train);
				if(!isLoadingDockEmpty() && (loading_dock.front()->getTrainID() == target_train->getTrainID()))
				{
					double remaining_unload = 0;
					for(auto d : event_set)
					{
						if(d.getSubject() == target_train)
						{
							if(d.getEventType() == Event::events_t::unload_end)
							{
								remaining_unload = d.getTime() - getTime();
								Event* e = &d;
								event_set.erase(*e);
							}
						}
					}
				
					double new_unload_end = target_train->getTrainCrew()->getArrivalTime() + remaining_unload;
					target_train->setUnloadTime(new_unload_end);
					generateEvents(Event::events_t::unload_end, *target_train);
				}

			}
			else if(event_it->getEventType() == Event::events_t::crew_arrival)
			{
				passTime(event_it->getTime());
				Train* target_train = event_it->getSubject();
				target_train->getTrainCrew()->updateRemainingWork(getTime());
				event_set.erase(event_it);
				if(!isLoadingDockEmpty() && (loading_dock.front()->getTrainID() == target_train->getTrainID()))
				{	
					announceUnHoggedInLoading();
					setLoadingDockIdle(false);
					dock_idle_time += (getTime() - dock_idle_start);
					setLoadingDockBusy(true);
					dock_busy_recording = false;
					setLoadingDockHogged(false);
					dock_hogged_time += (getTime() - dock_hogged_start);
					loading_dock.front()->setHoggedStatus(false);
				}
				else if(isLoadingDockEmpty() && (train_queue.front()->getTrainID() == target_train->getTrainID()))
				{
					announceUnHoggedInFront();
					setLoadingDockIdle(false);
					dock_idle_time += (getTime() - dock_idle_start);
					setLoadingDockBusy(true);
					dock_busy_recording = false;
					train_queue.front()->setHoggedStatus(false);
					loading_dock.push(train_queue.front());
					announceUnloadStart();
					generateEvents(Event::events_t::unload_end, *target_train);
					train_queue.pop();	
				}
				else
				{
					announceUnHoggedInQueue(*target_train);
					target_train->setHoggedStatus(false);
				}
				generateEvents(Event::events_t::crew_hog, *target_train);

			}
		}
	}

	printTime();
	std::cout << "simulation ended\n\n";
	avg_recorded_time_in_system = sum_recorded_time_in_system / num_trains_departed;
	avg_time_in_queue = sum_time_in_queue / getTime();
	
	dock_idle_percent = 100 * (dock_idle_time / getTime());
	dock_busy_percent = 100 * (dock_busy_time / getTime());
	dock_hogged_percent = 100 * (dock_hogged_time / getTime());

	hogged_histogram[0] = num_trains_departed;
	test_hist[0] = num_trains_departed;
	printStats();
}

void Dock::printStats()
{
	std::cout << "Statistics\n----------\n";
	std::cout << "Total number of trains served: " <<  num_trains_departed << std::endl;
	std::cout << "Average time-in-system per train: " << avg_recorded_time_in_system << "h" << std::endl;
	std::cout << "Maximum time-in-system per train: " << max_recorded_time_in_system << "h" << std::endl;
	std::cout << "Dock idle percentage: " << dock_idle_percent << "%" << std::endl;
	std::cout << "Dock busy percentage: " << dock_busy_percent << "%" << std::endl;
	std::cout << "Dock hogged-out percentage: " << dock_hogged_percent << "%" << std::endl;
	std::cout << "Time average of trains in queue: " << avg_time_in_queue << std::endl;
	std::cout << "Maximum number of trains in queue: " << max_recorded_queue_size << std::endl;
	std::cout << "Histogram of hogout count per train: " << std::endl;

	for(int i = 0; i <= max_recorded_hogged_count; ++i)
	{
		std::cout << "[" << i << "]: " << test_hist[i] << std::endl;
	}
}

void Dock::pre_SummonCrew(Train& empty_train)
{
	if(crew_travel.size() == 0)
	{
		crew_travel = crew_travel_extra;
	}
	crew_map[numCrew] = new Crew(numCrew);

	crew_map[numCrew]->setArrivalTime(crew_travel.front());

	crew_travel.pop();

	crew_map[numCrew]->setWorkTime(12);
	crew_map[numCrew]->setStartTime(getTime());
	empty_train.setTrainCrew(*crew_map[numCrew]);
	
	crew_map[numCrew]->setTrain(empty_train);
	generateEvents(Event::events_t::crew_arrival, empty_train);
	
	++numCrew;
	++crewIndex;
}
void Dock::pre_GenerateEvents(Event::events_t e, Train& subject)
{
	if(e == Event::events_t::train_arrival)
	{
		event_set.insert(*new Event(Event::events_t::train_arrival, events, subject.getArrivalTime(), subject));
		++events;
	}
	else if(e == Event::events_t::crew_hog)
	{
		event_set.insert(*new Event(Event::events_t::crew_hog, events, getTime() + subject.getTrainCrew()->getRemainingWork(), subject));
		++events;
	}
	else if(e == Event::events_t::unload_start)
	{
		event_set.insert(*new Event(Event::events_t::unload_start, events, getTime(), subject));
		++events;
	}
	else if(e == Event::events_t::unload_end)
	{
		event_set.insert(*new Event(Event::events_t::unload_end, events, subject.getUnloadTime() + getTime(), subject));
		++events;
	}
	else if(e == Event::events_t::crew_arrival)
	{
		event_set.insert(*new Event(Event::events_t::crew_arrival, events, getTime() + subject.getTrainCrew()->getArrivalTime(), subject));
		++events;
	}
	else if(e == Event::events_t::crew_summon)
	{
		event_set.insert(*new Event(Event::events_t::crew_summon, events, getTime(), subject));	
		++events;
	}
}
void Dock::pre_AttachCrew(Train& ghost_train)
{
	crew_map[crewIndex] = new Crew(ghost_train.getArrivalTime(), crewIndex);
	++numCrew;
	ghost_train.setTrainCrew(*crew_map[crewIndex]);
	crew_map[crewIndex]->setTrain(ghost_train);
	crew_map[crewIndex]->setWorkTime(crew_hours.front());
	crew_hours.pop();
	crew_map[crewIndex]->setStartTime(ghost_train.getArrivalTime());
	++crewIndex;
}
void Dock::pre_AssembleGhostTrain()
{
	test_train_map[numTrains] = new Train(train_unload_times.front(), numTrains, train_arrival_times.front());
	train_unload_times.pop();
	train_arrival_times.pop();
	generateEvents(Event::events_t::train_arrival, *test_train_map[numTrains]);
	++numTrains;
}
void Dock::pre_AssembleTrain()
{
	test_train_map[numTrains] = new Train(train_unload_times.front(), numTrains, train_arrival_times.front());
	train_unload_times.pop();
	train_arrival_times.pop();
	++numTrains;

	crew_map[crewIndex] = new Crew(crewIndex);
	crew_map[crewIndex]->setArrivalTime(test_train_map[trainIndex]->getArrivalTime());
	crew_map[crewIndex]->setStartTime(crew_hours.front());
	crew_hours.pop();
	++numCrew;

	test_train_map[trainIndex]->setTrainCrew(*crew_map[crewIndex]);

	crew_map[crewIndex]->setTrain(*test_train_map[trainIndex]);
	
	generateEvents(Event::events_t::train_arrival, *test_train_map[trainIndex]);
	++trainIndex;
	++crewIndex;
}
void Dock::pre_Platform()
{
	for(int i = 0; i < max_trains; ++i)
	{
		if(i == 0)
		{
			pre_AssembleTrain();
		}
		else
		{

			pre_AssembleGhostTrain();
		}
	}
	resetTime();
	
	while(num_trains_departed != max_trains)
	{
		if(isLoadingDockIdle() && !dock_idle_recording)
		{
			if(isLoadingDockEmpty() || (!isLoadingDockEmpty() && loading_dock.front()->getHoggedStatus()))
			{
				dock_idle_start = getTime();
				dock_idle_recording = true;
			}
		}
		else if(isLoadingDockBusy() && !dock_busy_recording)
		{
			if(!isLoadingDockEmpty() && !loading_dock.front()->getHoggedStatus())
			{
				dock_busy_start = getTime();
				dock_busy_recording = true;
			}
		}
		if(isLoadingDockHogged() && !dock_hogged_recording)
		{
			if(!isLoadingDockEmpty() && loading_dock.front()->getHoggedStatus())
			{
				dock_hogged_start = getTime();
				dock_hogged_recording = true;
			}
		}

		if(train_queue.size() > max_recorded_queue_size)
		{
			max_recorded_queue_size = train_queue.size();
		}

		if(isLoadingDockEmpty() && !train_queue.empty())
		{
			if(train_queue.front()->getHoggedStatus())
			{
				//announceHoggedInFront();
			}
			else
			{
				generateEvents(Event::events_t::unload_start, *train_queue.front());

				if(train_queue.front()->getUnloadTime() > train_queue.front()->getTrainCrew()->getRemainingWork())
				{
					generateEvents(Event::events_t::crew_hog, *train_queue.front());
				}	
			}
		}

		auto event_it = event_set.begin();	

		while(event_it->getTime() < getTime())
		{
			event_set.erase(event_it);
			event_it = event_set.begin();
		}

		if(event_it->getSubject()->hasDeparted())
		{
			event_set.erase(event_it);
		}
		
		else
		{
			if(event_it->getEventType() == Event::events_t::crew_hog)
			{
				passTime(event_it->getTime());
				Train* target_train = event_it->getSubject();
				target_train->setHoggedStatus(true);
				target_train->incHoggedCount();

				test_hist[target_train->getHoggedCount()] = test_hist[target_train->getHoggedCount()] + 1;
				total_hist[target_train->getTrainID()]++;
				
				if(target_train->getHoggedCount() > max_recorded_hogged_count)
				{
					max_recorded_hogged_count = target_train->getHoggedCount();
				}
				event_set.erase(event_it);
				
				if(!isLoadingDockEmpty() && (loading_dock.front()->getTrainID() == target_train->getTrainID()))
				{
					announceHoggedInLoading();
					setLoadingDockIdle(true);
					dock_idle_recording = false;;
					setLoadingDockBusy(false);
					dock_busy_time += (getTime() - dock_busy_start);

					setLoadingDockHogged(true);
					dock_hogged_recording = false;
				}
				else if(isLoadingDockEmpty() && (train_queue.front()->getTrainID() == target_train->getTrainID()))
				{
					announceHoggedInFront();
					setLoadingDockIdle(true);
					dock_idle_recording = false;;
				}
				else
				{
					announceHoggedInQueue(*target_train);
				}
				generateEvents(Event::events_t::crew_summon, *target_train);
			}
			else if(event_it->getEventType() == Event::events_t::train_arrival)
			{
				passTime(event_it->getTime());
				if(event_it->getSubject()->getTrainID() != 0)
				{
					pre_AttachCrew(*event_it->getSubject());
				}

				Train* target_train = event_it->getSubject();
				event_it->announceTrainArrival();

				event_set.erase(event_it);

				target_train->getTrainCrew()->updateRemainingWork(getTime());
				train_queue.push(target_train);	

				generateEvents(Event::events_t::crew_hog, *target_train);
			}
			
			else if(event_it->getEventType() == Event::events_t::unload_start)
			{
				passTime(event_it->getTime());	
				setLoadingDockIdle(false);

				dock_idle_time += (getTime() - dock_idle_start);

				setLoadingDockBusy(true);
				dock_busy_recording = false;	
				Train* target_train = event_it->getSubject();
				sum_time_in_queue += (getTime() - target_train->getArrivalTime());
				event_set.erase(event_it);
				loading_dock.push(train_queue.front());
				announceUnloadStart();
				generateEvents(Event::events_t::unload_end, *target_train);
				train_queue.pop();
			}
			
			else if(event_it->getEventType() == Event::events_t::unload_end)
			{
				passTime(event_it->getTime());
				setLoadingDockIdle(true);
				dock_idle_recording = false;
				setLoadingDockBusy(false);

				dock_busy_time += (getTime() - dock_busy_start);
				Train* target_train = event_it->getSubject();
				event_set.erase(event_it);
				target_train->setDeparted();
				target_train->getTrainCrew()->setDeparted();
				announceUnloadEnd();
				loading_dock.pop();
				++num_trains_departed;
				sum_recorded_time_in_system += (getTime() - target_train->getArrivalTime());
				if(getTime() - target_train->getArrivalTime() > max_recorded_time_in_system)
				{
					max_recorded_time_in_system = (getTime() - target_train->getArrivalTime());
				}
				for(auto d : event_set)
				{
					if(d.getSubject() == target_train)
					{
						Event* e = &d;
						event_set.erase(*e);
					}
				}
			}
			
			else if(event_it->getEventType() == Event::events_t::crew_summon)
			{
				passTime(event_it->getTime());
				Train* target_train = event_it->getSubject();
				event_set.erase(event_it);
				
				pre_SummonCrew(*target_train);
				if(!isLoadingDockEmpty() && (loading_dock.front()->getTrainID() == target_train->getTrainID()))
				{
					double remaining_unload = 0;
					for(auto d : event_set)
					{
						if(d.getSubject() == target_train)
						{
							if(d.getEventType() == Event::events_t::unload_end)
							{
								remaining_unload = d.getTime() - getTime();
								Event* e = &d;
								event_set.erase(*e);
							}
						}
					}
					double new_unload_end = target_train->getTrainCrew()->getArrivalTime() + remaining_unload;
					target_train->setUnloadTime(new_unload_end);
					generateEvents(Event::events_t::unload_end, *target_train);
				}
			}
			else if(event_it->getEventType() == Event::events_t::crew_arrival)
			{
				passTime(event_it->getTime());
				Train* target_train = event_it->getSubject();
				target_train->getTrainCrew()->updateRemainingWork(getTime());
				event_set.erase(event_it);
				if(!isLoadingDockEmpty() && (loading_dock.front()->getTrainID() == target_train->getTrainID()))
				{	
					announceUnHoggedInLoading();
					setLoadingDockIdle(false);
					dock_idle_time += (getTime() - dock_idle_start);
					setLoadingDockBusy(true);
					dock_busy_recording = false;
					setLoadingDockHogged(false);
					dock_hogged_time += (getTime() - dock_hogged_start);
					loading_dock.front()->setHoggedStatus(false);
				}
				else if(isLoadingDockEmpty() && (train_queue.front()->getTrainID() == target_train->getTrainID()))
				{
					announceUnHoggedInFront();
					setLoadingDockIdle(false);
					dock_idle_time += (getTime() - dock_idle_start);
					setLoadingDockBusy(true);
					dock_busy_recording = false;
					train_queue.front()->setHoggedStatus(false);
					loading_dock.push(train_queue.front());
					announceUnloadStart();
					generateEvents(Event::events_t::unload_end, *target_train);
					train_queue.pop();	
				}
				else
				{
					announceUnHoggedInQueue(*target_train);
					target_train->setHoggedStatus(false);
				}
				generateEvents(Event::events_t::crew_hog, *target_train);
			}
		}
	}

	printTime();
	std::cout << "simulation ended\n\n";
	avg_recorded_time_in_system = sum_recorded_time_in_system / num_trains_departed;
	avg_time_in_queue = sum_time_in_queue / getTime();
	
	dock_idle_percent = 100 * (dock_idle_time / getTime());
	dock_busy_percent = 100 * (dock_busy_time / getTime());
	dock_hogged_percent = 100 * (dock_hogged_time / getTime());
	test_hist[0] = num_trains_departed;
	printStats();
}

Dock::eventPair Dock::nextEvent()
{
	// UNUSED
}
