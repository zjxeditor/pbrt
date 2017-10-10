//
// Define the loop subdivision process.
//

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_SHAPES_LOOPSUBDIV_H
#define PBRT_SHAPES_LOOPSUBDIV_H

#include "shape.h"

namespace pbrt {

// LoopSubdiv Declarations
// todo: parse loop subdivision parameters from ParamSet Struct
//std::vector<std::shared_ptr<Shape>> CreateLoopSubdiv(const Transform *o2w,
//                                                     const Transform *w2o,
//                                                     bool reverseOrientation,
//                                                     const ParamSet &params);

}   // namespace pbrt

#endif //PBRT_SHAPES_LOOPSUBDIV_H
