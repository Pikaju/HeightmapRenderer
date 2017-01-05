#include "Noise.h"

#include <algorithm>
#include <numeric>
#include <random>

namespace util {

	Noise::Noise(unsigned int seed) : m_p(), m_seed(seed)
	{
		m_p.resize(256);
		std::iota(m_p.begin(), m_p.end(), 0);
		std::default_random_engine engine(seed);
		std::shuffle(m_p.begin(), m_p.end(), engine);
		m_p.insert(m_p.end(), m_p.begin(), m_p.end());
	}
	
	Noise::~Noise()
	{
	}

	float Noise::noise(float x) const
	{
		int ux = (int)floor(x);
		int h0 = m_p[std::hash<int>()(ux)& 255];
		int h1 = m_p[std::hash<int>()(ux + 1) & 255];
		float ix = fade(x - floor(x));
		return lerp(ix, (float)h0, (float)h1) / 255.0f * 2.0f - 1.0f;
	}
	
	float Noise::noise(float x, float y) const
	{
		int ux = (int)floor(x);
		int uy = (int)floor(y);
		int h0 = m_p[std::hash<int>()(ux) & 255 + m_p[std::hash<int>()(uy) & 255]];
		int h1 = m_p[std::hash<int>()(ux + 1) & 255 + m_p[std::hash<int>()(uy) & 255]];
		int h2 = m_p[std::hash<int>()(ux) & 255 + m_p[std::hash<int>()(uy + 1) & 255]];
		int h3 = m_p[std::hash<int>()(ux + 1) & 255 + m_p[std::hash<int>()(uy + 1) & 255]];
		float ix = fade(x - floor(x));
		float iy = fade(y - floor(y));
		return lerp(ix, lerp(iy, (float)h0, (float)h2), lerp(iy, (float)h1, (float)h3)) / 255.0f * 2.0f - 1.0f;
	}

}