#pragma once 
#include <SEGGER_RTT.h>
#include <printf.h>

#define TEST_ASSERT(cond)   \
({  \
    int __test_failed = 0;  \
    if(!(cond)) {             \
        printf ("%s [TEST_ERROR] %s() Test: " #cond " failed in line %d \n", \
            RTT_CTRL_TEXT_BRIGHT_RED ,  \
            __func__,           \
            __LINE__);          \
            __test_failed = 1;  \
    } else {                    \
        printf ("%s [TEST_SUCCESS] %s() Test: " #cond " passed! \n", \
            RTT_CTRL_TEXT_BRIGHT_GREEN, \
            __func__);   \
    }   \
    __test_failed;  \
})

#define TEST_ASSERT_NOT_NULL(a) TEST_ASSERT(a != NULL)
#define TEST_ASSERT_NULL(a) TEST_ASSERT(a == NULL)
#define TEST_ASSERT_NON_ZERO(a) TEST_ASSERT(a != 0)
#define TEST_ASSERT_ZERO(a) TEST_ASSERT(a == 0)
#define TEST_ASSERT_EQUALS(a,b) TEST_ASSERT(a == b)
#define TEST_ASSERT_NOT_EQUALS(a,b) TEST_ASSERT(a != b)

#define TEST_LOG_INFO(fmt, ...) \
    do {    \
        printf("%s [LOG_INFO]: %s(): " fmt "\n",    \
            RTT_CTRL_TEXT_BRIGHT_GREEN, \
            __func__,   \
            ##__VA_ARGS__); \
    }while(0) 

#define TEST_LOG_W(fmt, ...) \
    do {    \
        printf("%s [LOG_INFO]: %s(): " fmt "\n",    \
            RTT_CTRL_TEXT_BRIGHT_YELLOW, \
            __func__,   \
            ##__VA_ARGS__); \
    }while(0) 

#define TEST_LOG_ERROR(fmt, ...) \
    do {    \
        printf("%s [LOG_INFO]: %s(): " fmt "\n",    \
            RTT_CTRL_TEXT_BRIGHT_RED, \
            __func__,   \
            ##__VA_ARGS__); \
    }while(0) 
