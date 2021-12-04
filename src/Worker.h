#pragma once

// #include <folly/concurrency/ConcurrentHashMap.h>
#include <memory>
#include <unordered_map>

class FileSlice;

class Worker {
  public:
    static void read_and_parse(const FileSlice &slice,
                               std::unordered_map<int32_t, std::string> &umap);
};