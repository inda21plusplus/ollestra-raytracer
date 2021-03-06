#pragma once

#include "Ray.h"
#include "AABB.h"

class Material;

struct HitRecord
{
    Point3 p;
    Vec3 normal;
    shared_ptr<Material> mat_ptr;
    double t;
    double u;
    double v;
    bool front_face;

    inline void set_face_normal(const Ray &r, const Vec3 &outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const = 0;
    virtual bool boundingBox(double time0, double time1, AABB &OutBox) const = 0;
};