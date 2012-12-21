#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>
#include <cfloat> // FLT_MAX
#include <iostream>
#include <malloc.h>
#include <stdint.h>
#include <png.h> //libpng
using namespace std;

#include "image_png.h"
#include "raytracer.h"
#include "light.h"
#include "scene_parser.h"
#include "image.h"
#include "vectors.h"
#include "camera.h"
#include "ray.h"
#include "hit.h"
#include "group.h"
#include "material.h"

float clamp(float a)
{
    if(a > 1.0f)
        return 1.0f;
    else if(a < 0.0f)
        return 0.0f;
    else
        return a;
}

/*void run(const size_t threads, vector<std::function<void()> > &tasks)
{
    boost::thread_group group;
    const size_t taskCount = (tasks.size() / threads) + 1;
    for(size_t i = 0; i < tasks.size(); i+=taskCount)
    {
        group.create_thread([&tasks, i, taskCount]()
        {
            const size_t end = min(tasks.size(), i+taskCount);
            for(size_t j = i; j<end; ++j)
                tasks[j]();
        });
    }
    group.join_all();
}*/

int main(int argc, char * argv[])
{
    char *input_file = NULL;
    int width = 100;
    int height = 100;
    char *output_file = NULL;
    float depth_min = 0;
    float depth_max = 1;
    char *depth_file = NULL;
    char *normal_file = NULL;
    bool shade_back = false;
    bool shadows = true;
    float weight = 1;
    int bounces = 0;

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc); 
            input_file = argv[i];
        }
        else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc); 
            width = atoi(argv[i]);
            i++; assert (i < argc); 
            height = atoi(argv[i]);
        }
        else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc); 
            output_file = argv[i];
        }
        else if (!strcmp(argv[i],"-depth")) {
            i++; assert (i < argc); 
            depth_min = atof(argv[i]);
            i++; assert (i < argc); 
            depth_max = atof(argv[i]);
            i++; assert (i < argc); 
            depth_file = argv[i];
        }
        else if(!strcmp(argv[i],"-normals")){
            i++; assert(i<argc);
            normal_file = argv[i];
        }
        else if(!strcmp(argv[i],"-shade_back")){
            shade_back = true;
        }
        else if(!strcmp(argv[i],"-bounces")){
            i++; assert(i<argc);
            bounces = atoi(argv[i]);
        }
        else if(!strcmp(argv[i],"-weight")){
            i++; assert(i<argc);
            weight = atof(argv[i]);
        }
        else if(!strcmp(argv[i],"-noshadow")){
            shadows=false;
        }
        else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    } 

    // Define Image Files
    png img(width, height);
    png img_depth(width, height);
    png img_normal(width, height);

    // Read input file
    SceneParser *s = new SceneParser(input_file);
    Camera *c = s->getCamera();
    Group *g = s->getGroup();

    // Set output files
    bool out=false, depthout=false, normout=false;
    if(output_file != NULL)
        out = true;
    if(depth_file != NULL)
        depthout = true;
    if(normal_file != NULL)
        normout = true;

    /*vector<std::function<void()> > tasks(height);*/

    // Main loop
    for(int i=0;i<width;i++) {
        for(int j=0;j<height;j++)
        {
            float xi = (float)i / width; 
            float yj = (float)j / height;
            Vec2f p(xi,yj); //0,0 to 1,1

            Ray r = c->generateRay(p);
            Hit h(FLT_MAX, NULL, Vec3f(0,0,0)); // The object hit
            float tmin = c->getTMin();

            RayTracer t(s, bounces, shadows, shade_back);
            
            /*tasks.push_back([&img,t,i,j,p,width,height]()
            {*/
                //Vec3f cc = t.castRay(r,h,tmin,0,1);  // comment out for anti-aliasing
                Vec3f cc = t.antiAlias(p, float(width), float(height)); // comment out for no AA
                img.setPixel(i,j,cc);
            /*});*/

            //if(debugg)
            {
                Hit hh(FLT_MAX, NULL, Vec3f(0,0,0)); // The object hit
                g->intersect(r,hh,tmin);
                if(hh.getMaterial() != NULL)
                {
                    Vec3f n = hh.getNormal();
                    if(depthout) {
                        float dc = clamp((depth_max-hh.getT()) / (depth_max-depth_min)); // depth colour
                        img_depth.setPixel(i,j,Vec3f(dc,dc,dc));
                    }
                    if(normout) {
                        n.Set(abs(n.x()), abs(n.y()), abs(n.z())); //positive or negative
                        img_normal.setPixel(i,j,n);
                    }
                }
            }
        }
    }

    // Save the image
    if(out)
        img.render(output_file);
    if(depthout)
        img_depth.render(depth_file);
    if(normout)
        img_normal.render(normal_file);

    return 0;
}
