//
// Define the hyperboloid geometry shape.
//

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SHAPES_HYPERBOLOID_H
#define PBRT_SHAPES_HYPERBOLOID_H

#include "shape.h"

namespace pbrt {

// Hyperboloid Declarations
class Hyperboloid : public Shape {
public:
    // Hyperboloid Public Methods
    Hyperboloid(const Transform *o2w, const Transform *w2o, bool ro,
                const Point3f &point1, const Point3f &point2, Float tm);
    Bounds3f ObjectBound() const;
    bool Intersect(const Ray &ray, Float *tHit, SurfaceInteraction *isect,
                   bool testAlphaTexture) const;
    bool IntersectP(const Ray &ray, bool testAlphaTexture) const;
    Float Area() const;

protected:
    // Hyperboloid Private Data
    Point3f p1, p2;
    Float zMin, zMax;
    Float phiMax;
    Float rMax;
    Float ah, ch;
};

}   // namespace pbrt

#endif //PBRT_SHAPES_HYPERBOLOID_H
