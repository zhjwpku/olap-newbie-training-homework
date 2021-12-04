#include <fstream>
#include <iostream>
#include <thread>
#include <ctime>
#include "FileUtils.h"
#include "Worker.h"
#include "Constants.h"

static inline void handle_buffer(std::unordered_map<int32_t, std::string> &umap,
                          char buffer[], int len,
                          int &bar_num, int &suppkey, int &idx, char comment[]) {
    for (int i = 0; i < len; i++) {
        char c = buffer[i];
        if (c == '|') {
            bar_num ++;
            continue;
        }
        if (bar_num == 2) {
            suppkey = suppkey * 10 + (c - '0');
            continue;
        }
        if (bar_num == 15) {
            comment[idx++] = c;
        }
        if (bar_num == 16) {
            // get the value
            auto iter = umap.find(suppkey);
            if (iter == umap.end()) {
                umap[suppkey] = std::string(comment, idx);
            } else if (umap[suppkey] < std::string_view(comment, idx)) {
                umap[suppkey] = std::string(comment, idx);
            }
        }
        if (c == '\n') {
            // clean status
            bar_num = 0;
            suppkey = 0;
            idx = 0;
        }
    }
}

void Worker::read_and_parse(const FileSlice &slice,
                            std::unordered_map<int32_t, std::string> &umap) {
    
    // read -> parse -> then insert into concurrent hash map
    // std::cout << "thread " << std::this_thread::get_id() << " handle slice " << slice << std::endl;
    struct timespec ts1, ts2;
    timespec_get(&ts1, TIME_UTC);

    std::ifstream file(std::string(slice.filename).c_str(), std::ios::binary | std::ios::ate);
    char buffer[Constants::BlockSize];

    long beg = slice.beg;
    long end = slice.end;
    long first_to_read = slice.first_to_read;
    int step = Constants::BlockSize;
    int remaining = end & Constants::BlockSizeMask;
    end -= remaining;

    // memorize a line of data
    int bar_num = 0;
    int suppkey = 0;
    char comment[45];
    int idx = 0;

    // handle the first one
    if (first_to_read > beg) {
        long offset = first_to_read;
        long length = beg + step - offset;
        // std::cout << "offset: " << offset << "; length: " << length << std::endl;

        file.seekg(offset, std::ios::beg);
        if (file.read(buffer, length)) {
            handle_buffer(umap, buffer, length, bar_num, suppkey, idx, comment);
        } else {
            std::cout << "Error\n";
        }

        // update the beg
        beg += step;
    }

    // handle the middle ones
    while (beg < end) {
        long offset = beg;
        long length = step;
        // std::cout << "offset: " << offset << "; length: " << length << std::endl;

        file.seekg(offset, std::ios::beg);
        if (file.read(buffer, length)) {
            handle_buffer(umap, buffer, length, bar_num, suppkey, idx, comment);
        } else {
            std::cout << "Error\n";
        }
        beg += step;
    }

    // handle the last one
    if (remaining > 0) {
        long offset = beg;
        long length = remaining;
        // std::cout << "offset: " << offset << "; length: " << length << std::endl;
        file.seekg(offset, std::ios::beg);
        if (file.read(buffer, length)) {
            handle_buffer(umap, buffer, length, bar_num, suppkey, idx, comment);
        } else {
            std::cout << "Error\n";
        }
    }

    // std::cout <<  << umap.size() << " data\n";
    timespec_get(&ts2, TIME_UTC);
    std::cout << "thread " << std::this_thread::get_id() << " spent " <<
        ((int64_t)ts2.tv_sec - (int64_t)ts1.tv_sec) * (int64_t)1000000000
         + ((int64_t)ts2.tv_nsec - (int64_t)ts1.tv_nsec) << " ns.\n";
}
