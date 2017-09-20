//
// Provide interface for geometry shapes used for ray tracer
//

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_SHAPE_H
#define PBRT_CORE_SHAPE_H

#include "pbrt.h"
#include "geometry.h"
#include "interaction.h"
#include "transform.h"

namespace pbrt {

// Shape Declarations
class Shape {
public:
    // Shape Interface
    Shape(const Transform *ObjectToWorld, const Transform *WorldToObject, bool reverseOrientation);
    virtual ~Shape();
    virtual Bounds3f ObjectBound() const = 0;
    virtual Bounds3f WorldBound() const;
    virtual bool Intersect(const Ray &ray, Float *tHit, SurfaceInteraction *isect,
                           bool testAlphaTexture = true) const = 0;
    virtual bool IntersectP(const Ray &ray, bool testAlphaTexture = true) const {
        return Intersect(ray, nullptr, nullptr, testAlphaTexture);
    }
    virtual Float Area() const = 0;

    // Shape Public Data
    const Transform *ObjectToWorld, *WorldToObject;
    const bool reverseOrientation;
    const bool transformSwapsHandedness;
};


}   // namespace pbrt


#endif //PBRT_CORE_SHAPE_H
