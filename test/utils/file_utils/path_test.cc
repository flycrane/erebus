#include <gtest/gtest.h>

#include <string>

#include <utils/file_utils/path.h>

namespace {
        TEST(PathTest,SetsConstructorParametersCorrect) {
                const std::string path="testpath.hi";
                Path p(path);

                ASSERT_EQ(path,p.toString());
        }
}

int main(int argc, char **argv) {
                ::testing::InitGoogleTest(&argc,argv);
                return RUN_ALL_TESTS();
}

