#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Vec3.h"

class Sphere : public Hittable
{
private:
    Point3 center;
    double radius;
    shared_ptr<Material> material;

public:
    Sphere() {}
    Sphere(Point3 c, double r, shared_ptr<Material> mat) : center(c), radius(r), material(mat){};

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool boundingBox(double time0, double time1, AABB &OutBox) const override;

private:
    static void getSphereUV(const Point3 &p, double &u, double &v)
    {
        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
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
    getSphereUV(outward_normal, rec.u, rec.v);
    rec.mat_ptr = material;

    return true;
}
bool Sphere::boundingBox(double time0, double time1, AABB &OutBox) const
{
    OutBox = AABB(
        center - Vec3(radius, radius, radius),
        center + Vec3(radius, radius, radius));
    return true;
}