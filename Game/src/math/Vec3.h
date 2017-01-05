#pragma once

namespace math {

	template<typename T>
	class Vec3
	{
	public:
		T x;
		T y;
		T z;

		Vec3(const T& v = (T)0);
		Vec3(const T& x, const T& y, const T& z);
		~Vec3();

		template<typename T2> T2 getLength() const;
		inline T getLengthSquared() const { return x * x + y * y + z * z; }
		void normalize();
		Vec3<T> normalized() const;
		inline T dot(const Vec3<T>& v) const { return x * v.x + y + v.y + z * v.z; }
		inline Vec3<T> cross(const Vec3<T>& v) const { return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

		template<typename T2> inline Vec3<T2> operator+(const T2& v) const { return Vec3<T2>((T2)x + v, (T2)y + v, (T2)z + v); }
		template<typename T2> inline Vec3<T2> operator-(const T2& v) const { return Vec3<T2>((T2)x - v, (T2)y - v, (T2)z - v); }
		template<typename T2> inline Vec3<T2> operator*(const T2& v) const { return Vec3<T2>((T2)x * v, (T2)y * v, (T2)z * v); }
		template<typename T2> inline Vec3<T2> operator/(const T2& v) const { return Vec3<T2>((T2)x / v, (T2)y / v, (T2)z / v); }

		inline Vec3<T> operator+(const Vec3<T>& v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
		inline Vec3<T> operator-(const Vec3<T>& v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
		inline Vec3<T> operator*(const Vec3<T>& v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); }
		inline Vec3<T> operator/(const Vec3<T>& v) const { return Vec3<T>(x / v.x, y / v.y, z / v.z); }

		inline void operator+=(const T& v) { x += v; y += v; z += v; }
		inline void operator-=(const T& v) { x -= v; y -= v; z -= v; }
		inline void operator*=(const T& v) { x *= v; y *= v; z *= v; }
		inline void operator/=(const T& v) { x /= v; y /= v; z /= v; }

		inline void operator+=(const Vec3<T>& v) { x += v.x; y += v.y; z += v.z; }
		inline void operator-=(const Vec3<T>& v) { x -= v.x; y -= v.y; z -= v.z; }
		inline void operator*=(const Vec3<T>& v) { x *= v.x; y *= v.y; z *= v.z; }
		inline void operator/=(const Vec3<T>& v) { x /= v.x; y /= v.y; z /= v.z; }

		inline bool operator==(const Vec3<T>& v) const { return x == v.x && y == v.y && z == v.z; }
	};

	typedef Vec3<unsigned char> Vec3b;
	typedef Vec3<int> Vec3i;
	typedef Vec3<float> Vec3f;
	typedef Vec3<double> Vec3d;

}