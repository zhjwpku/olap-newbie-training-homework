#include <gtest/gtest.h>
#include <Solution.h>

using namespace std;

TEST(solution, test1) {
    Solution s;
    s.load_lineitem("/home/lineitem.tbl");
    std::string res = s.query(100, 200);
    res = s.query(200, 2000);
    res = s.query(5000, 6000);
    res = s.query(2345, 6789);
    res = s.query(123, 456);
}