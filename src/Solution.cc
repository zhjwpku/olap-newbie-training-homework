#include <iostream>
#include <thread>

#include "Solution.h"
#include "FileUtils.h"
#include "Constants.h"
#include "Worker.h"

Solution::Solution() {
    for (int i = 0; i < Constants::ThreadNum; i++) {
        vumap.push_back(std::unordered_map<int, std::string>(1000000));
    }
}

void Solution::load_lineitem(const std::string &filename) {
    struct timespec ts1, ts2, ts3, ts4;
    timespec_get(&ts1, TIME_UTC);
    std::vector<FileSlice> slices = FileUtils::splitFile(filename, Constants::ThreadNum);
    timespec_get(&ts2, TIME_UTC);

    // for each slice, use one thread to handle it
    std::vector<std::thread> threads;
    for (int i = 0; i < slices.size(); i++) {
            threads.push_back(std::thread(Worker::read_and_parse, slices[i], std::ref(vumap[i])));
    }

    // wait for all worker to finish
    for (auto &th : threads) {
        th.join();
    }

    timespec_get(&ts3, TIME_UTC);

    // merge all the unordered_map into a map
    for (int i = 0; i < slices.size(); i++) {
        for (auto x: vumap[i]) {
            auto iter = mmap.find(x.first);
            if (iter == mmap.end()) {
                mmap[x.first] = x.second;
            } else if (mmap[x.first] < x.second) {
                mmap[x.first] = x.second;
            }
        }
    }

    timespec_get(&ts4, TIME_UTC);

    std::cout << "load linetime split file use time " <<
        ((int64_t)ts2.tv_sec - (int64_t)ts1.tv_sec) * (int64_t)1000000000
        + ((int64_t)ts2.tv_nsec - (int64_t)ts1.tv_nsec) << " ns.\n";

    std::cout << "read and parse the file use time " <<
        ((int64_t)ts3.tv_sec - (int64_t)ts2.tv_sec) * (int64_t)1000000000
        + ((int64_t)ts3.tv_nsec - (int64_t)ts2.tv_nsec) << " ns.\n";

    std::cout << "merge unordered_map to map use time " <<
        ((int64_t)ts4.tv_sec - (int64_t)ts3.tv_sec) * (int64_t)1000000000
        + ((int64_t)ts4.tv_nsec - (int64_t)ts3.tv_nsec) << " ns.\n";
}

std::string Solution::query(int param1, int param2) {
    struct timespec ts1, ts2;
    timespec_get(&ts1, TIME_UTC);
    if (param2 < param2) {
        std::swap(param1, param2);
    }
    auto iter1 = mmap.lower_bound(param1);
    auto iter2 = mmap.upper_bound(param2);
    std::string result = "";
    int key = 0;
    while (iter1 != iter2)
    {
        if (result < iter1->second) {
            result = iter1->second;
            key = iter1->first;
        }
        iter1++;
    }
    timespec_get(&ts2, TIME_UTC);
    std::cout << "The max value between " << param1 << " and " << param2 << " is key:" << key << ", value: " << result 
              << "; used time " << ((int64_t)ts2.tv_sec - (int64_t)ts1.tv_sec) * (int64_t)1000000000
              + ((int64_t)ts2.tv_nsec - (int64_t)ts1.tv_nsec) << " ns.\n";
    return result;
}