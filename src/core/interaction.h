//
// Provide interaction information used for light computation.
//

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_INTERACTION_H
#define PBRT_CORE_INTERACTION_H

#include "pbrt.h"
#include "geometry.h"
#include "transform.h"

namespace pbrt {

// todo: declare MediumInterface in the right place
struct MediumInterface {
    MediumInterface() {}
    MediumInterface(const int *p) : P(p) {};
    const int *P;
};

// todo: declare Shape in the right place
class Shape {
public:
    bool reverseOrientation;
    bool transformSwapsHandedness;
};

// Interaction Declarations
struct Interaction {
    // Interaction Public Methods
    Interaction() : time(0) {}
    Interaction(const Point3f &p, const Normal3f &n, const Vector3f &pError,
                const Vector3f &wo, Float time,
                const MediumInterface &mediumInterface)
            : p(p),
              time(time),
              pError(pError),
              wo(Normalize(wo)),
              n(n),
              mediumInterface(mediumInterface) {}
    Interaction(const Point3f &p, const Vector3f &wo, Float time,
                const MediumInterface &mediumInterface)
            : p(p), time(time), wo(wo), mediumInterface(mediumInterface) {}
    Interaction(const Point3f &p, Float time,
                const MediumInterface &mediumInterface)
            : p(p), time(time), mediumInterface(mediumInterface) {}

    bool IsSurfaceInteraction() const { return n != Normal3f(); }
    bool IsMediumInteraction() const { return !IsSurfaceInteraction(); }

    // Interaction Public Data
    Point3f p;
    Float time;
    Vector3f pError;
    Vector3f wo;
    Normal3f n;
    MediumInterface mediumInterface;
};

// SurfaceInteraction Declarations
class SurfaceInteraction : public Interaction {
public:
    // SurfaceInteraction Public Methods
    SurfaceInteraction() {}
    SurfaceInteraction(const Point3f &p, const Vector3f &pError,
                       const Point2f &uv, const Vector3f &wo,
                       const Vector3f &dpdu, const Vector3f &dpdv,
                       const Normal3f &dndu, const Normal3f &dndv, Float time,
                       const Shape *sh);
    void SetShadingGeometry(const Vector3f &dpdu, const Vector3f &dpdv,
                            const Normal3f &dndu, const Normal3f &dndv,
                            bool orientationIsAuthoritative);

    // SurfaceInteraction Public Data
    Point2f uv;
    Vector3f dpdu, dpdv;
    Normal3f dndu, dndv;
    const Shape *shape = nullptr;
    struct {
        Normal3f n;
        Vector3f dpdu, dpdv;
        Normal3f dndu, dndv;
    } shading;

};


}   // namespace pbrt

#endif //PBRT_CORE_INTERACTION_H
