/* 
 * File:   main.cpp
 * Author: Julien Métais
 *
 * Created on 11 février 2019, 14:37
 */


#include "hitable_list.h"
#include "camera.h"
#include "material.h"

//Objets monochromes
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

//Premier shading (directionnel)
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

//Color
vec3 color(const ray& r, hitable *world, int depth){
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
        if(world->hit(r, 0.001, MAXFLOAT, rec)){
            ray scattered;
            vec3 attenuation;
            //vec3 target = rec.p + rec.normal + random_in_unit_sphere();
            if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
                return attenuation*color(scattered, world, depth+1);
            }
            else{
                return vec3(0, 0, 0);
            }
            //return 0.5*color( ray(rec.p, target - rec.p), world);
        }
        else{ //Sky (blue&white lerp)
            vec3 unit_direction = unit_vector(r.direction());
            float t = 0.5*(unit_direction.y() + 1.0);
            return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
        }
}

//Random scene
hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48()); 
            if ((center-vec3(4,0.2,0)).length() > 0.9) { 
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
                }
                else if (choose_mat < 0.95) { // metal
                    list[i++] = new sphere(center, 0.2,
                            new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())),  0.5*drand48()));
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list,i);
}

int main(int argc, char** argv) {
    srand(time(NULL));
    
    //Picture size
    int nx = 1920;
    int ny = 1080;
    int ns = 100;
    
    //Initializing the ppm file
    cout << "P3\n" << nx << " " << ny << "\n255\n" ;
    
    //World creation
    hitable *list[5];
    list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1, 0, -1), 0.5f, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    list[3] = new sphere(vec3(-1, 0, -1), 0.5f, new dielectric(1.5));
    list[4] = new sphere(vec3(-1, 0, -1), -0.45f, new dielectric(1.5));
    hitable *world = new hitable_list(list, 5);
    world = random_scene();
    
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    //float dist_to_focus = (lookfrom - lookat).length();
    float dist_to_focus = 10.0;
    float aperture = 0.01;
        //Choose up, fov, aspect ratio
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 25, float(nx)/float(ny), aperture, dist_to_focus);
    
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
                col += color(r, world, 0);
            }
            
            //Display
            col /= float(ns);
                //Gamma correction
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            
            cout << ir << " " << ig << " " << ib <<"\n";
        }
    }
    return 0;
}