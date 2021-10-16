#pragma once

#include "Commons.h"
#include "Ray.h"
#include "Hittable.h"
#include "Texture.h"

class Material
{
public:
    virtual bool scatter(
        const Ray &ray_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const = 0;

    virtual Color emitted(double u, double v, const Point3 &p) const
    {
        return Color(0.01, 0.01, 0.01);
    }
};

class Lambertian : public Material
{
private:
    shared_ptr<Texture> albedo;

public:
    Lambertian(const Color &a) : albedo(make_shared<SolidColor>(a)) {}
    Lambertian(shared_ptr<Texture> a) : albedo(a) {}

    virtual bool scatter(const Ray &ray_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        Vec3 scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
};

class Metal : public Material
{
private:
    Color albedo;
    double fuzz;

public:
    Metal(const Color &a, double f) : albedo(a), fuzz(f) {}
    Metal(const Color &a) : albedo(a), fuzz(0) {}

    virtual bool scatter(const Ray &ray_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        Vec3 reflected = reflect(unit_vector(ray_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};

class Dielectric : public Material
{
private:
    double ir;

public:
    Dielectric(double _ir) : ir(_ir) {}

    virtual bool scatter(const Ray &ray_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        attenuation = Color(1, 1, 1);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        Vec3 unit_direction = unit_vector(ray_in.direction());
        double cos_th = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_th = sqrt(1.0 - cos_th * cos_th);

        bool _refract = refraction_ratio * sin_th <= 1.0;
        Vec3 direction;
        if (_refract || reflectance(cos_th, refraction_ratio) < random_double())
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        else
            direction = reflect(unit_direction, rec.normal);

        scattered = Ray(rec.p, direction);
        return true;
    }

private:
    static double reflectance(double cos, double ref_idx)
    {
        // Schlick's approximation
        auto r_0 = (1 - ref_idx) / (1 + ref_idx);
        r_0 = r_0 * r_0;
        return r_0 + (1 - r_0) * pow((1 - cos), 5);
    }
};

class DiffuseLight : public Material
{
private:
    shared_ptr<Texture> emit;

public:
    DiffuseLight(shared_ptr<Texture> a) : emit(a) {}
    DiffuseLight(Color c) : emit(make_shared<SolidColor>(c)) {}

    virtual bool scatter(
        const Ray &ray_in, const HitRecord &rec, Color &attenuation, Ray &scattered) const override
    {
        return false;
    }

    virtual Color emitted(double u, double v, const Point3 &p) const override
    {
        return emit->value(u, v, p);
    }
};