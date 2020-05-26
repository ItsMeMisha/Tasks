#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

class BMPFile {

public:
    BMPFile (const char*& fileName);
    ~BMPFile();

    char* image = nullptr;
    void writeImage (const char* fileName);

private:
    static const int dataOffset   = 10;
    static const int widthOffset  = 18;
    static const int heightOffset = 22;

    std::size_t height      = 0;
    std::size_t width       = 0;
    std::size_t fileSize    = 0;
    std::size_t imageOffset = 0;
    
    void loadImage (const char*& fileName);
    char* data = nullptr;
   
};

BMPFile::BMPFile (const char*& fileName) {

    loadImage (fileName);
    
    imageOffset = *(std::size_t*)(data + dataOffset);
    widthOffset = *(std::size_t*)(data + widthOffset);
    heightOffset = *(std::size_t*)(data + heightOffset);

    image = data + imageOffset;
}

BMPFile::~BMPFile() {

    delete[] data;
}

void BMPFile::loadImage (const char*& fileName);

    FILE* input = fopen (fileName, "rb");
    struct stat* fileInfo = (struct stat*) calloc (1, sizeof (struct stat));
    stat (fileName, fileInfo);

    fileSize = fileInfo->st_size;

    data = (char*) calloc (fileSize, sizeof (char));
    fread (data, fileSize, sizeof (char), input);
    fclose (input);
}

void BMPFile::writeImage (const char* fileName) {

    FILE* output = fopen (fileName, "wb");
    fwrite (data, sizeof(char), fileSize, output);
    fclose (output);
}

BMPFile* blend(const BMPFile& frontImg, const BMPFile& backImg) {

    


}

BMPFile* blend_asm(const BMPFile& frontImg, const BMPFile& backImg) {



}


