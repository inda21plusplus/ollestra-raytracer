#pragma once

#include "Commons.h"
#include "Ray.h"
#include "Vec3.h"

class Camera
{
private:
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;

public:
    Camera(double vfov, double ar)
    {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto vp_height = 2.0 * h;
        auto vp_width = ar * vp_height;
        double focal_length = 1;

        origin = Point3(0, 0, 0);
        horizontal = Vec3(vp_width, 0, 0);
        vertical = Vec3(0, vp_height, 0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
    }

    Ray getRay(double u, double v) const
    {
        return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }
};