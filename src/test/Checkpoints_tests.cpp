//
// Unit tests for block-chain checkpoints
//
#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>

#include "../checkpoints.h"
#include "../util.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(Checkpoints_tests)

BOOST_AUTO_TEST_CASE(sanity)
{
    uint256 p104 = uint256("0x000000006afe30806352f2015829527dd91f19fbc2d28f799c3ad61c37746fdb");
    uint256 p14401 = uint256("0x0000000000388541b88c57883a480fd6cfa7b93f68e0c71538b08b2c4d875fa2");
    BOOST_CHECK(Checkpoints::CheckBlock(104, p104));
    BOOST_CHECK(Checkpoints::CheckBlock(14401, p14401));

    
    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!Checkpoints::CheckBlock(104, p14401));
    BOOST_CHECK(!Checkpoints::CheckBlock(14401, p104));

    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(Checkpoints::CheckBlock(104+1, p14401));
    BOOST_CHECK(Checkpoints::CheckBlock(14401+1, p104));

    BOOST_CHECK(Checkpoints::GetTotalBlocksEstimate() >= 14401);
}    

BOOST_AUTO_TEST_SUITE_END()
