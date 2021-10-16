#pragma once
#include "Hittable.h"
#include "AABB.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class HittableList : public Hittable
{
public:
    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<Hittable> object) { objects.push_back(object); }

    virtual bool hit(
        const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool boundingBox(double time0, double time1, AABB &OutBox) const override;

public:
    std::vector<shared_ptr<Hittable>> objects;
};

bool HittableList::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    HitRecord tempRecord;
    bool hitAnything = false;
    double closest_yet = t_max;

    for (const auto &object : objects)
    {
        if (object->hit(ray, t_min, closest_yet, tempRecord))
        {
            hitAnything = true;
            closest_yet = tempRecord.t;
            rec = tempRecord;
        }
    }

    return hitAnything;
}

bool HittableList::boundingBox(double time0, double time1, AABB &OutBox) const
{
    if (objects.empty())
        return false;

    AABB tBox;
    bool first_box = true;

    for (const auto &object : objects)
    {
        if (!object->boundingBox(time0, time1, tBox))
            return false;
        OutBox = first_box ? tBox : surroundingBox(OutBox, tBox);
        first_box = false;
    }

    return true;
}