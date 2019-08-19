// Kenny Park



#include "Train.hpp"
#include "Crew.hpp"


Train::Train ()  : generator(std::random_device()()){

	std::uniform_real_distribution<double> distribution(3.50, 4.50);	
	unloadTime = distribution(generator);
}

Train::Train(int pickID) : generator(std::random_device()()), id(pickID) {
	std::uniform_real_distribution<double> distribution(3.50, 4.50);
	
	unloadTime = distribution(generator);
}

Train::Train(double pickUnloadTime, int pickID) : unloadTime(pickUnloadTime), id(pickID) {

}

Train::Train(double pickUnloadTime, int pickID, double arrive) : unloadTime(pickUnloadTime), id(pickID), arrivalTime(arrive) {

}

double Train::getUnloadTime() const{
	return unloadTime;
}

int Train::getTrainID() const {
	return id;

}

int Train::getHoggedCount() const
{
	return hogged_count;
}

bool Train::hasDeparted() const
{
	return departed;
}

double Train::getArrivalTime() const {
	return arrivalTime;
}

Crew* Train::getTrainCrew() const
{
	return currentCrew;
}

bool Train::getHoggedStatus() const
{
	return crew_hogged;
}

double Train::getTimeInQueue(double t) const
{
	return (t - time_in_queue);
}

void Train::generateUnloadTime()
{
	std::uniform_real_distribution<double> distribution(3.50, 4.50);	
	unloadTime = distribution(generator);
}

void Train::setUnloadTime(double new_time)
{
	unloadTime = new_time;
}

void Train::setTrainID(int value)
{
	id = value;
}

void Train::setDeparted()
{
	departed = true;
}

void Train::setHoggedStatus(bool status)
{
	crew_hogged = status;
}

void Train::incHoggedCount()
{
	hogged_count++;
}

void Train::setTrainCrew(Crew& newCrew)
{
	currentCrew = &newCrew;
}

void Train::setTimeInQueue(double t)
{
	time_in_queue = t;
}

void Train::toString() const
{
	std::cout << "Train "  << getTrainID();
	std::cout << ", Arrival Time: " << std::setprecision(2) << std::fixed << getArrivalTime();
	std::cout << ", Unload Time:  " << std::setprecision(2) << std::fixed << getUnloadTime();
	
	std::cout << "\nTEST: " << getTrainCrew()->getCrewID() << "\n";
	std::cout << "**CREW STUFF** Arrival Time: " << getTrainCrew()->getArrivalTime() << ", Work Time: " << getTrainCrew()->getWorkTime() << std::endl;
	std::cout << ", Train's crew: " << getTrainCrew()->getCrewID() << "\n";
}
