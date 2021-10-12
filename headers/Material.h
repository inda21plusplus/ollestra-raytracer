#pragma once

#include "Commons.h"
#include "Ray.h"
#include "Hittable.h"

class Material
{
public:
    virtual bool scatter(
        const Ray &ray_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
private:
    Color albedo;

public:
    Lambertian(const Color &a) : albedo(a) {}

    virtual bool scatter(const Ray &ray_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        Vec3 scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

class Metal : public Material
{
private:
    Color albedo;

public:
    Metal(const Color &a) : albedo(a) {}

    virtual bool scatter(const Ray &ray_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        Vec3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};