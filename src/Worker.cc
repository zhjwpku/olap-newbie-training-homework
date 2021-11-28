#include <fstream>
#include "Worker.h"
#include "Constants.h"

void Worker::read_and_parse(const FileSlice &slice,
                            std::shared_ptr<folly::ConcurrentHashMap<int32_t, std::string>> chmap,
                            bool should_print) {
    // read -> parse -> then insert into concurrent hash map
    if (should_print) {
        std::cout << "thread " << std::this_thread::get_id() << " handle slice " << slice << std::endl;
    }

    std::ifstream file(std::string(slice.filename).c_str(), std::ios::binary | std::ios::ate);
    std::vector<char> buffer(Constants::BlockSize);

    should_print = false;

    long beg = slice.beg;
    long end = slice.end;
    long first_to_read = slice.first_to_read;
    int step = Constants::BlockSize;
    int remaining = end & Constants::BlockSizeMask;
    end -= remaining;

    // handle the first one
    if (first_to_read > beg) {
        long offset = first_to_read;
        long length = beg + step - offset;
        if (should_print) {
            std::cout << "offset: " << offset << "; length: " << length << std::endl;
        }

        file.seekg(offset, std::ios::beg);
        if (file.read(buffer.data(), length)) {
            
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
        if (should_print) {
            std::cout << "offset: " << offset << "; length: " << length << std::endl;
        }
        file.seekg(offset, std::ios::beg);
        if (file.read(buffer.data(), length)) {
            
        } else {
            std::cout << "Error\n";
        }
        beg += step;
    }

    // handle the last one
    if (remaining > 0) {
        long offset = beg;
        long length = remaining;
        if (should_print) {
            std::cout << "offset: " << offset << "; length: " << length << std::endl;
        }
        file.seekg(offset, std::ios::beg);
        if (file.read(buffer.data(), length)) {
            
        } else {
            std::cout << "Error\n";
        }
    }
}