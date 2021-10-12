#pragma once

#include "Vec3.h"

class Ray
{
private:
    Point3 orig;
    Vec3 dir;

public:
    Ray() {}
    Ray(const Point3 &_origin, const Vec3 &_direction)
        : orig(_origin), dir(_direction)
    {
    }

    Point3 origin() const { return orig; }
    Vec3 direction() const { return dir; }

    Point3 at(double t) const
    {
        return orig + t * dir;
    }
};
