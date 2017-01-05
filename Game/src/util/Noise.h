#pragma once

#include <vector>

namespace util {

	class Noise
	{
	public:
		Noise(unsigned int seed);
		~Noise();

		float noise(float x) const;
		float noise(float x, float y) const;
	protected:
		inline float lerp(float x, float a, float b) const { return x * (b - a) + a; }
		inline float fade(float x) const { return x * x * x * (x * (x * 6.0f - 15.0f) + 10.0f); }
		inline float grad(int hash, float x, float y, float z) const {
			int h = hash & 15;
			float u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
			return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
		}
	private:
		std::vector<int> m_p;
		unsigned int m_seed;
	};

}