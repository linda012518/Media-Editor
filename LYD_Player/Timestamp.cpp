#include "Timestamp.h"

using namespace std::chrono;

namespace lyd
{
	Timestamp::Timestamp()
	{
		update();
	}

	Timestamp::~Timestamp()
	{
	}

	void Timestamp::update()
	{
		_begin = high_resolution_clock::now();
	}

	double Timestamp::getElapsedSecond()
	{
		return  getElapsedTimeInMicroSec() * 0.000001;
	}

	double Timestamp::getElapsedTimeInMilliSec()
	{
		return this->getElapsedTimeInMicroSec() * 0.001;
	}

	long long Timestamp::getElapsedTimeInMicroSec()
	{
		return duration_cast<microseconds>(high_resolution_clock::now() - _begin).count();
	}

}