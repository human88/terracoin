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
        ;

    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 546, uint256("000000002a936ca763904c3c35fce2f3556c559c0214345d31b1bcebf76acb70"))
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
