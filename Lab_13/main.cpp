#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{ 0x4D42 };          // File type always BM which is 0x4D42
    uint32_t file_size{ 0 };               // Size of the file (in bytes)
    uint16_t reserved1{ 0 };               // Reserved, always 0
    uint16_t reserved2{ 0 };               // Reserved, always 0
    uint32_t offset_data{ 0 };             // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
    uint32_t size{ 0 };                    // Size of this header (in bytes)
    int32_t width{ 0 };                    // width of bitmap in pixels
    int32_t height{ 0 };                   // width of bitmap in pixels
    // (if positive, bottom-up, with origin in lower left corner)
    // (if negative, top-down, with origin in upper left corner)
    uint16_t planes{ 1 };                  // No. of planes for the target device, this is always 1
    uint16_t bit_count{ 0 };               // No. of bits per pixel
    uint32_t compression{ 0 };             // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    uint32_t size_image{ 0 };              // 0 - for uncompressed images
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };             // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    uint32_t colors_important{ 0 };        // No. of colors used for displaying the bitmap. If 0 all colors are required
};

struct BMPColorHeader {
    uint32_t red_mask{ 0x00ff0000 };         // Bit mask for the red channel
    uint32_t green_mask{ 0x0000ff00 };       // Bit mask for the green channel
    uint32_t blue_mask{ 0x000000ff };        // Bit mask for the blue channel
    uint32_t alpha_mask{ 0xff000000 };       // Bit mask for the alpha channel
    uint32_t color_space_type{ 0x73524742 }; // Default "sRGB" (0x73524742)
    uint32_t unused[16]{ 0 };               // Unused data for sRGB color space
};
#pragma pack(pop)

class BMP {
public:
    BMP(const std::string& filepath);
    void write(const std::string& filepath);
    void embedMessage(const std::string& message);
    std::string extractMessage(size_t length);

private:
    BMPFileHeader file_header;
    BMPInfoHeader bmp_info_header;
    BMPColorHeader bmp_color_header;
    std::vector<uint8_t> data;

    void readHeaders(std::ifstream& input);
    void writeHeaders(std::ofstream& output);
};

BMP::BMP(const std::string& filepath) {
    std::ifstream input(filepath, std::ios::binary);
    if (input) {
        readHeaders(input);
        input.seekg(file_header.offset_data, input.beg);
        data.resize(bmp_info_header.size_image);
        input.read(reinterpret_cast<char*>(data.data()), data.size());
    }
    else {
        throw std::runtime_error("Unable to open file: " + filepath);
    }
}

void BMP::write(const std::string& filepath) {
    std::ofstream output(filepath, std::ios::binary);
    if (output) {
        writeHeaders(output);
        output.write(reinterpret_cast<const char*>(data.data()), data.size());
    }
    else {
        throw std::runtime_error("Unable to open file: " + filepath);
    }
}

void BMP::embedMessage(const std::string& message) {
    if (message.size() * 8 > data.size()) {
        throw std::runtime_error("Message is too large to fit in the image.");
    }

    size_t data_index = 0;
    for (char c : message) {
        for (int bit = 0; bit < 8; ++bit) {
            data[data_index] = (data[data_index] & 0xFE) | ((c >> bit) & 1);
            ++data_index;
        }
    }
}

std::string BMP::extractMessage(size_t length) {
    std::string message;
    message.reserve(length);

    size_t data_index = 0;
    for (size_t i = 0; i < length; ++i) {
        char c = 0;
        for (int bit = 0; bit < 8; ++bit) {
            c |= (data[data_index] & 1) << bit;
            ++data_index;
        }
        message.push_back(c);
    }

    return message;
}

void BMP::readHeaders(std::ifstream& input) {
    input.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    input.read(reinterpret_cast<char*>(&bmp_info_header), sizeof(bmp_info_header));

    if (bmp_info_header.bit_count == 32) {
        input.read(reinterpret_cast<char*>(&bmp_color_header), sizeof(bmp_color_header));
    }

    // Adjust the header fields for output
    if (bmp_info_header.bit_count == 32) {
        bmp_info_header.size = sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
        file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + sizeof(BMPColorHeader);
    }
    else {
        bmp_info_header.size = sizeof(BMPInfoHeader);
        file_header.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    }
    file_header.file_size = file_header.offset_data + bmp_info_header.size_image;
}

void BMP::writeHeaders(std::ofstream& output) {
    output.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
    output.write(reinterpret_cast<const char*>(&bmp_info_header), sizeof(bmp_info_header));

    if (bmp_info_header.bit_count == 32) {
        output.write(reinterpret_cast<const char*>(&bmp_color_header), sizeof(bmp_color_header));
    }
}

int main() {
    try {
        BMP bmp("sample_640x426.bmp");



        std::string message = "KryshtalMaksim";
        bmp.embedMessage(message);
        bmp.write("sample_640x426_output.bmp");

        BMP bmp2("sample_640x426_output.bmp");
        std::string extracted_message = bmp2.extractMessage(message.size());
        std::cout << "Extracted message: " << "Разработать собственное приложение, в котором должен быть реализован метод НЗБ. При этом: выбор файла-контейнера – по согласованию с преподавателем; реализовать два варианта осаждаемого/извлекаемого сообщения: собственные фамилия, имя и отчество; текстовая часть отчета по одной из выполненных лабораторных работ;" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    system("pause");
    return 0;
}
