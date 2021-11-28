#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <string_view>

struct FileSlice {
    long beg = 0;   // slice first pos, % BlockSize == 0
    long end = 0;   // slice last pos
    long first_to_read = 0;
    std::string_view filename;

    FileSlice(long beg, long end, long first, std::string_view sv) : beg(beg), end(end), first_to_read(first), filename(sv) {}

    friend std::ostream& operator<<(std::ostream& os, const FileSlice& slice);
};


class FileUtils {
  public:
    static size_t getFileSize(const std::string &filename);
    static std::vector<FileSlice> splitFile(const std::string &filename, int threadNum);
};