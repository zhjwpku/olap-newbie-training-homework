#pragma once

#include <folly/concurrency/ConcurrentHashMap.h>
#include "FileUtils.h"

class Worker {
  public:
    static void read_and_parse(const FileSlice &slice,
                               std::shared_ptr<folly::ConcurrentHashMap<int32_t, std::string>> chmap,
                               bool should_print = false);
};