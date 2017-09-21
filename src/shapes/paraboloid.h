//
// Define the paraboloid geometry shape.
//

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SHAPES_PARABOLOID_H
#define PBRT_SHAPES_PARABOLOID_H

#include "shape.h"

namespace pbrt {

// Paraboloid Declarations
class Paraboloid : public Shape {
public:
    // Paraboloid Public Methods
    Paraboloid(const Transform *o2w, const Transform *w2o,
               bool reverseOrientation, Float radius, Float z0, Float z1,
               Float phiMax);
    Bounds3f ObjectBound() const;
    bool Intersect(const Ray &ray, Float *tHit, SurfaceInteraction *isect,
                   bool testAlphaTexture) const;
    bool IntersectP(const Ray &ray, bool testAlphaTexture) const;
    Float Area() const;

protected:
    // Paraboloid Private Data
    const Float radius, zMin, zMax, phiMax;
};

}   // namespace pbrt

#endif //PBRT_SHAPES_PARABOLOID_H
