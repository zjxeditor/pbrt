//
// Combine geometry information and shading information together for an object.
//

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_CORE_PRIMITIVES_H
#define PBRT_CORE_PRIMITIVES_H

#include "pbrt.h"
#include "shape.h"
#include "material.h"
#include "zztempholder.h"

namespace pbrt{

// Primitive Declarations
class Primitive {
public:
    // Primitive Interface
    virtual ~Primitive();
    virtual Bounds3f WorldBound() const = 0;
    virtual bool Intersect(const Ray &r, SurfaceInteraction *) const = 0;
    virtual bool IntersectP(const Ray &r) const = 0;
    virtual const AreaLight *GetAreaLight() const = 0;
    virtual const Material *GetMaterial() const = 0;
    virtual void ComputeScatteringFunctions(SurfaceInteraction *isect,
                                            MemoryArena &arena,
                                            TransportMode mode,
                                            bool allowMultipleLobes) const = 0;
};

}   // namespace pbrt




#endif //PBRT_CORE_PRIMITIVES_H
