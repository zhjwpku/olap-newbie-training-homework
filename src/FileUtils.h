#pragma once

#include <vector>
#include <string>
#include <iostream>

struct FileSlice {
    long beg = 0;   // slice first pos, % BlockSize == 0
    long end = 0;   // slice last pos
    long first_to_read = 0;

    FileSlice(long beg, long end, long first) : beg(beg), end(end), first_to_read(first) {}

    friend std::ostream& operator<<(std::ostream& os, const FileSlice& slice);
};


class FileUtils {
  public:
    static size_t getFileSize(const std::string &filename);
    static std::vector<FileSlice> splitFile(const std::string &filename, int threadNum);
};