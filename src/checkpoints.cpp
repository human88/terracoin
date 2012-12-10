// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        (    0, uint256("0x00000000804bbc6a621a9dbb564ce469f492e1ccf2d70f8a6b241e26a277afa2"))
        (    1, uint256("0x000000000fbc1c60a7610c894f98d102390e9e00cc18caced4eb4198ec0c3645"))
        (   31, uint256("0x00000000045341e3ebdfa180e4a0f1e4da23829609517a3673b4a796714a7593"))
        (  104, uint256("0x000000006afe30806352f2015829527dd91f19fbc2d28f799c3ad61c37746fdb"))
        (  355, uint256("0x0000000000c0e178ddd6a8f15724f37470428c233883c302267299b21fb5d237"))
        (  721, uint256("0x00000000027671a417f3d3eafac6d150f0b2ccba37f0b63f75bc657ec25950ed"))
        ( 2000, uint256("0x000000000283ecd683fe30d4542929a78873df7818029793f84e9c65dc337d94"))
        ( 3000, uint256("0x0000000000317b69ff2a56284442fede7ffa66f75d000f1cf34171ad671db4b6"))
        ( 4000, uint256("0x00000000001190cad5d66d028b6afcf22db58d2b5c17abf2bf2e1353be13097d"))
        ( 4255, uint256("0x000000000018b3ba5b241f3e88b4a88e580f9e7dd0fc7fc786ff22c586e53dd9"))
        ( 5631, uint256("0x00000000001243509866938e344c0010bd88b156da27ef9d707cb1f1698f2a32"))
        ( 6000, uint256("0x00000000000c1abfd7c29d07e23ef52631c6874e42e6a240a4d3678d9c716d81"))
        ( 7395, uint256("0x0000000000005d8e1281d6b28fe6b504ab81e7e3ec561e97b7a98973e449f7fb"))
        ( 8001, uint256("0x00000000001ca63707536b6dfc8ba4c5aa7fce19b6295ef73e195554cdb92d44"))
        ( 8157, uint256("0x00000000001303998da2714abf02159f1421103e77fee3b876d69c0fa7b108d9"))
        (12311, uint256("0x00000000002b5708fefdceb5db42cd2135eaf23f23a95c285e8f310262f8d639"))
        (13224, uint256("0x00000000000765c69f777ccbc44fab23edab9126f1b4ec5078450aebc3809c36"))
        (14401, uint256("0x0000000000388541b88c57883a480fd6cfa7b93f68e0c71538b08b2c4d875fa2"))
        (15238, uint256("0x000000000000ae09d46bc1a9c5ca4c7b5e51bf23d3108926daa140d64355d390"))
        (18426, uint256("0x00000000001fb1c075dbfa27f7ba83928a2d35152ccae59c742f698fb8cb8108"))
        (19114, uint256("0x000000000022224f57adecffdc8f5cffb3b932838310aefdd12aaabcc6122259"))
        (20124, uint256("0x00000000002374e0e1fcee28b520dff3f3e86cb7ff7afdea112aaf2002101900"))
        (21711, uint256("0x00000000002189b0ae139b8c449bbcb99d520b8a798b7e0f0ff8ab8539e9bcb4"))
        (22100, uint256("0x0000000000142ace7d8db003da69191896986c5564e604e3100d14c17daaf90f"))
        (22566, uint256("0x00000000000c28dc052d277d18e104e4e63c53e4018273b3af49f772af205d43"))
        (24076, uint256("0x00000000000522702c7f0ee6fa2ce3978cc0f3056ecc73d8cde1035891c5c4d5"))
        (25372, uint256("0x0000000000202428a01ad6b8a2e256162b5bba35efd1e7f45dc42dbf5861f784"))
        (25538, uint256("0x00000000002faa5586493e3821d90482348b0462d625d03d086a4a2a2301f6ef"))
        (26814, uint256("0x0000000000179b0ed2a7d4390ff2c6213f1c522790b4e084a4e2036b53e6765b"))
        ;

    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        (    0, uint256("0x00000000d64b490e447fb522682bfa6bcb27886ed1a94d7a4856fb92ab130875"))
        (    1, uint256("0x00000000d64b490e447fb522682bfa6bcb27886ed1a94d7a4856fb92ab130875"))
        ;

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (fTestNet ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
