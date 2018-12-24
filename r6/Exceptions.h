#pragma once
#include <exception>

namespace r6 {
    struct bad_syntax : public std::exception {
        const char* what() const noexcept override {
            return "Incorrect syntax. See 'start r6.exe --help'.";
        }
    };

    struct bad_file_extension : public std::exception {
        const char* what() const noexcept override {
            return "Incorrect extension. See 'start r6.exe --help'.";
        }
    };
}
