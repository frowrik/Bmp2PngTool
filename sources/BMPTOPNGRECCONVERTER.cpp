#include <iostream>
#include <filesystem>

#define STBI_WINDOWS_UTF8 1
#define STBIW_WINDOWS_UTF8 1
#define STB_IMAGE_WRITE_IMPLEMENTATION 1
#define STB_IMAGE_IMPLEMENTATION 1
#include "stb_image.h"
#include "stb_image_write.h"

void processImageToPNG(const char* nameIn, const char* nameOut) {
    int w;
    int h;
    int channels_in_file;
    stbi_uc* imagein = stbi_load(nameIn, &w, &h, &channels_in_file, 0);
    if (imagein) {
        stbi_write_png(nameOut, w, h, channels_in_file, imagein, w * channels_in_file);

        stbi_image_free(imagein);
    }
}

void recursive_processBMP(const std::filesystem::path& src, const std::filesystem::path& dst) {
    if (std::filesystem::is_directory(src)) {
        std::filesystem::create_directories(dst);

        auto diriter = std::filesystem::directory_iterator(src);
        
        for (auto const & item : diriter) {
            if (item.is_regular_file()) {
                auto filepath   = item.path();
                auto name       = filepath.filename();
                auto ext        = filepath.extension();

                if (ext.compare(L".bmp") == 0) {
                    auto fileinpatch = (src / item.path()).generic_u8string();
                    auto fileoutpatch = (dst / item.path().stem()).generic_u8string() + ".png";
                    
                    if (!std::filesystem::exists(fileoutpatch)){
                        processImageToPNG(fileinpatch.c_str(), fileoutpatch.c_str());
                        std::cout << fileoutpatch << "\n";
                    }
                }
            } else {
                recursive_processBMP(item.path(), dst / item.path().filename());
            }
        }

    }
}

void recursive_processALL(const std::filesystem::path& src, const std::filesystem::path& dst) {
    if (std::filesystem::is_directory(src)) {
        std::filesystem::create_directories(dst);

        auto diriter = std::filesystem::directory_iterator(src);

        for (auto const& item : diriter) {
            if (item.is_regular_file()) {
                auto filepath = item.path();
                auto name = filepath.filename();
                auto ext = filepath.extension();

                {
                    auto fileinpatch = (src / item.path()).generic_u8string();
                    auto fileoutpatch = (dst / item.path().stem()).generic_u8string() + ".png";

                    if (!std::filesystem::exists(fileoutpatch)) {
                        processImageToPNG(fileinpatch.c_str(), fileoutpatch.c_str());
                        std::cout << fileoutpatch << "\n";
                    }
                }
            }
            else {
                recursive_processALL(item.path(), dst / item.path().filename());
            }
        }

    }
}

int main(int argc, char** argv) {
    
    if (argc > 2) {
        std::cout << "dir bmp input";
        std::cout << argv[1];
        std::cout << "\n";

        std::cout << "dir png output";
        std::cout << argv[2];
        std::cout << "\n";

        std::filesystem::path In = std::filesystem::u8path(argv[1]);
        std::filesystem::path Out = std::filesystem::u8path(argv[2]);

        recursive_processBMP(In, Out);
    }

    return 0;
}
