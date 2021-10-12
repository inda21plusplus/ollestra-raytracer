#pragma once

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable
{
private:
    Point3 center;
    double radius;

public:
    Sphere() {}
    Sphere(Point3 c, double r) : center(c), radius(r){};

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
};

bool Sphere::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    Vec3 oc = ray.origin() - center;
    double a = ray.direction().length_squared();
    double half_b = dot(oc, ray.direction());
    double c = oc.length_squared() - radius * radius;

    double discriminant = half_b * half_b - a * c;
    if (discriminant < 0)
        return false;

    double sqrtd = sqrt(discriminant);

    // Nearest root in acceptable range.
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max)
    {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max)
            return false;
    }

    rec.t = root;
    rec.p = ray.at(rec.t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(ray, outward_normal);

    return true;
}