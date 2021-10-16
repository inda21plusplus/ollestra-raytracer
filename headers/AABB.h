#pragma once

#include "Vec3.h"
#include "Ray.h"

class AABB
{
private:
    Point3 maximum;
    Point3 minimum;

public:
    AABB() {}
    AABB(const Point3 &a, const Point3 &b) : maximum(a), minimum(b) {}

    Point3 min() const { return minimum; }
    Point3 max() const { return maximum; }

    bool hit_alt(const Ray &ray, double t_min, double t_max) const
    {
        for (size_t i = 0; i < 3; i++)
        {
            double t0 = fmin((minimum[i] - ray.origin()[i]) / ray.direction()[i],
                             (maximum[i] - ray.origin()[i]) / ray.direction()[i]);
            double t1 = fmax((minimum[i] - ray.origin()[i]) / ray.direction()[i],
                             (maximum[i] - ray.origin()[i]) / ray.direction()[i]);

            t_min = fmax(t0, t_min);
            t_max = fmin(t1, t_max);
            if (t_max <= t_min)
                return false;
        }
        return true;
    }

    bool hit(const Ray &ray, double t_min, double t_max) const
    {
        for (size_t i = 0; i < 3; i++)
        {
            double invdir = 1.0 / ray.direction()[i];
            double t0 = (minimum[i] - ray.origin()[i]) * invdir;
            double t1 = (maximum[i] - ray.origin()[i]) * invdir;

            if (invdir < 0.0f)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;

            if (t_max <= t_min)
                return false;
        }

        return true;
    }
};

inline AABB surroundingBox(AABB box0, AABB box1)
{
    Point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));

    Point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));

    return AABB(small, big);
}