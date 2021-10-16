#pragma once

#include <algorithm>

#include "Commons.h"
#include "Vec3.h"
#include "Ray.h"

#include "Hittable.h"
#include "HittableList.h"

class BVHNode : public Hittable
{
private:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    AABB box;

public:
    BVHNode();
    BVHNode(const HittableList &list, double time0, double time1)
        : BVHNode(list.objects, 0, list.objects.size(), time0, time1) {}

    BVHNode(const std::vector<shared_ptr<Hittable>> &srcObjects,
            size_t start, size_t end, double time0, double time1)
    {
        auto objects = srcObjects;

        int axis = random_int(0, 2);
        auto comparator = (axis == 0)   ? boxXCompare
                          : (axis == 1) ? boxYCompare
                                        : boxZCcompare;
        size_t objectSpan = end - start;

        if (objectSpan == 1)
        {
            left = right = objects[start];
        }
        else if (objectSpan == 2)
        {
            if (comparator(objects[start], objects[start + 1]))
            {
                left = objects[start];
                right = objects[start + 1];
            }
            else
            {
                left = objects[start + 1];
                right = objects[start];
            }
        }
        else
        {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            double mid = start + objectSpan / 2;
            left = make_shared<BVHNode>(objects, start, mid, time0, time1);
            right = make_shared<BVHNode>(objects, mid, end, time0, time1);
        }

        AABB box_left, box_right;

        if (!left->boundingBox(time0, time1, box_left) || !right->boundingBox(time0, time1, box_right))
            std::cerr << "No bounding box in bvh_node constructor.\n";

        box = surroundingBox(box_left, box_right);
    }

    virtual bool
    hit(
        const Ray &ray, double t_min, double t_max, HitRecord &rec) const override;
    virtual bool boundingBox(double time0, double time1, AABB &OutBox) const override;
};

bool BVHNode::boundingBox(double time0, double time1, AABB &OutBox) const
{
    OutBox = box;
    return true;
}

bool BVHNode::hit(const Ray &ray, double t_min, double t_max, HitRecord &rec) const
{
    if (!box.hit(ray, t_min, t_max))
        return false;

    bool hit_left = left->hit(ray, t_min, t_max, rec);
    bool hit_right = right->hit(ray, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

inline bool boxCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis)
{
    AABB box_a;
    AABB box_b;

    if (!a->boundingBox(0, 0, box_a) || !b->boundingBox(0, 0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min()[axis] < box_b.min()[axis];
}

bool boxXCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
    return boxCompare(a, b, 0);
}

bool boxYCompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
    return boxCompare(a, b, 1);
}

bool boxZCcompare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
{
    return boxCompare(a, b, 2);
}