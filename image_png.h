#ifndef __IMG_PNG_H_INCLUDED__
#define __IMG_PNG_H_INCLUDED__

#include <cstdlib>
#include <cstdio>
#include <png.h>
#include <assert.h>
#include "vectors.h"

class png {
public:
    /****************************
    // Constructor/Destructor   */
    png(int w, int h) { width = w; height = h; data = new Vec3f[width*height]; }
    ~png() { delete [] data; }

    /****************************
    // Accesors                 */
    int w() const { return width; }
    int h() const { return height; }
    const Vec3f& getPixel(int x, int y) const {
        assert(x>=0&&x<width); assert(y>=0&&y<height);
        return data[y*width+x]; }

    /****************************
    // Modifers                 */
    void flood(const Vec3f &color) {
        for(int i=0;i<width*height;++i)
            data[i] = color; }
    void setPixel(int x, int y, const Vec3f &color) {
        assert(x>=0&&x<width); assert(y>=0&&y<height);
        data[(width-y-1)*width + x] = color; }

    /****************************
    // Output                   */
    void render(const char *filename) const {

        assert(filename!=NULL);
        const char *ext = &filename[strlen(filename)-4];
        assert(!strcmp(ext,".png"));
        
        FILE *f = fopen(filename, "wb"); assert(f != NULL);
        png_structp png_ptr; png_infop info_ptr;

        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
        assert(png_ptr != NULL);

        info_ptr = png_create_info_struct(png_ptr);
        assert(info_ptr != NULL);
        assert(!setjmp(png_jmpbuf(png_ptr)));

        png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, 
            PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_BASE,PNG_FILTER_TYPE_BASE);
        assert(!setjmp(png_jmpbuf(png_ptr)));

        png_byte **row_ptrs = (png_bytep*)png_malloc(png_ptr,height*sizeof(png_byte*));
        for(int y=0;y<height;++y)
        {
            png_byte *row = (png_bytep)png_malloc(png_ptr,sizeof(uint8_t)*width*3);
            row_ptrs[y] = row;
            for(int x=0;x<width;++x)
            {
                *row++ = (uint8_t)floor(data[y*width+x].r()*255.0f);
                *row++ = (uint8_t)floor(data[y*width+x].g()*255.0f);
                *row++ = (uint8_t)floor(data[y*width+x].b()*255.0f);
            }
        }

        png_init_io(png_ptr, f);
        png_set_rows(png_ptr, info_ptr, row_ptrs);
        png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

        for(int y=0; y<height; y++)
            png_free(png_ptr, row_ptrs[y]);
        png_free(png_ptr, row_ptrs);
        png_destroy_write_struct(&png_ptr, &info_ptr);

        fclose(f);
    }

private:
    int width, height;
    Vec3f *data;
};

#endif
