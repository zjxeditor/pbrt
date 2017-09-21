//
// Define the cone geometry shape.
//

#include "shapes/cone.h"
#include "efloat.h"

namespace pbrt {

// Cone Method Definitions
Cone::Cone(const Transform *o2w, const Transform *w2o, bool ro, Float height,
           Float radius, Float phiMax)
        : Shape(o2w, w2o, ro),
          radius(radius),
          height(height),
          phiMax(Radians(Clamp(phiMax, 0, 360))) {}

Bounds3f Cone::ObjectBound() const {
    Point3f p1 = Point3f(-radius, -radius, 0);
    Point3f p2 = Point3f(radius, radius, height);
    return Bounds3f(p1, p2);
}

bool Cone::Intersect(const Ray &r, Float *tHit, SurfaceInteraction *isect,
                     bool testAlphaTexture) const {
    Float phi;
    Point3f pHit;
    // Transform _Ray_ to object space
    Vector3f oErr, dErr;
    Ray ray = (*WorldToObject)(r, &oErr, &dErr);

    // Compute quadratic cone coefficients

    // Initialize _EFloat_ ray coordinate values
    EFloat ox(ray.o.x, oErr.x), oy(ray.o.y, oErr.y), oz(ray.o.z, oErr.z);
    EFloat dx(ray.d.x, dErr.x), dy(ray.d.y, dErr.y), dz(ray.d.z, dErr.z);
    EFloat k = EFloat(radius) / EFloat(height);
    k = k * k;
    EFloat a = dx * dx + dy * dy - k * dz * dz;
    EFloat b = 2 * (dx * ox + dy * oy - k * dz * (oz - height));
    EFloat c = ox * ox + oy * oy - k * (oz - height) * (oz - height);

    // Solve quadratic equation for _t_ values
    EFloat t0, t1;
    if (!Quadratic(a, b, c, &t0, &t1)) return false;

    // Check quadric shape _t0_ and _t1_ for nearest intersection
    if (t0.UpperBound() > ray.tMax || t1.LowerBound() <= 0) return false;
    EFloat tShapeHit = t0;
    if (tShapeHit.LowerBound() <= 0) {
        tShapeHit = t1;
        if (tShapeHit.UpperBound() > ray.tMax) return false;
    }

    // Compute cone inverse mapping
    pHit = ray((Float)tShapeHit);
    phi = std::atan2(pHit.y, pHit.x);
    if (phi < 0.) phi += 2 * Pi;

    // Test cone intersection against clipping parameters
    if (pHit.z < 0 || pHit.z > height || phi > phiMax) {
        if (tShapeHit == t1) return false;
        tShapeHit = t1;
        if (t1.UpperBound() > ray.tMax) return false;
        // Compute cone inverse mapping
        pHit = ray((Float)tShapeHit);
        phi = std::atan2(pHit.y, pHit.x);
        if (phi < 0.) phi += 2 * Pi;
        if (pHit.z < 0 || pHit.z > height || phi > phiMax) return false;
    }

    // Find parametric representation of cone hit
    Float u = phi / phiMax;
    Float v = pHit.z / height;

    // Compute cone $\dpdu$ and $\dpdv$
    Vector3f dpdu(-phiMax * pHit.y, phiMax * pHit.x, 0);
    Vector3f dpdv(-pHit.x / (1.f - v), -pHit.y / (1.f - v), height);

    // Compute cone $\dndu$ and $\dndv$
    Vector3f d2Pduu = -phiMax * phiMax * Vector3f(pHit.x, pHit.y, 0.);
    Vector3f d2Pduv = phiMax / (1.f - v) * Vector3f(pHit.y, -pHit.x, 0.);
    Vector3f d2Pdvv(0, 0, 0);

    // Compute coefficients for fundamental forms
    Float E = Dot(dpdu, dpdu);
    Float F = Dot(dpdu, dpdv);
    Float G = Dot(dpdv, dpdv);
    Vector3f N = Normalize(Cross(dpdu, dpdv));
    Float e = Dot(N, d2Pduu);
    Float f = Dot(N, d2Pduv);
    Float g = Dot(N, d2Pdvv);

    // Compute $\dndu$ and $\dndv$ from fundamental form coefficients
    Float invEGF2 = 1 / (E * G - F * F);
    Normal3f dndu = Normal3f((f * F - e * G) * invEGF2 * dpdu +
                             (e * F - f * E) * invEGF2 * dpdv);
    Normal3f dndv = Normal3f((g * F - f * G) * invEGF2 * dpdu +
                             (f * F - g * E) * invEGF2 * dpdv);

    // Compute error bounds for cone intersection

    // Compute error bounds for intersection computed with ray equation
    EFloat px = ox + tShapeHit * dx;
    EFloat py = oy + tShapeHit * dy;
    EFloat pz = oz + tShapeHit * dz;
    Vector3f pError = Vector3f(px.GetAbsoluteError(), py.GetAbsoluteError(),
                               pz.GetAbsoluteError());

    // Initialize _SurfaceInteraction_ from parametric information
    *isect = (*ObjectToWorld)(SurfaceInteraction(pHit, pError, Point2f(u, v),
                                                 -ray.d, dpdu, dpdv, dndu, dndv,
                                                 ray.time, this));
    *tHit = (Float)tShapeHit;
    return true;
}

bool Cone::IntersectP(const Ray &r, bool testAlphaTexture) const {
    Float phi;
    Point3f pHit;
    // Transform _Ray_ to object space
    Vector3f oErr, dErr;
    Ray ray = (*WorldToObject)(r, &oErr, &dErr);

    // Compute quadratic cone coefficients

    // Initialize _EFloat_ ray coordinate values
    EFloat ox(ray.o.x, oErr.x), oy(ray.o.y, oErr.y), oz(ray.o.z, oErr.z);
    EFloat dx(ray.d.x, dErr.x), dy(ray.d.y, dErr.y), dz(ray.d.z, dErr.z);
    EFloat k = EFloat(radius) / EFloat(height);
    k = k * k;
    EFloat a = dx * dx + dy * dy - k * dz * dz;
    EFloat b = 2 * (dx * ox + dy * oy - k * dz * (oz - height));
    EFloat c = ox * ox + oy * oy - k * (oz - height) * (oz - height);

    // Solve quadratic equation for _t_ values
    EFloat t0, t1;
    if (!Quadratic(a, b, c, &t0, &t1)) return false;

    // Check quadric shape _t0_ and _t1_ for nearest intersection
    if (t0.UpperBound() > ray.tMax || t1.LowerBound() <= 0) return false;
    EFloat tShapeHit = t0;
    if (tShapeHit.LowerBound() <= 0) {
        tShapeHit = t1;
        if (tShapeHit.UpperBound() > ray.tMax) return false;
    }

    // Compute cone inverse mapping
    pHit = ray((Float)tShapeHit);
    phi = std::atan2(pHit.y, pHit.x);
    if (phi < 0.) phi += 2 * Pi;

    // Test cone intersection against clipping parameters
    if (pHit.z < 0 || pHit.z > height || phi > phiMax) {
        if (tShapeHit == t1) return false;
        tShapeHit = t1;
        if (t1.UpperBound() > ray.tMax) return false;
        // Compute cone inverse mapping
        pHit = ray((Float)tShapeHit);
        phi = std::atan2(pHit.y, pHit.x);
        if (phi < 0.) phi += 2 * Pi;
        if (pHit.z < 0 || pHit.z > height || phi > phiMax) return false;
    }
    return true;
}

Float Cone::Area() const {
    return radius * std::sqrt((height * height) + (radius * radius)) * phiMax / 2;
}

}   // namespace pbrt