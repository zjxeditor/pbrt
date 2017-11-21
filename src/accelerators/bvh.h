////
//// BVH accelerator for ray tracer.
////
//
//#if defined(_MSC_VER)
//#define NOMINMAX
//#pragma once
//#endif
//
//#ifndef PBRT_ACCELERATORS_BVH_H
//#define PBRT_ACCELERATORS_BVH_H
//
//#include "pbrt.h"
//#include "primitives.h"
//#include <atomic>
//
//namespace pbrt {
//
//// BVHAccel Forward Declarations
//struct BVHBuildNode;
//struct BVHPrimitiveInfo;
//struct MortonPrimitive;
//struct LinearBVHNode;
//
//// BVHAccel Declarations
//class BVHAccel : Aggregate {
//public:
//    // BVHAccel Public Types
//    enum class SplitMethod {SAH, HLBVH, Middle, EqualCounts};
//
//    // BVHAccel Public Methods
//    BVHAccel(const std::vector<std::shared_ptr<Primitive>> &p,
//             int maxPrimsInNode = 1,
//             SplitMethod splitMethod = SplitMethod::SAH);
//    ~BVHAccel();
//
//    Bounds3f WorldBound() const;
//    bool Intersect(const Ray &ray, SurfaceInteraction *isect) const;
//    bool IntersectP(const Ray &ray) const;
//
//private:
//    // BVHAccel Private Methods
//    BVHBuildNode *recursiveBuild(
//            MemoryArena &arena, std::vector<BVHPrimitiveInfo> &primitiveInfo,
//            int start, int end, int *totalNodes,
//            std::vector<std::shared_ptr<Primitive>> &orderedPrims);
//     BVHBuildNode *HLBVHBuild(
//             MemoryArena &arena, const std::vector<BVHPrimitiveInfo> &primitiveInfo,
//             int *totalNodes, std::vector<std::shared_ptr<Primitive>> &orderedPrims) const;
//    BVHBuildNode *emitLBVH(
//            BVHBuildNode *&buildNodes,
//            const std::vector<BVHPrimitiveInfo> &primitiveInfo,
//            MortonPrimitive *mortonPrims, int nPrimitives, int *totalNodes,
//            std::vector<std::shared_ptr<Primitive>> &orderedPrims,
//            std::atomic<int> *orderedPrimsOffset, int bitIndex) const;
//    BVHBuildNode *buildUpperSAH(MemoryArena &areana, std::vector<BVHBuildNode *> &treeletroots,
//                                int start, int end, int *totalNodes) const;
//    int flattenBVHTree(BVHBuildNode *node, int *offset);
//
//    // BVHAccel Private Data
//    const int maxPrimsInNode;
//    const SplitMethod splitMethod;
//    std::vector<std::shared_ptr<Primitive>> primitives;
//    LinearBVHNode *nodes = nullptr;
//};
//
//
//}   // namespace pbrt
//
//
//#endif // PBRT_ACCELERATORS_BVH_H
