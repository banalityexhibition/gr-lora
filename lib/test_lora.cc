#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define BOOST_TEST_MODULE lora_test
#include <boost/test/unit_test.hpp>

#include "qa_lora.h"

BOOST_AUTO_TEST_CASE(test_lora)
{
    // Add your unit test case suite from qa_lora
    BOOST_CHECK(qa_lora::suite());
}
