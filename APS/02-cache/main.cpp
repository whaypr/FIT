#include <bits/types/FILE.h>
#include <cstdio>
#include <fstream>
#include <cmath>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;


/*uint8_t calculatePixel(uint8_t mm, uint8_t ml, uint8_t mr, uint8_t tm, uint8_t bm) {
    return max( 0, min(255, 5*mm - ml - mr - tm - bm) );
}*/


void addToHist(uint8_t r, uint8_t g, uint8_t b, int hist[]) {
    int Y = round(0.2126*r + 0.7152*g + 0.0722*b);

    if (Y > 203)
        hist[4] += 1;
    else if (Y > 152)
        hist[3] += 1;
    else if (Y > 101)
        hist[2] += 1;
    else if (Y > 50)
        hist[1] += 1;
    else
        hist[0] += 1;
}


int main(int argc, char** argv) {
    int fd = open (argv[1], O_RDONLY);

    // get size of the file
    struct stat s;
    fstat (fd, & s);
    int size = s.st_size;

    unsigned char *f = (unsigned char *) mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);

    int width, height;

    // load width
    string str = "";
    int i = 3;
    while (true) {
        //putchar(f[3]);
        if (f[i] == '\n')
            break;
        str += (int)f[i];
        i++;
    }
    width = stoi(str);

    // load height
    str = "";
    i++;
    while (true) {
        if (f[i] == '\n')
            break;
        str += (int)f[i];
        i++;
    }
    height = stoi(str);

    // skip the rest of the header
    i += 5; // \n255\n


    // process and store output data
    FILE * imageOut = fopen("output.ppm", "w");
    fputs("P6\n", imageOut);
    fprintf(imageOut, "%d", width);
    fputs("\n", imageOut);
    fprintf(imageOut, "%d", height);
    fputs("\n255\n", imageOut);

    int hist[5] = {0, 0, 0, 0, 0};
    unsigned char r, g, b;

    // copy the first row
    for (int w = 0; w < width; ++w) {
        r = f[i++];
        g = f[i++];
        b = f[i++];
        fputc(r, imageOut);
        fputc(g, imageOut);
        fputc(b, imageOut);
        addToHist(r, g, b, hist);
    }
    
    // process the middle
    for (int h = 1; h < height - 1; ++h) {
        // copy first item
        r = f[i++];
        g = f[i++];
        b = f[i++];
        fputc(r, imageOut);
        fputc(g, imageOut);
        fputc(b, imageOut);
        addToHist(r, g, b, hist);

        for (int w = 1; w < width - 1; ++w) {
            r = max( 0, min(255, 5*f[i] - f[i-3] - f[i+3] - f[i-3*width] - f[i+3*width]) );
            i++;
            g = max( 0, min(255, 5*f[i] - f[i-3] - f[i+3] - f[i-3*width] - f[i+3*width]) );
            i++;
            b = max( 0, min(255, 5*f[i] - f[i-3] - f[i+3] - f[i-3*width] - f[i+3*width]) );
            i++;
            fputc(r, imageOut);
            fputc(g, imageOut);
            fputc(b, imageOut);
            addToHist(r, g, b, hist);
        }

        // copy last item
        r = f[i++];
        g = f[i++];
        b = f[i++];
        fputc(r, imageOut);
        fputc(g, imageOut);
        fputc(b, imageOut);
        addToHist(r, g, b, hist);
    }

    // copy the last row
    for (int w = 0; w < width; ++w) {
        r = f[i++];
        g = f[i++];
        b = f[i++];
        fputc(r, imageOut);
        fputc(g, imageOut);
        fputc(b, imageOut);
        addToHist(r, g, b, hist);
    }

    // save histogram
    FILE * histOut = fopen("output.txt", "w");
    for (int i = 0; i < 4; ++i) {
        fprintf(histOut, "%d", hist[i]);
        fputc(' ', histOut);
    }
        
    fprintf(histOut, "%d", hist[4]);

    close(fd);
    fclose(imageOut);
    fclose(histOut);
    munmap(f, size);

    return 0;
}
