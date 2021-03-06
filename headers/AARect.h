#pragma once

#include "Commons.h"

#include "Vec3.h"
#include "Ray.h"

#include "Hittable.h"

class XYRect : public Hittable
{
private:
    shared_ptr<Material> mp;
    double x0, x1, y0, y1, k;

public:
    XYRect();
    XYRect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<Material> mat)
        : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {}

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;

    virtual bool boundingBox(double time0, double time1, AABB &OutBox) const override
    {
        OutBox = AABB(Point3(x0, y0, k - 1e-4), Point3(x1, y1, k + 1e-4));
        return true;
    }
};

class XZRect : public Hittable
{
private:
    shared_ptr<Material> mp;
    double x0, x1, z0, z1, k;

public:
    XZRect();
    XZRect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<Material> mat)
        : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {}

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;

    virtual bool boundingBox(double time0, double time1, AABB &OutBox) const override
    {
        OutBox = AABB(Point3(x0, k - 1e-4, z0), Point3(x1, k + 1e-4, z1));
        return true;
    }
};

class YZRect : public Hittable
{
private:
    shared_ptr<Material> mp;
    double y0, y1, z0, z1, k;

public:
    YZRect();
    YZRect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<Material> mat)
        : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {}

    virtual bool hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;

    virtual bool boundingBox(double time0, double time1, AABB &OutBox) const override
    {
        OutBox = AABB(Point3(k - 1e-4, y0, z0), Point3(k + 1e-4, y1, z1));
        return true;
    }
};

// Functions
bool XYRect::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    auto t = (k - ray.origin().z()) / ray.direction().z();
    if (t < t_min || t > t_max)
        return false;
    auto x = ray.origin().x() + t * ray.direction().x();
    auto y = ray.origin().y() + t * ray.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    auto outward_normal = Vec3(0, 0, 1);
    rec.set_face_normal(ray, outward_normal);
    rec.mat_ptr = mp;
    rec.p = ray.at(t);
    return true;
}

bool XZRect::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    auto t = (k - ray.origin().y()) / ray.direction().y();
    if (t < t_min || t > t_max)
        return false;
    auto x = ray.origin().x() + t * ray.direction().x();
    auto z = ray.origin().z() + t * ray.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    auto outward_normal = Vec3(0, 1, 0);
    rec.set_face_normal(ray, outward_normal);
    rec.mat_ptr = mp;
    rec.p = ray.at(t);
    return true;
}

bool YZRect::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    auto t = (k - ray.origin().x()) / ray.direction().x();
    if (t < t_min || t > t_max)
        return false;
    auto y = ray.origin().y() + t * ray.direction().y();
    auto z = ray.origin().z() + t * ray.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    auto outward_normal = Vec3(1, 0, 0);
    rec.set_face_normal(ray, outward_normal);
    rec.mat_ptr = mp;
    rec.p = ray.at(t);
    return true;
}
