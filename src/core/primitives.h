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

// GeometricPrimitive Declarations
class GeometricPrimitive : public Primitive {
public:
    // GeometricPrimitive Public Methods
    GeometricPrimitive(const std::shared_ptr<Shape> &shape,
                       const std::shared_ptr<Material> &material,
                       const std::shared_ptr<AreaLight> &areaLight,
                       const MediumInterface &mediumInterface)
            : shape(shape),
              material(material),
              areaLight(areaLight),
              mediumInterface(mediumInterface) {}

    virtual Bounds3f WorldBound() const;
    virtual bool Intersect(const Ray &r, SurfaceInteraction *isect) const;
    virtual bool IntersectP(const Ray &r) const;
    const AreaLight *GetAreaLight() const;
    const Material *GetMaterial() const;
    void ComputeScatteringFunctions(SurfaceInteraction *isect,
                                    MemoryArena &arena, TransportMode mode,
                                    bool allowMultipleLobes) const;

private:
    // GeometricPrimitive Private Data
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Material> material;
    std::shared_ptr<AreaLight> areaLight;
    MediumInterface mediumInterface;
};

// TransformedPrimitive Declarations
class TransformedPrimitive : public Primitive {
public:
    // TransformedPrimitive Public Methods
    TransformedPrimitive(std::shared_ptr<Primitive> &primitive,
                         const AnimatedTransform &PrimitiveToWorld)
            : primitive(primitive), PrimitiveToWorld(PrimitiveToWorld) {}
    bool Intersect(const Ray &r, SurfaceInteraction *in) const;
    bool IntersectP(const Ray &r) const;
    const AreaLight *GetAreaLight() const { return nullptr; }
    const Material *GetMaterial() const { return nullptr; }
    void ComputeScatteringFunctions(SurfaceInteraction *isect,
                                    MemoryArena &arena, TransportMode mode,
                                    bool allowMultipleLobes) const {
        LOG(FATAL) <<
                   "TransformedPrimitive::ComputeScatteringFunctions() shouldn't be "
                           "called";
    }
    Bounds3f WorldBound() const {
        return PrimitiveToWorld.MotionBounds(primitive->WorldBound());
    }

private:
    // TransformedPrimitive Private Data
    std::shared_ptr<Primitive> primitive;
    const AnimatedTransform PrimitiveToWorld;
};

// Aggregate Declarations. Accelerator should subclass this class.
class Aggregate : public Primitive {
public:
    // Aggregate Public Methods
    const AreaLight *GetAreaLight() const;
    const Material *GetMaterial() const;
    void ComputeScatteringFunctions(SurfaceInteraction *isect,
                                    MemoryArena &arena, TransportMode mode,
                                    bool allowMultipleLobes) const;
};

}   // namespace pbrt

#endif //PBRT_CORE_PRIMITIVES_H
