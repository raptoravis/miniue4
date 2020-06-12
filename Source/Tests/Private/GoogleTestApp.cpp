#include "GoogleTestApp.h"

#include "gtest/gtest.h"

// Do not do this in Unreal 4.20. It will cause some errors because internally google test
// adds windows.h which conflicts to other headers that CoreMinimal.h depends.
#include "src/gtest-all.cc"
#include "src/gmock-all.cc"
#include "src/gtest_main.cc"

class GivenASampleTest : public ::testing::Test
{
};

TEST_F(GivenASampleTest, WhenRunningSampleTestShouldPass)
{
	ASSERT_TRUE(true);
}