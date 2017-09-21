//
// Define the disk geometry shape.
//

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SHAPES_DISK_H
#define PBRT_SHAPES_DISK_H

#include "shape.h"

namespace pbrt {

// Disk Declarations
class Disk : public Shape {
public:
    // Disk Public Methods
    Disk(const Transform *ObjectToWorld, const Transform *WorldToObject,
         bool reverseOrientation, Float height, Float radius, Float innerRadius,
         Float phiMax)
            : Shape(ObjectToWorld, WorldToObject, reverseOrientation),
              height(height),
              radius(radius),
              innerRadius(innerRadius),
              phiMax(Radians(Clamp(phiMax, 0, 2 * Pi))) {}
    Bounds3f ObjectBound() const;
    bool Intersect(const Ray &ray, Float *tHit, SurfaceInteraction *isect,
                   bool testAlphaTexture) const;
    bool IntersectP(const Ray &ray, bool testAlphaTexture) const;
    Float Area() const;

private:
    // Disk Private Data
    const Float height, radius, innerRadius, phiMax;
};

}   // namespace pbrt

#endif //PBRT_SHAPES_DISK_H
