#pragma once
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdint>
#include <complex>

class Filters
{
	public:
		static std::vector<std::complex<float>> LowPass(const std::vector<std::complex<float>>& _data)
		{
			//copy to fresh vector
			std::vector<std::complex<float>> data[_data.size()];
			
			float dt = 0.5f;
			float rc = 0.5f;

			float a = dt / (rc + dt);
			/*
			for(int i = 1; i < data.size(); i++)
			{
				data[i] = a * _data[i] + (1.0f - a) * data[i-1];
			}
			*/
			return data;
		}
	private:
		
};
