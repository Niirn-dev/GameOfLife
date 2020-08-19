#pragma once

#include "FrameTimer.h"
#include <numeric>
#include <string>
#include <algorithm>
#include <sstream>

class Bencher
{
public:
	void Start()
	{
		ft.Mark();
	}
	bool End()
	{
		const float dt = ft.Mark();
		min  = std::min( min,dt );
		max  = std::max( max,dt );
		mean = ( mean + dt ) / 2.0f;
		++count;

		if ( count >= nSamples )
		{
			// Record old values
			prevMin		= min;
			prevMax		= max;
			prevMean	= mean;

			// Reset counter values
			min			= std::numeric_limits<float>::max();
			max			= 0.0f;
			mean		= 0.0f;
			count		= 0;
			// Signal the end of the recording
			return true;
		}
		return false;
	}
	operator std::wstring() const
	{
		std::wostringstream oss;
		oss << L"Sample size:" << nSamples << std::endl
			<< L"Min:\t" << prevMin * 1000 << L"ms" << std::endl
			<< L"Max:\t" << prevMax * 1000 << L"ms" << std::endl
			<< L"Mean:\t" << prevMean * 1000 << L"ms" << std::endl;
		return oss.str();
	}

private:
	FrameTimer ft;
	int		count		= 0;
	int		nSamples	= 10;
	float	min			= std::numeric_limits<float>::max();
	float	max			= 0.0f;
	float	mean		= 0.0f;
	float	prevMin		= std::numeric_limits<float>::signaling_NaN();
	float	prevMax		= std::numeric_limits<float>::signaling_NaN();
	float	prevMean	= std::numeric_limits<float>::signaling_NaN();
};
