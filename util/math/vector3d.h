#ifndef ZN_VECTOR3D_H
#define ZN_VECTOR3D_H

#include "vector3t.h"

namespace zylann {

// 3-dimensional vector using 64-bit floats, regardless on compiling options
typedef Vector3T<double> Vector3d;



namespace math {

inline Vector3d floor(const Vector3d a) {
	return Vector3d(Math::floor(a.x), Math::floor(a.y), Math::floor(a.z));
}

inline Vector3d ceil(const Vector3d a) {
	return Vector3d(Math::ceil(a.x), Math::ceil(a.y), Math::ceil(a.z));
}

inline Vector3d lerp(const Vector3d a, const Vector3d b, const double t) {
	return Vector3d(Math::lerp(a.x, b.x, t), Math::lerp(a.y, b.y, t), Math::lerp(a.z, b.z, t));
}

inline bool has_nan(const Vector3d &v) {
	return is_nan(v.x) || is_nan(v.y) || is_nan(v.z);
}

inline Vector3d normalized(const Vector3d &v) {
	const double lengthsq = length_squared(v);
	if (lengthsq == 0) {
		return Vector3d();
	} else {
		const double length = Math::sqrt(lengthsq);
		return v / length;
	}
}

inline Vector3d spherify (const Vector3d &v)
{
	double x2 = v.x * v.x;
	double y2 = v.y * v.y;
	double z2 = v.z * v.z;
	Vector3d s = Vector3d(0,0,0);
	s.x = v.x * Math::sqrt(1.0 - y2 / 2.0 - z2 / 2.0 + y2 * z2 / 3.0);
	s.y = v.y * Math::sqrt(1.0 - x2 / 2.0 - z2 / 2.0 + x2 * z2 / 3.0);
	s.z = v.z * Math::sqrt(1.0 - x2 / 2.0 - y2 / 2.0 + x2 * y2 / 3.0);

	return s;
}

inline Vector3d cubify(const Vector3d& position)
{
    double x,y,z;
    x = position.x;
    y = position.y;
    z = position.z;
	Vector3d finalPosition = Vector3d();
    double fx, fy, fz;
    fx = fabs(x);
    fy = fabs(y);
    fz = fabs(z);

    const double inverseSqrt2 = 0.70710676908493042;

    if (fy >= fx && fy >= fz) {
        double a2 = x * x * 2.0;
        double b2 = z * z * 2.0;
        double inner = -a2 + b2 -3;
        double innersqrt = -Math::sqrt((inner * inner) - 12.0 * a2);

        if(x == 0.0 || x == -0.0) { 
            finalPosition.x = 0.0; 
        }
        else {
            finalPosition.x = Math::sqrt(innersqrt + a2 - b2 + 3.0) * inverseSqrt2;
        }

        if(z == 0.0 || z == -0.0) {
            finalPosition.z = 0.0;
        }
        else {
            finalPosition.z = Math::sqrt(innersqrt - a2 + b2 + 3.0) * inverseSqrt2;
        }

        if(finalPosition.x > 1.0) finalPosition.x = 1.0;
        if(finalPosition.z > 1.0) finalPosition.z = 1.0;

        if(x < 0) finalPosition.x = -finalPosition.x;
        if(z < 0) finalPosition.z = -finalPosition.z;

        if (y > 0) {
            // top face
            finalPosition.y = 1.0;
        }
        else {
            // bottom face
            finalPosition.y = -1.0;
        }
    }
    else if (fx >= fy && fx >= fz) {
        double a2 = y * y * 2.0;
        double b2 = z * z * 2.0;
        double inner = -a2 + b2 -3;
        double innersqrt = -Math::sqrt((inner * inner) - 12.0 * a2);

        if(y == 0.0 || y == -0.0) { 
            finalPosition.y = 0.0; 
        }
        else {
            finalPosition.y = Math::sqrt(innersqrt + a2 - b2 + 3.0) * inverseSqrt2;
        }

        if(z == 0.0 || z == -0.0) {
            finalPosition.z = 0.0;
        }
        else {
            finalPosition.z = Math::sqrt(innersqrt - a2 + b2 + 3.0) * inverseSqrt2;
        }

        if(finalPosition.y > 1.0) finalPosition.y = 1.0;
        if(finalPosition.z > 1.0) finalPosition.z = 1.0;

        if(y < 0) finalPosition.y = -finalPosition.y;
        if(z < 0) finalPosition.z = -finalPosition.z;

        if (x > 0) {
            // right face
            finalPosition.x = 1.0;
        }
        else {
            // left face
            finalPosition.x = -1.0;
        }
    }
    else {
        double a2 = x * x * 2.0;
        double b2 = y * y * 2.0;
        double inner = -a2 + b2 -3;
        double innersqrt = -Math::sqrt((inner * inner) - 12.0 * a2);

        if(x == 0.0 || x == -0.0) { 
            finalPosition.x = 0.0; 
        }
        else {
            finalPosition.x = Math::sqrt(innersqrt + a2 - b2 + 3.0) * inverseSqrt2;
        }

        if(y == 0.0 || y == -0.0) {
            finalPosition.y = 0.0;
        }
        else {
            finalPosition.y = Math::sqrt(innersqrt - a2 + b2 + 3.0) * inverseSqrt2;
        }

        if(finalPosition.x > 1.0) finalPosition.x = 1.0;
        if(finalPosition.y > 1.0) finalPosition.y = 1.0;

        if(x < 0) finalPosition.x = -finalPosition.x;
        if(y < 0) finalPosition.y = -finalPosition.y;

        if (z > 0) {
            // front face
            finalPosition.z = 1.0;
        }
        else {
            // back face
            finalPosition.z = -1.0;
        }
    }
	return finalPosition;
}

inline double length(const Vector3d &v)
{
    return Math::sqrt(length_squared(v));
}

inline Vector3d cross(const Vector3d &v1, const Vector3d &v2)
{
	double x, y, z;
    x = v1.y * v2.z - v2.y * v1.z;
    y = (v1.x * v2.z - v2.x * v1.z) * -1.0;
    z = v1.x * v2.y - v2.z * v1.y;

    return Vector3d(x, y, z);
}

inline bool is_normalized(const Vector3d &v) {
	// use length_squared() instead of length() to avoid sqrt(), makes it more stringent.
	return Math::is_equal_approx(length_squared(v), 1, double(UNIT_EPSILON));
}


} // namespace math


} // namespace zylann

#endif // ZN_VECTOR3D_H
