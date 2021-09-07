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
        if (stbi_write_png(nameOut, w, h, channels_in_file, imagein, w * channels_in_file) == 0) {
            std::cout << "not save image " << nameOut << "\n";
        }

        stbi_image_free(imagein);
    } else {
        std::cout << "not open image " << nameIn << "\n";
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
                        std::cout << fileinpatch << " to " << fileoutpatch << "\n";
                    } else {
                        std::cout << "not convert, image already exists " << fileoutpatch << "\n";
                    }
                }
            } else {
                recursive_processBMP(item.path(), dst / item.path().filename());
            }
        }

    }
}

int main(int argc, char** argv) {
    std::cout << "example cmd 1: bmptopngconv input_dir output_dir\n";
    std::cout << "example cmd 2: bmptopngconv input_bmp output_png\n";
    if (argc > 2) {
        std::cout << "bmp or dir input ";
        std::cout << argv[1];
        std::cout << "\n";

        std::cout << "png or dir output ";
        std::cout << argv[2];
        std::cout << "\n";

        std::filesystem::path In    = std::filesystem::absolute(std::filesystem::u8path(argv[1]));
        std::filesystem::path Out   = std::filesystem::absolute(std::filesystem::u8path(argv[2]));

        std::filesystem::create_directories(Out.parent_path());

        if (std::filesystem::is_regular_file(In)) {
            if (In.extension().compare(L".bmp") == 0) {
                auto fileinpatch    = In.generic_u8string();
                auto fileoutpatch   = Out.generic_u8string();

                if (!std::filesystem::exists(fileoutpatch)) {
                    processImageToPNG(fileinpatch.c_str(), fileoutpatch.c_str());
                    std::cout << fileinpatch << " to " << fileoutpatch << "\n";
                } else {
                    std::cout << "not convert, image already exists " << fileoutpatch << "\n";
                }
            }
        } else {
            recursive_processBMP(In, Out);
        }
    }

    return 0;
}
