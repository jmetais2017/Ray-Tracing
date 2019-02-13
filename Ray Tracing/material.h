
/* 
 * File:   material.h
 * Author: Julien
 *
 * Created on 11 février 2019, 22:36
 */

#ifndef MATERIAL_H
#define MATERIAL_H

struct hit_record;

#include "sphere.h"

//Polynomial approximation for angle-varying reflectivity
inline float schlick(float cosine, float ref_idx){
    float r0 = (1 - ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1 - r0)*pow((1 - cosine), 5);
}

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
private:

};

class lambertian : public material {
public:
    lambertian(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }
    
    vec3 albedo;
    
private:

};


class metal : public material {
public:
    metal(const vec3& a, float f) : albedo(a) { if (f<1) fuzz = f; else fuzz = 1; }
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    
    vec3 albedo;
    float fuzz;
private:

};


class dielectric : public material {
public :
    dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const{
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        float reflect_prob;
        float cosine;
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = - rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else{
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if (refract (r_in.direction(), outward_normal, ni_over_nt, refracted)){
            reflect_prob = schlick(cosine, ref_idx);
        }
        else{
            scattered = ray(rec.p, reflected);
            reflect_prob = 1.0;
        }
        float random = float(rand() % RAND_MAX) / float(RAND_MAX);
        if (random < reflect_prob){ //Reflection
            scattered = ray(rec.p, reflected);
        }
        else{                                   //Refraction
            scattered = ray(rec.p, refracted);
        }
        return true;
    }
    
    float ref_idx;
};

#endif /* MATERIAL_H */

