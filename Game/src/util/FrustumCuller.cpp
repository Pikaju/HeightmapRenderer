#include "FrustumCuller.h"

namespace util {

	FrustumCuller::FrustumCuller()
	{
	}

	FrustumCuller::~FrustumCuller()
	{
	}

	void FrustumCuller::extract(const math::Mat4f& m)
	{
		m_p[0] = Plane(math::Vec3f(m.get(0, 3) + m.get(0, 0), m.get(1, 3) + m.get(1, 0), m.get(2, 3) + m.get(2, 0)), m.get(3, 3) + m.get(3, 0));
		m_p[1] = Plane(math::Vec3f(m.get(0, 3) - m.get(0, 0), m.get(1, 3) - m.get(1, 0), m.get(2, 3) - m.get(2, 0)), m.get(3, 3) - m.get(3, 0));
		m_p[2] = Plane(math::Vec3f(m.get(0, 3) + m.get(0, 1), m.get(1, 3) + m.get(1, 1), m.get(2, 3) + m.get(2, 1)), m.get(3, 3) + m.get(3, 1));
		m_p[3] = Plane(math::Vec3f(m.get(0, 3) - m.get(0, 1), m.get(1, 3) - m.get(1, 1), m.get(2, 3) - m.get(2, 1)), m.get(3, 3) - m.get(3, 1));
		m_p[4] = Plane(math::Vec3f(m.get(0, 3) + m.get(0, 2), m.get(1, 3) + m.get(1, 2), m.get(2, 3) + m.get(2, 2)), m.get(3, 3) + m.get(3, 2));
		m_p[5] = Plane(math::Vec3f(m.get(0, 3) - m.get(0, 2), m.get(1, 3) - m.get(1, 2), m.get(2, 3) - m.get(2, 2)), m.get(3, 3) - m.get(3, 2));
		for (unsigned int i = 0; i < 6; i++) {
			m_p[i].normalize();
		}
	}

	bool FrustumCuller::sphereInFrustum(const math::Vec3f& position, float radius) const
	{
		for (unsigned int i = 0; i < 6; i++) {
			if (m_p[i].normal.dot(position) + m_p[i].d <= -radius) {
				return false;
			}
		}
		return true;
	}

}