#pragma once

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
// #include <folly/concurrency/ConcurrentHashMap.h>

class Solution {
private:
  // maintain a vector of unordered map
  std::vector<std::unordered_map<int, std::string>> vumap;

  // merge all unordered map to a map
  std::map<int, std::string> mmap;

public:
  Solution();
  void load_lineitem(const std::string &filename);
  std::string query(int param1, int param2);
};

