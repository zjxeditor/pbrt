//
// Define the curve geometry shape.
//

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SHAPES_CURVE_H
#define PBRT_SHAPES_CURVE_H

#include "shape.h"

namespace pbrt {

// CurveType Declarations
enum class CurveType {
    Flat, Cylinder, Ribbon
};

// CurveCommon Declarations
struct CurveCommon {
    CurveCommon(const Point3f c[4], Float w0, Float w1, CurveType type, const Normal3f *norm);

    const CurveType type;
    Point3f cpObj[4];
    Float width[2];
    Normal3f n[2];
    Float normalAngle, invSinNormalAngle;
};




// Curve Declarations
class Curve : public Shape {
public:
    // Curve Public Methods
    Curve(const Transform *ObjectToWorld, const Transform *WorldToObject,
          bool reverseOrientation, const std::shared_ptr<CurveCommon> &common,
          Float uMin, Float uMax)
            : Shape(ObjectToWorld, WorldToObject, reverseOrientation),
              common(common),
              uMin(uMin),
              uMax(uMax) {}

    Bounds3f ObjectBound() const;
    bool Intersect(const Ray &ray, Float *tHit, SurfaceInteraction *isect,
                   bool testAlphaTexture) const;
    Float Area() const;

private:
    // Curve Private Methods
    bool recursiveIntersect(const Ray &r, Float *tHit,
                            SurfaceInteraction *isect, const Point3f cp[4],
                            const Transform &rayToObject, Float u0, Float u1,
                            int depth) const;

    // Curve Private Data
    const std::shared_ptr<CurveCommon> common;
    const Float uMin, uMax;
};

}   // namespace pbrt

#endif //PBRT_SHAPES_CURVE_H
