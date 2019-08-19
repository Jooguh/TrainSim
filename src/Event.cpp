// Kenny Park


#include "Event.hpp"

int Event::global_id = 0;

Event::Event () 
{
	
}

Event::Event (events_t e, int ID, double eventTime, Train& subject) : eType(e), id(ID), time(eventTime)
{
	eventSubject = &subject;
}

void Event::setEventType(events_t e)
{
	eType = e;
}

void Event::setID(int i)
{
	id = i;
}

void Event::setTime(double t)
{
	time = t;
}

void Event::setSubject(Train& subject)
{
	eventSubject = &subject;
}

void Event::announceTrainArrival() const
{
	std::cout << "Time " << std::setprecision(2) << std::fixed << time << ": ";
	std::cout << "train " << eventSubject->getTrainID();
	std::cout << " arrival for " << std::setprecision(2) << std::fixed << eventSubject->getUnloadTime() << "h of unloading, ";
	std::cout << "crew " << eventSubject->getTrainCrew()->getCrewID() << " with " << std::setprecision(2) << std::fixed << eventSubject->getTrainCrew()->getWorkTime() << "h before hogout \n";
}

void Event::announceCrewSummon() const
{

}

void Event::announceCrewArrival() const
{
}

void Event::announceCrewHog() const
{
	std::cout << "Time " << std::setprecision(2) << std::fixed << time << ": ";
	std::cout << "train " << eventSubject->getTrainID();
	std::cout << " crew " << eventSubject->getTrainCrew()->getCrewID() << " hogged out " << std::endl;
}

void Event::printEvent() const
{
	std::cout << "EVENT: " << id << ", Train " << getSubject()->getTrainID() << ", Event time: " << getTime() << ", Event type: ";
	if(eType == unload_start)
	{
		std::cout << "unload_start";
	}
	else if(eType == unload_end)
	{
		std::cout << "unload_end";
	}
	else if(eType == train_arrival)
	{
		std::cout << "train_arrival";
	}
	else if(eType == crew_summon)
	{
		std::cout << "crew_summon";
	}
	else if(eType == crew_arrival)
	{
		std::cout << "crew_arrival";
	}
	else if(eType == crew_hog)
	{
		std::cout << "crew_hog";
	}
	std::cout << std::endl;
}

Train* Event::getSubject() const
{
	return eventSubject;
}

int Event::getID() const
{
	return id;
}

double Event::getTime() const
{
	return time;
}

Event::events_t  Event::getEventType() const
{
	return eType;
}


