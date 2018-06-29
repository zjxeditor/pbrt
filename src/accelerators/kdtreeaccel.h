//
// kd-tree accelerator for ray tracer.
//

#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_ACCELERATORS_KDTREEACCEL_H
#define PBRT_ACCELERATORS_KDTREEACCEL_H

#include "pbrt.h"
#include "primitives.h"

namespace pbrt {

// KdTreeAccel Declarations
struct KdAccelNode;
struct BoundEdge;
class KdTreeAccel : public Aggregate {
public:
    // KdTreeAccel Public Methods
    KdTreeAccel(const std::vector<std::shared_ptr<Primitive>> &p,
                int isectCost = 80, int traversalCost = 1,
                Float emptyBonus = 0.5, int maxPrims = 1, int maxDepth = -1);
    ~KdTreeAccel();
    Bounds3f WorldBound() const { return bounds; }
    bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;
    bool IntersectP(const Ray &ray) const;

private:
    // KdTreeAccel Private Methods
    void buildTree(int nodeNum, const Bounds3f &bounds,
                   const std::vector<Bounds3f> &primBounds, int *primNums,
                   int nprims, int depth,
                   const std::unique_ptr<BoundEdge[]> edges[3], int *prims0,
                   int *prims1, int badRefines = 0);

    // KdTreeAccel Private Data
    const int isectCost, traversalCost, maxPrims;
    const Float emptyBonus;
    std::vector<std::shared_ptr<Primitive>> primitives;
    std::vector<int> primitiveIndices;
    KdAccelNode *nodes;
    int nAllocedNodes, nextFreeNode;
    Bounds3f bounds;
};


}   // namespace pbrt

#endif  // PBRT_ACCELERATORS_KDTREEACCEL_H
