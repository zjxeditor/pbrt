//
// Provide basic material interface.
//

#include "material.h"


namespace pbrt {

// Material Method Definitions
Material::~Material() {}

void Material::Bump(const std::shared_ptr<Texture<Float>> &d,
                    SurfaceInteraction *si) {
    // todo: implement the material Bump method
    return;
}

}   // namespace pbrt