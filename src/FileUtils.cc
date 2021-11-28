#include <iostream>
#include <fstream>

#include "FileUtils.h"
#include "Constants.h"

std::ostream& operator<<(std::ostream& os, const FileSlice& slice)
{
    os << slice.filename << " [beg: " <<  slice.beg << "; end: " << slice.end << "; first to read: " << slice.first_to_read << ']';
    return os;
}

size_t FileUtils::getFileSize(const std::string &filename) {
  std::streampos begin,end;
  std::ifstream myfile (filename.c_str(), std::ios::binary);
  begin = myfile.tellg();
  myfile.seekg (0, std::ios::end);
  end = myfile.tellg();
  myfile.close();
  return end - begin;
}

std::vector<FileSlice> FileUtils::splitFile(const std::string &filename, int threadNum) {
    std::vector<FileSlice> result;
    size_t fileSize = getFileSize(filename);
    // std::cout << fileSize << std::endl;
    long step = fileSize / threadNum;
    step = (step + Constants::BlockSize - 1) & (~Constants::BlockSizeMask);
    // std::cout << step << std::endl;

    // only file slice
    if (fileSize <= step) {
        result.emplace_back(0, fileSize, 0, filename);
        return result;
    }

    // multiple file slices
    long beg = 0;
    long end = step;
    
    while (beg < fileSize) {
        end = end < fileSize ? end : fileSize;
        result.emplace_back(beg, end, beg, filename);
        beg += step;
        end += step;
    }

    // read the border and normalize file slices
    std::ifstream inf {filename};
    std::string strData;
    for (int i = 0; i < result.size() - 1; i++) {
        inf.seekg(result[i].end, std::ios::beg);
        std::getline(inf, strData);
        result[i].end += strData.size() + 1;
        result[i+1].first_to_read += strData.size() + 1;
    }

    return result;
}