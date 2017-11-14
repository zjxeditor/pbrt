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

// todo: declare Medium in the right place
class Medium {
public:
    Medium() : a(0) {}
    bool operator==(const Medium& m){ return a == m.a; }
    bool operator!=(const Medium& m){ return a != m.a; }

    int a;
};

// todo: declare MediumInterface in the right place
struct MediumInterface {
    MediumInterface() : inside(nullptr), outside(nullptr) {}
    // MediumInterface Public Methods
    MediumInterface(const Medium *medium) : inside(medium), outside(medium) {}
    MediumInterface(const Medium *inside, const Medium *outside)
            : inside(inside), outside(outside) {}
    bool IsMediumTransition() const { return inside != outside; }
    const Medium *inside, *outside;
};

// todo: declare Texture in the right place
template <typename T>
class Texture {
public:
    // Texture Interface
    virtual T Evaluate(const SurfaceInteraction &) const = 0;
    virtual ~Texture() {}
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

    Ray SpawnRay(const Vector3f &d) const {
        Point3f o = OffsetRayOrigin(p, pError, n, d);
        return Ray(o, d, Infinity, time, GetMedium(d));
    }
    Ray SpawnRayTo(const Point3f &p2) const {
        Point3f origin = OffsetRayOrigin(p, pError, n, p2 - p);
        Vector3f d = p2 - p;
        return Ray(origin, d, 1 - ShadowEpsilon, time, GetMedium(d));
    }
    Ray SpawnRayTo(const Interaction &it) const {
        Point3f origin = OffsetRayOrigin(p, pError, n, it.p - p);
        Point3f target = OffsetRayOrigin(it.p, it.pError, it.n, origin - it.p);
        Vector3f d = target - origin;
        return Ray(origin, d, 1 - ShadowEpsilon, time, GetMedium(d));
    }

    bool IsSurfaceInteraction() const { return n != Normal3f(); }
    bool IsMediumInteraction() const { return !IsSurfaceInteraction(); }

    const Medium *GetMedium(const Vector3f &w) const {
        return Dot(w, n) > 0 ? mediumInterface.outside : mediumInterface.inside;
    }
    const Medium *GetMedium() const {
        CHECK_EQ(mediumInterface.inside, mediumInterface.outside);
        return mediumInterface.inside;
    }

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
