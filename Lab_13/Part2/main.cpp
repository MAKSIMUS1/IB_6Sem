#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <stdexcept>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{ 0x4D42 };
    uint32_t file_size{ 0 };
    uint16_t reserved1{ 0 };
    uint16_t reserved2{ 0 };
    uint32_t offset_data{ 0 };
};

struct BMPInfoHeader {
    uint32_t size{ 0 };
    int32_t width{ 0 };
    int32_t height{ 0 };
    uint16_t planes{ 1 };
    uint16_t bit_count{ 0 };
    uint32_t compression{ 0 };
    uint32_t size_image{ 0 };
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };
    uint32_t colors_important{ 0 };
};
#pragma pack(pop)

std::vector<uint8_t> load_bmp(const std::string& filename, BMPFileHeader& file_header, BMPInfoHeader& bmp_info_header) {
    std::ifstream input(filename, std::ios::binary);
    if (!input) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    input.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    input.read(reinterpret_cast<char*>(&bmp_info_header), sizeof(bmp_info_header));

    if (file_header.file_type != 0x4D42) {
        throw std::runtime_error("Error: Not a BMP file");
    }

    std::vector<uint8_t> img_data(bmp_info_header.size_image);
    input.seekg(file_header.offset_data, std::ios::beg);
    input.read(reinterpret_cast<char*>(img_data.data()), bmp_info_header.size_image);
    input.close();
    return img_data;
}

void save_bmp(const std::string& filename, const BMPFileHeader& file_header, const BMPInfoHeader& bmp_info_header, const std::vector<uint8_t>& img_data) {
    std::ofstream output(filename, std::ios::binary);
    if (!output) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    output.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
    output.write(reinterpret_cast<const char*>(&bmp_info_header), sizeof(bmp_info_header));
    output.write(reinterpret_cast<const char*>(img_data.data()), img_data.size());
    output.close();
}

void create_lsb_color_image(const std::vector<uint8_t>& img_data, int width, int height, const std::string& output_filename) {
    std::vector<uint8_t> lsb_img_data = img_data;

    for (size_t i = 0; i < img_data.size(); i += 3) {
        uint8_t blue = img_data[i] & 1;
        uint8_t green = img_data[i + 1] & 1;
        uint8_t red = img_data[i + 2] & 1;

        lsb_img_data[i] = blue ? 255 : 0;
        lsb_img_data[i + 1] = green ? 255 : 0;
        lsb_img_data[i + 2] = red ? 255 : 0;
    }

    BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;
    bmp_info_header.width = width;
    bmp_info_header.height = height;
    bmp_info_header.bit_count = 24;
    bmp_info_header.size_image = lsb_img_data.size();
    file_header.file_size = file_header.offset_data + lsb_img_data.size();

    save_bmp(output_filename, file_header, bmp_info_header, lsb_img_data);
}

int main() {
    try {
        BMPFileHeader file_header;
        BMPInfoHeader bmp_info_header;

        std::string input_filename = "sample_640x426.bmp";
        std::string output_filename = "output_lsb.bmp";

        // Load original image
        std::vector<uint8_t> img_data = load_bmp(input_filename, file_header, bmp_info_header);

        int width = bmp_info_header.width;
        int height = bmp_info_header.height;

        // Create and save LSB color image
        create_lsb_color_image(img_data, width, height, output_filename);

        std::cout << "LSB color image saved as " << output_filename << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
