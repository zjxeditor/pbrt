//
// Define the cone geometry shape.
//

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SHAPES_CONE_H
#define PBRT_SHAPES_CONE_H

#include "shape.h"

namespace pbrt {

// Cone Declarations
class Cone : public Shape {
public:
    // Cone Public Methods
    Cone(const Transform *o2w, const Transform *w2o, bool reverseOrientation,
         Float height, Float radius, Float phiMax);
    Bounds3f ObjectBound() const;
    bool Intersect(const Ray &ray, Float *tHit, SurfaceInteraction *isect,
                   bool testAlphaTexture) const;
    bool IntersectP(const Ray &ray, bool testAlphaTexture) const;
    Float Area() const;

private:
    // Cone Private Data
    const Float radius, height, phiMax;
};

}   // namespace pbrt

#endif //PBRT_SHAPES_CONE_H
