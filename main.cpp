#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include "lib/Filepath.h"
#include "lib/ImageFormats/ImageBuffer.h"
#include "lib/ImageFormats/BMP.h"
#include "lib/ImageFormats/R6.h"


void printHelp();


int main(int nArg, char* args[]) {
    std::vector<std::string> allowedExts {"R6", "bmp"};
    Filepath source, output;

    r6::EMode mode;
    bool dithering = false;
    try {
        for (int i = 1; i < nArg; ++i) {
            // --help
            if (strcmp(args[i], "--help") == 0 && nArg == 2) {
                printHelp();
                return 0;
            }

            // --mode
            if (strcmp(args[i], "-m") == 0 || strcmp(args[i], "--mode") == 0) {
                if (nArg >= i+3) {
                    if (strcmp(args[i+1], "dedicated") == 0) {
                        mode = r6::EMode::DEDICATED; i++; continue;
                    } else if (strcmp(args[i+1], "fixed") == 0) {
                        mode = r6::EMode::FIXED; i++; continue;
                    } else if (strcmp(args[i+1], "grayscale") == 0) {
                        mode = r6::EMode::GRAYSCALE; i++; continue;
                    }
                    throw r6::bad_syntax();
                } else {
                    throw r6::bad_syntax();
                }
            }

            // --dithering
            if (strcmp(args[i], "-d") == 0 || strcmp(args[i], "--dithering") == 0) {
                dithering = true;
                continue;
            }

            // source file
            if (!source.initialized()) {
                source.init(args[i]);
                continue;
            }

            // output file
            if (!output.initialized()) {
                output.init(args[i]);
                continue;
            }
        }


        // validate
        if (!source.initialized() ||
        none_of(allowedExts.begin(), allowedExts.end(), [&source](std::string ext){return ext == source.ext();}) ||
        (output.initialized() && none_of( allowedExts.begin(), allowedExts.end(), [&output](std::string ext){return ext == output.ext();}))) {
            throw r6::bad_syntax();
        }

        ImageBuffer buffer;
        BMP bmp;
        bmp.load(source, &buffer);

       buffer.useDedicatedPalette(6);
       /* buffer.useFixedPalette(palette);
        * buffer.convertToGrayscale(nBits);
        */
        bmp.save(output, &buffer);


    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}


void printHelp() {
    using namespace std;
    cout << "USAGE: r6.exe [(-m | --mode) {dedicated | fixed | grayscale}]" << endl;
    cout << "              [(-d | --dithering)]" << endl;
    cout << "              \"path\\to\\sourceImage.{r6 | bmp}\"" << endl;
    cout << "              \"path\\to\\output.{r6 | bmp}\"" << endl;
    cout << endl;
    cout << "DEFAULTS:" << endl;
    cout << "mode: dedicated" << endl;
    cout << "dithering: enabled" << endl;
    cout << "output: the same file path, but with r6 extension if source extension is bmp and vice versa" << endl;
    cout << endl;
    cout << "EXAMPLES:" << endl;
    cout << "R6.exe -m fixed -d foo.bmp bar.r6" << endl;
    cout << "Convert image to R6 format using fixed color palette and use dithering." << endl;
    cout << endl;
    cout << "R6.exe bar.r6 foo.bmp" << endl;
    cout << "Convert image to bmp format." << endl;
    cout << endl;
    cout << "ABOUT R6:" << endl;
    cout << "- raster graphics file format" << endl;
    cout << "- uses 6-bits to store RGB colors" << endl;
    cout << "- uses Rice compression algorithm" << endl;
    cout << "- supports: fixed color palette, dedicated color palette, grayscale" << endl;
}
