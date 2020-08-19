#include "FrameTimer.h"

using namespace std::chrono;

FrameTimer::FrameTimer() :
	m_last( steady_clock::now() )
{

}

float FrameTimer::Mark()
{
	auto old = m_last;
	m_last = steady_clock::now();
	duration<float> d = m_last - old;
	return d.count();
}
