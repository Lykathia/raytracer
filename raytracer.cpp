#include "raytracer.h"

RayTracer::RayTracer(SceneParser *sc, int b, bool shadows, bool bs)
{
    s = sc;
    max_bounce=b;
    noshadows = shadows;
    back_shading = bs;
}

float RayTracer::specularExponent(Ray r, Hit h, Vec3f dir)
{
    // (h.n)^e -- h=halfway vector (direction+view)/|(direction+view)|, n = normal
    Vec3f view = r.getDirection(); view.Negate(); 
    Vec3f half = (dir+view); half.Normalize(); // Halfway vector
    float t = half.Dot3(h.getNormal()); t = max(0.0f,t);
    return t*pow(t,(h.getMaterial()->getExponent()*5)); // Not sure why *5, just played with values until it looked right.
}

float RayTracer::clamp(float a)
{
    if(a > 1.0f)
        return 1.0f;
    else if(a < 0.0f)
        return 0.0f;
    else
        return a;
}

const Vec3f RayTracer::antiAlias(Vec2f pixel, float height, float width)
{
    const int sampleSize = 16; const int sqrtSample = 4; // So don't have to compute sqrt each time
    float tmin = s->getCamera()->getTMin();

    Vec3f c[sampleSize];
    Vec2f p[sampleSize];

    for(int i=0;i<sqrtSample; ++i)
        for(int j=0;j<sqrtSample;++j)
            p[i*sqrtSample+j] = Vec2f(
            ((float(rand())/float(RAND_MAX))/width + (i/sqrtSample)) + pixel.x(),
            ((float(rand())/float(RAND_MAX))/height + (j/sqrtSample)) + pixel.y()); //jitter in subpixel

    Ray r[sampleSize]; Hit h[sampleSize];
    for(int i=0;i<sampleSize;++i) {
        r[i] = s->getCamera()->generateRay(p[i]);
        h[i] = Hit(FLT_MAX, NULL, Vec3f(0.0f,0.0f,0.0f));
    }

    for(int i=0;i<sampleSize;i++)
        c[i] = castRay(r[i], h[i], tmin, 0, 1);

    Vec3f sum(0,0,0), ret;
    for(int i=0;i<sampleSize;++i)
        sum += c[i];

    ret = (sum * (1.0f / sampleSize)); ret.Clamp();

    return ret;
}

const Vec3f RayTracer::castRay(Ray &r, Hit &h, float tmin, int n, float refracIndex)
{
    Vec3f colour = s->getBackgroundColor();
    Vec3f zero(0.0f,0.0f,0.0f);
    
    /* Find Nearest Object */
    if(s->getGroup()->intersect(r, h, tmin))
    {
        Vec3f norm = h.getNormal();
        if(back_shading)
            if(norm.Dot3(r.getDirection())>0.0f)
                norm.Negate();

        /* Shade and Texture */
        Vec3f diffuse = h.getMaterial()->getDiffuseColor() * s->getAmbientLight();
        Vec3f reflective = h.getMaterial()->getReflectiveColor();
        Vec3f refractive = h.getMaterial()->getTransparentColor();
    
        /* Lighting */
        for(int k=0;k<s->getNumLights();k++)
        {
            Light *l = s->getLight(k);
            Vec3f dir, col; float d;

            l->getIllumination(h.getIntersectionPoint(), dir, col, d);
            dir.Normalize();
            
            /* Check in shadow */
            Hit shadowHit(d, NULL, zero);
            if(!s->getGroup()->intersect(Ray(h.getIntersectionPoint(), dir), shadowHit, EPS) || !noshadows)
            {   
                // lambert * diffuse
                diffuse += clamp(dir.Dot3(norm)) * col * h.getMaterial()->getDiffuseColor();
                if(norm.Dot3(dir)>0.0f)
                    diffuse += specularExponent(r,h,dir) * col * h.getMaterial()->getSpecularColor();
            }
        }

        /* Reflection */
        if((reflective!=zero) && (n<max_bounce))
        {
            // d - 2*(n.d)*n
            reflective.Normalize();
            Vec3f reflec_d = r.getDirection() - (2.0f*r.getDirection().Dot3(norm))*norm; reflec_d.Normalize();
            Ray reflec_r(h.getIntersectionPoint(), reflec_d); Hit reflec_h(FLT_MAX, NULL, zero);
            reflective = h.getMaterial()->getReflectiveColor() * castRay(reflec_r, reflec_h, EPS, n+1, refracIndex);
        }
        else
            reflective = zero;

        /* Refraction */ 
        if((h.getMaterial()->getTransparentColor() != zero) && (n<max_bounce))
        {
            float computedIndex = (norm.Dot3(r.getDirection())<=0.0f) ? h.getMaterial()->getRefractionIndex() : 1.0f;
            
            Vec3f trans_d = r.getDirection(); //trans_d.Negate();
            float angle = norm.Dot3(trans_d) * -1.0f;
            float ii = refracIndex / computedIndex;

            float t = 1.0f - (ii*ii) * (1.0f-(angle*angle));
            if(t>=0.0f)
            {
                Vec3f rd = (ii * angle - sqrt(t))*norm + ii*trans_d; rd.Normalize(); rd.Normalize();
                Ray trans_r(h.getIntersectionPoint(), rd); Hit trans_h(FLT_MAX, NULL, zero);
                refractive = h.getMaterial()->getTransparentColor() * castRay(trans_r,trans_h,EPS,n+1,computedIndex);
            }
        }
        else refractive = zero;

        // Ambiant + Diffuse + Specular + Reflective + Transparent */
        colour = diffuse + reflective + refractive; colour.Clamp();

    }
    return colour;
}

