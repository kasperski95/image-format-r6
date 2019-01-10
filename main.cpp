#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>                        // none_of
#include "lib/Filepath.h"
#include "lib/ImageFormats/ImageBuffer.h"
#include "lib/ImageFormats/BMP.h"
#include "lib/ImageFormats/R6.h"


void printHelp();


struct bad_syntax : public std::exception {
    const char* what() const noexcept override {
        return "Incorrect syntax.";
    }
};

struct bad_file_extension : public std::exception {
    const char* what() const noexcept override {
        return "Unsupported extension.";
    }
};

//=================================================================

int main(int nArg, char* args[]) {
    std::vector<std::string> allowedExts {"r6", "bmp"};
    Filepath source, output;
    ImageBuffer buffer;
    R6 r6;
    BMP bmp;

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
                if (nArg >= i+2) {
                    if (strcmp(args[i+1], "dedicated") == 0) {
                        r6.mode = R6::Mode::DEDICATED; i++; continue;
                    } else if (strcmp(args[i+1], "fixed") == 0) {
                        r6.mode = R6::Mode::FIXED; i++; continue;
                    } else if (strcmp(args[i+1], "grayscale") == 0) {
                        r6.mode = R6::Mode::GRAYSCALE; i++; continue;
                    }
                    throw bad_syntax();
                } else {
                    throw bad_syntax();
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
            throw bad_file_extension();
        }


        // core
        {
            unsigned int sourceFileSize;
            // load
            if (source.ext() == "bmp") {
                sourceFileSize = bmp.load(source, &buffer);

                // choose palette
                switch(r6.mode) {
                    case R6::Mode::DEDICATED:
                        buffer.generatePalette(64);
                    break;
                    case R6::Mode::FIXED:
                        buffer.palette(r6.palette());

                        /* save palette
                        {
                            buffer.generateBufferFromPalette();
                            output.ext("bmp");
                            bmp.save(output, &buffer);
                            return 666;
                        }*/
                    break;
                    case R6::Mode::GRAYSCALE:
                        buffer.palette(r6.palette());
                        buffer.grayscale(true);
                    break;
                }

                // dither
                if (dithering) {
                    buffer.dither();
                }
                r6.dithering = dithering;
            } else if (source.ext() == "r6") {
                sourceFileSize = r6.load(source, &buffer);
            }

            // save
            unsigned int outputFileSize;
            if (output.ext() == "bmp") {
                outputFileSize = bmp.save(output, &buffer);
            } else if (output.ext() == "r6") {
                outputFileSize = r6.save(output, &buffer);
            }

            // print result
            std::cout << source.ext() << ": " << sourceFileSize << " B | ";
            std::cout << output.ext() << ": " << outputFileSize << " B | ";
            std::cout << round((float)outputFileSize / sourceFileSize * 100) << " %" << std::endl;

            if (source.ext() == "r6") {
                std::cout << std::endl << "R6 HEADER" << std::endl;
                r6.print(source);
            }
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

//=================================================================

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
    cout << "EXAMPLES OF USE:" << endl;
    cout << "R6.exe -m fixed -d foo.bmp bar.r6" << endl;
    cout << "Convert image to R6 format using fixed color palette and use dithering." << endl;
    cout << endl;
    cout << "R6.exe bar.r6 foo.bmp" << endl;
    cout << "Convert image to bmp format." << endl;
}
