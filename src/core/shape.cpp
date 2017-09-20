//
// Provide interface for geometry shapes used for ray tracer
//

#include "shape.h"

namespace pbrt {

// Shape Method Definitions
Shape::~Shape() {}

Shape::Shape(const Transform *ObjectToWorld, const Transform *WorldToObject, bool reverseOrientation)
        : ObjectToWorld(ObjectToWorld), WorldToObject(WorldToObject), reverseOrientation(reverseOrientation),
          transformSwapsHandedness(ObjectToWorld->SwapsHandedness()) {}

Bounds3f Shape::WorldBound() const { return (*ObjectToWorld)(ObjectBound()); }


}   // namespace pbrt