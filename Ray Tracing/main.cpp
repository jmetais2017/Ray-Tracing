
/* 
 * File:   main.cpp
 * Author: Julien Métais
 *
 * Created on 11 février 2019, 14:37
 */

#include <ctime>

#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"

/*
bool hit_sphere(const vec3& center, float radius, const ray& r){
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}
*/
/*
float hit_sphere(const vec3& center, float radius, const ray& r){
        vec3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float b = 2.0 * dot(oc, r.direction());
        float c = dot(oc, oc) - radius*radius;
        float discriminant = b*b - 4*a*c;
        if (discriminant < 0){
            return -1.0;
        }
        else{
            return (-b - sqrt(discriminant))/(2.0*a);
        }
}
*/

vec3 color(const ray& r, hitable *world){
        /*
        if(hit_sphere(vec3(0, 0, 1), 0.5, r)){
            return vec3(1, 0, 0);
        }
        */
        /*
        float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
        if(t>0.0){
            vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1));
            return 0.5*vec3(N.x()+1, N.y()+1, N.z()+1);
        }
        vec3 unit_direction = unit_vector(r.direction());
        t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
         */
        hit_record rec;
        if(world->hit(r, 0.0, MAXFLOAT, rec)){
            return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
        }
        else{
            vec3 unit_direction = unit_vector(r.direction());
            float t = 0.5*(unit_direction.y() + 1.0);
            return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
        }
}


int main(int argc, char** argv) {
    srand(time(NULL));
    
    //Picture size
    int nx = 200;
    int ny = 100;
    int ns = 100;
    
    //Initializing the ppm file
    cout << "P3\n" << nx << " " << ny << "\n255\n" ;
    
    //World creation
    hitable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(list, 2);
    camera cam;
    
    float random;
    
    //Sweeping through the picture
    for (int j = ny-1; j>=0; j--){
        for (int i = 0; i<nx; i++){
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++){
                //Picture coordinates
                random = float(rand() % RAND_MAX) / float(RAND_MAX);
                float u = float(i + random) / float(nx);
                random = float(rand() % RAND_MAX) / float(RAND_MAX);
                float v = float(j + random) / float(ny);
                
                //Ray "look at"
                ray r = cam.get_ray(u, v);

                //Computing the ray color
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world);
            }
            
            //Display
            col /= float(ns);
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            
            cout << ir << " " << ig << " " << ib <<"\n";
        }
    }
    return 0;
}

