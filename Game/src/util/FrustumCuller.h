#pragma once

#include "../math/Vec3.h"
#include "../math/Mat4.h"

namespace util {

	class Plane
	{
	public:
		math::Vec3f normal;
		float d;

		Plane() : normal(), d(0.0f) {  }
		Plane(const math::Vec3f& normal, float d) : normal(normal), d(d) {  }
		~Plane() {  }

		void normalize() { float l = normal.getLength<float>(); if (l == 0.0f) return; normal /= l; d /= l; }
	};

	class FrustumCuller
	{
	public:
		FrustumCuller();
		~FrustumCuller();

		void extract(const math::Mat4f& m);

		bool sphereInFrustum(const math::Vec3f& position, float radius) const;
	private:
		Plane m_p[6];
	};

}