#pragma once
#include <exception>
#include "_ImageFormat.h"

class R6 : public ImageFormat {
public:
    void load(Filepath const &filepath, ImageBuffer* buffer) override;
    void save(Filepath const &filepath, ImageBuffer* buffer) override;
};

//TODO: refactor
namespace r6 {
    enum class EMode {
        DEDICATED,
        FIXED,
        GRAYSCALE,
    };

    struct bad_syntax : public std::exception {
        const char* what() const noexcept override {
            return "Incorrect syntax.";
        }
    };

    struct bad_file_extension : public std::exception {
        const char* what() const noexcept override {
            return "Incorrect extension.";
        }
    };
}
