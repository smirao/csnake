#include <lib/Unity/src/unity.h>
#include <include/functions.h>

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_add()
{
    TEST_ASSERT_EQUAL_INT(4, add(2,2));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_add);
    return UNITY_END();
}