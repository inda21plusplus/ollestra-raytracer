// https://raytracing.github.io/books/RayTracingInOneWeekend.html#thevec3class
#pragma once
#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3
{
private:
    double e[3];

public:
    Vec3() : e{0, 0, 0} {}
    Vec3(double _x, double _y, double _z) : e{_x, _y, _z} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double &operator[](int i) { return e[i]; }

    Vec3 &operator+=(const Vec3 &v)
    {
        e[0] += v.x();
        e[1] += v.y();
        e[2] += v.z();
        return *this;
    }

    Vec3 &operator*=(const double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3 &operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    double length() const
    {
        return sqrt(length_squared());
    }

    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    bool near_zero() const
    {
        const double z = 1e-8;
        return (fabs(e[0]) < z) && (fabs(e[1]) < z) && (fabs(e[2]) < z);
    }

    inline static Vec3 random()
    {
        return Vec3(random_double(), random_double(), random_double());
    }

    inline static Vec3 random(double min, double max)
    {
        return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

using Point3 = Vec3; // 3D point
using Color = Vec3;  // RGB color

inline std::ostream &operator<<(std::ostream &out, const Vec3 &v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline Vec3 operator*(double t, const Vec3 &v)
{
    return Vec3(t * v.x(), t * v.y(), t * v.z());
}

inline Vec3 operator*(const Vec3 &v, double t)
{
    return t * v;
}

inline Vec3 operator/(Vec3 v, double t)
{
    return (1 / t) * v;
}

inline double dot(const Vec3 &u, const Vec3 &v)
{
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v)
{
    return Vec3(u.y() * v.z() - u.z() * v.y(),
                u.z() * v.x() - u.x() * v.z(),
                u.x() * v.y() - u.y() * v.x());
}

inline Vec3 unit_vector(Vec3 v)
{
    return v / v.length();
}

inline Vec3 random_in_unit_sphere()
{
    while (true)
    {
        Vec3 p = Vec3::random(-1, 1);
        if (p.length_squared() >= 1)
            continue;
        return p;
    }
}

inline Vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

inline Vec3 ranomd_in_hemisphere(const Vec3 &normal)
{
    Vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline Vec3 reflect(const Vec3 &v, const Vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3 &uv, const Vec3 &n, double ei)
{
    double cos_theta = fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp = ei * (uv + cos_theta * n);
    Vec3 r_out_parallell = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;

    return r_out_perp + r_out_parallell;
}