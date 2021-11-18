#include <gtest/gtest.h>
#include <Solution.h>

using namespace std;

TEST(solution, test1) {
    Solution s;
    s.load_lineitem("/home/lineitem.tbl");
}