#include <iostream>

#include "Solution.h"
#include "FileUtils.h"
#include "Constants.h"
#include "Worker.h"

void Solution::load_lineitem(const std::string &filename) {
    std::vector<FileSlice> slices = FileUtils::splitFile(filename, Constants::ThreadNum);

    // for each slice, use one thread to handle it
    std::vector<std::thread> threads;
    for (int i = 0; i < slices.size(); i++) {
        if (i == 0) {
            threads.push_back(std::thread(Worker::read_and_parse, slices[i], chmap, false));
        } else {
            threads.push_back(std::thread(Worker::read_and_parse, slices[i], chmap, false));
        }
    }

    // wait for all worker to finish
    for (auto &th : threads) {
        th.join();
    }
}

std::string Solution::query(int param1, int param2) {
    return "";
}