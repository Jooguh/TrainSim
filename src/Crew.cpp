// Kenny Park

#include "Crew.hpp"
#include "Train.hpp"

Crew::Crew() : generator(std::random_device()()), is_hogged_out(false){
	
	std::uniform_real_distribution<double> work_distribution(6.0, 11.0);
	work_time = work_distribution(generator);
	std::uniform_real_distribution<double> arrival_distribution(2.50, 3.50);
	arrival_time = arrival_distribution(generator);
	
}

Crew::Crew(double pickWorkTime, double pickArrivalTime, int pickID) : work_time(pickWorkTime), id(pickID), is_hogged_out(false){

}

Crew::Crew(int pickID) : generator(std::random_device()()), id(pickID), is_hogged_out(false) {

	std::uniform_real_distribution<double> work_distribution(6.0, 11.0);
	work_time = work_distribution(generator);
	std::uniform_real_distribution<double> arrival_distribution(2.50, 3.50);
	arrival_time = arrival_distribution(generator);
}

Crew::Crew(double pickArrivalTime, int pickID) : generator(std::random_device()()), id(pickID), arrival_time(pickArrivalTime), is_hogged_out(false) {

	std::uniform_real_distribution<double> work_distribution(6.0, 11.0);
	work_time = work_distribution(generator);
	
}


double Crew::getWorkTime() const
{
	return work_time;
}

double Crew::getArrivalTime() const
{
	return arrival_time;
}

double Crew::getStartTime() const
{
	return start_time;
}

double Crew::getRemainingWork() const
{
	return remaining_work_time;
}

int Crew::getCrewID() const
{
	return id;
}

bool Crew::getHoggedOut() const
{
	return is_hogged_out;
}

bool Crew::hasDeparted() const
{
	return departed;
}

void Crew::generateWorkTime()
{
	std::uniform_real_distribution<double> work_distribution(6.0, 11.0);
	work_time = work_distribution(generator);
}

void Crew::generateArrivalTime()
{
	std::uniform_real_distribution<double> arrival_distribution(2.50, 3.50);
	arrival_time = arrival_distribution(generator);
}

void Crew::setCrewID(int value)
{
	id = value;
}

void Crew::setHoggedOutStatus(bool status)
{
	is_hogged_out = status;
}

void Crew::setArrivalTime(double a)
{
	arrival_time = a;
}

void Crew::setWorkTime(double w)
{
	work_time = w;
}

void Crew::setStartTime(double s)
{
	start_time = s;
}

void Crew::setDeparted()
{
	departed = true;
}

void Crew::updateRemainingWork(double currentTime)
{
	remaining_work_time = work_time - (currentTime - start_time);
}

void Crew::setTrain(Train& newTrain)
{
	currentTrain = &newTrain;
}

Train* Crew::getTrain() const
{
	return currentTrain;
}

std::string Crew::toString(void) const
{
	std::stringstream str;

	str << "Crew " << getCrewID();
	std::cout << std::endl;
	return str.str();

}
