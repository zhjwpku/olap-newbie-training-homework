#pragma once

#include <string>
#include <memory>
#include <folly/concurrency/ConcurrentHashMap.h>

class Solution {
private:
  // main a folly concurrent hash map
  std::shared_ptr<folly::ConcurrentHashMap<int32_t, std::string>> chmap;
public:

  Solution() {
    chmap = std::make_shared<folly::ConcurrentHashMap<int32_t, std::string>>(100);
  }
  void load_lineitem(const std::string &filename);
  std::string query(int param1, int param2);
};

