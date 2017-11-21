//
// Provide undefined class and struct placeholder.
//

#ifndef PBRT_CORE_ZZTEMPHOLDER_H
#define PBRT_CORE_ZZTEMPHOLDER_H

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#include "pbrt.h"

namespace pbrt {

// todo: declare these classes and structs in the right place

class Medium {
public:
    Medium() : a(0) {}
    bool operator==(const Medium& m){ return a == m.a; }
    bool operator!=(const Medium& m){ return a != m.a; }

    int a;
};

struct MediumInterface {
    MediumInterface() : inside(nullptr), outside(nullptr) {}
    // MediumInterface Public Methods
    MediumInterface(const Medium *medium) : inside(medium), outside(medium) {}
    MediumInterface(const Medium *inside, const Medium *outside)
            : inside(inside), outside(outside) {}
    bool IsMediumTransition() const { return inside != outside; }
    const Medium *inside, *outside;
};

template <typename T>
class Texture {
public:
    // Texture Interface
    virtual T Evaluate(const SurfaceInteraction &) const = 0;
    virtual ~Texture() {}
};

class AreaLight
{
public:
    int a;
};


class BSDF {
public:
    int a;
};

class BSSRDF {
public:
    int a;
};



}   // namespace pbr

#endif //PBRT_CORE_ZZTEMPHOLDER_H
