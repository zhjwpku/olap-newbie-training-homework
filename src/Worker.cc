#include "Worker.h"

void Worker::read_and_parse(const FileSlice &slice,
                            std::shared_ptr<folly::ConcurrentHashMap<int32_t, std::string>> chmap) {
    // read -> parse -> then insert into concurrent hash map
    std::cout << "thread " << std::this_thread::get_id() << " handle slice " << slice << std::endl;
}