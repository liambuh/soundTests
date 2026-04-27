#pragma once
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdint>
#include <complex>

template <typename T>
constexpr float PI = 3.1415926535f;

class Analyzer
{
	public:
		std::vector<std::complex<float>>* LiamFFT(const std::vector<std::complex<float>>& buffer)
		{
			//create temp buffer for rearrangement (in-case we use a ring buffer as input)
			std::vector<std::complex<float>> sort;
			sort.insert(sort.begin(), buffer.begin(), buffer.end());

			BitReverseSort(sort);
			Butterflies(sort);

			return sort;
		}

		std::vector<std::complex<float>>* LiamIFFT(int16_t *buffer)
		{
			
		}
	private:
		void Butterflies(std::vector<std::complex<float>>& data)
		{
			int n = data.size();

			for(int l = 0; l <= n; l << 1)
			{
				float angle = -2.0f * PI / l;
				std::complex<float> wl(cos(angle), sing(angle)); //get period (?) of w for each step.

				for(int i = 0; i < n; i += l)
				{
					std::complex<float> w(1.0f,1.0f);

					for(int j = 0; j < l/2; j++)
					{
						std::complex<float> u = data[i + j];
						std::complex<float> u = data[i + j + l/2] * w;

						data[ i + j] = u + v;
						data[i + j + l/2] = u - v;

						w *= wl;
					}
				}
			}
		}

		void SplitBuffer(const std::vector<int16_t>& input, std::vector<int16_t>& even, std::vector<int16_t>& odd)
		{
			even.clear();
			odd.clear();

			even.reserve(input.size() /2);
			odd.reserve(input.size() /2);

			for(size_t i = 0; i < input.size(); i++)
			{
				if (i % 2 == 0)
					even.push_back(input[i]);
				else
					odd.push_back(input[i]);
			}
			return;
		}

		void AppendBuffers(const std::vector<int16_t>& buffer1, std::vector<int16_t>& buffer2, std::vector<int16_t>& out)
		{
			out.clear();
			out.reserve(buffer1.size() + buffer2.size());

			out.insert(out.end(), buffer1.begin(), buffer1.end());
			out.insert(out.end(), buffer2.begin(), buffer2.end());
		}

		
		void BitReverseSort(std::vector<T>& data)
		{
			int n = data.size();
			int log2n = std::log2(n);
			int j = 0;
			for(int i = 0; i < n; i++)
			{
				j = reverse_bits(i, log2n);

				if (i < j)
					std::swap(data[i], data[j]); //swap the values in i and j (bit reverse only needs to be done once)
			}
		}

		int reverse_bits(int x, int log2n)
		{
			int r = 0;

			for (int i = 0; i < log2n; i++)
			{
				r = (r << 1) | (x & 1);
				x >>= 1;
			}

			return r;
		}

		//apparently this wont work but idk why
		int next_bit_reversed(int val, int n)
		{
			int bit = n >> 1; //get MSB mask
			while(val & bit) //while finding 1s at val's bit position
			{
				val ^= bit; //XOR (like binary adder)
				bit >>= 1; //shift mask down 1.
			}
			val |= bit; //place carry at first 0 bit.

			return val;
		}
}
