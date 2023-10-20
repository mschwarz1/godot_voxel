#ifndef ZYLANN_VECTOR3F_H
#define ZYLANN_VECTOR3F_H

#include "../errors.h"
#include "vector3t.h"
#include <iosfwd>

namespace zylann {


typedef float precision_t; 

// 32-bit float precision 3D vector.
// Because Godot's `Vector3` uses `real_t`, so when `real_t` is `double` it forces some things to use double-precision
// vectors while they dont need that amount of precision. This is also a problem for some third-party libraries
// that do not support `double` as a result.
typedef Vector3T<precision_t> Vector3f;

namespace math {

inline Vector3f floor(const Vector3f a) {
	return Vector3f(Math::floor(a.x), Math::floor(a.y), Math::floor(a.z));
}

inline Vector3f ceil(const Vector3f a) {
	return Vector3f(Math::ceil(a.x), Math::ceil(a.y), Math::ceil(a.z));
}

inline Vector3f lerp(const Vector3f a, const Vector3f b, const precision_t t) {
	return Vector3f(Math::lerp(a.x, b.x, t), Math::lerp(a.y, b.y, t), Math::lerp(a.z, b.z, t));
}

inline bool has_nan(const Vector3f &v) {
	return is_nan(v.x) || is_nan(v.y) || is_nan(v.z);
}

inline Vector3f normalized(const Vector3f &v) {
	const precision_t lengthsq = length_squared(v);
	if (lengthsq == 0) {
		return Vector3f();
	} else {
		const precision_t length = Math::sqrt(lengthsq);
		return v / length;
	}
}
const double isqrt2 = 0.70710676908493042;

inline Vector3f spherify (const Vector3f &v)
{
	precision_t x2 = v.x * v.x;
	precision_t y2 = v.y * v.y;
	precision_t z2 = v.z * v.z;
	Vector3f s = Vector3f(0,0,0);
	s.x = v.x * Math::sqrt(1.0 - y2 / 2.0 - z2 / 2.0 + y2 * z2 / 3.0);
	s.y = v.y * Math::sqrt(1.0 - x2 / 2.0 - z2 / 2.0 + x2 * z2 / 3.0);
	s.z = v.z * Math::sqrt(1.0 - x2 / 2.0 - y2 / 2.0 + x2 * y2 / 3.0);

	return s;
}

inline Vector3f cubify(const Vector3f& position)
{
    precision_t x,y,z;
    x = position.x;
    y = position.y;
    z = position.z;
	Vector3f finalPosition = Vector3f();
    precision_t fx, fy, fz;
    fx = fabsf(x);
    fy = fabsf(y);
    fz = fabsf(z);

    const precision_t inverseSqrt2 = 0.70710676908493042;

    if (fy >= fx && fy >= fz) {
        precision_t a2 = x * x * 2.0;
        precision_t b2 = z * z * 2.0;
        precision_t inner = -a2 + b2 -3;
        precision_t innersqrt = -Math::sqrt((inner * inner) - 12.0 * a2);

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
        precision_t a2 = y * y * 2.0;
        precision_t b2 = z * z * 2.0;
        precision_t inner = -a2 + b2 -3;
        precision_t innersqrt = -Math::sqrt((inner * inner) - 12.0 * a2);

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
        precision_t a2 = x * x * 2.0;
        precision_t b2 = y * y * 2.0;
        precision_t inner = -a2 + b2 -3;
        precision_t innersqrt = -Math::sqrt((inner * inner) - 12.0 * a2);

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


inline Vector3f cross(const Vector3f &v1, const Vector3f &v2)
{
	double x, y, z;
    x = v1.y * v2.z - v2.y * v1.z;
    y = (v1.x * v2.z - v2.x * v1.z) * -1.0;
    z = v1.x * v2.y - v2.z * v1.y;

    return Vector3f(x, y, z);
}

inline Vector3f normalized(Vector3f v, float &out_length) {
	const float lengthsq = length_squared(v);
	if (lengthsq == 0) {
		out_length = 0.f;
		return Vector3f();
	} else {
		const float length = Math::sqrt(lengthsq);
		out_length = length;
		return v / length;
	}
}

inline bool is_normalized(const Vector3f &v) {
	// use length_squared() instead of length() to avoid sqrt(), makes it more stringent.
	return Math::is_equal_approx(length_squared(v), 1, precision_t(UNIT_EPSILON));
}

} // namespace math

std::stringstream &operator<<(std::stringstream &ss, const Vector3f &v);

} // namespace zylann

#endif // ZYLANN_VECTOR3F_H
