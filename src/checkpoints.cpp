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

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is conservative. On multi-core CPUs with
    // parallel signature checking enabled, this number is way too high.
    // We prefer a progressbar that's faster at the end than the other
    // way around, though.
    static const double fSigcheckVerificationFactor = 15.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64 nTimeLastCheckpoint;
        int64 nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
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
        (28326, uint256("0x0000000000141af96e4ab491d6534a6740491d62799b1669418d33bb007acfd7"))
        (28951, uint256("0x00000000002404c991c7f9e1d641e8938f1ab704a3f9e1d22589d817948a202f"))
        (30765, uint256("0x00000000004be710d96035855f406c1393c922d5948d82dce494368e3993c76a"))
        (32122, uint256("0x0000000000109367e18d9c9762cdb8bfb3c524628ad2ce9bcb02a6a9bfa13e39"))
        (33526, uint256("0x0000000000406137d7afb03df768f0c6d7ab1efdbc14325d018b62b7ef17fa1c"))
        (34310, uint256("0x0000000000325205f89b7685639fc997248cbaf8dbf2d53ad2e00f98948de58c"))
        (35277, uint256("0x00000000000214b28bc7b1ea230417442417d2381673bcec4ac3ef87c6d0b9f8"))
        (36341, uint256("0x0000000000143a6fd244037ebf301c6898f34c6db428916057eadddec4e35061"))
        (51013, uint256("0x00000000002afdd383affb15708fd329feaa68fdabe477bce4eceec7525dc7f2"))
        (63421, uint256("0x000000000020867e3050e7f4b4402c578215a2174723f614d31d5f21eb61a173"))
        (67755, uint256("0x00000000001151bbacf6f169312aaa0c71e0f71765ceb4e8ebffabc219993ba7"))
        (69198, uint256("0x000000000041498e3911ccbd9aee327bb9bc58dcdf4cd51956909ce5575fccf5"))
        (71945, uint256("0x00000000006d1cb2ad6700614c54a962bbe7d6baeb02c227b9dda2e0a59077da"))
        (74654, uint256("0x00000000001b274b44531f13d5a023ae0450e765e403893c64aea7c6c21eec8e"))
        (77505, uint256("0x00000000003be70f239212ba753a1fdd985fee027e276556619eeb40a771c151"))
        (95971, uint256("0x000000000009d877e435995db1565558e30a7f8ee220cad5d0a4a055d0ebb8bf"))
       (106681, uint256("0x000000000000d081d43eb74429e7344f18c4300796faaa54f5432c4976a9fc2b"))
       (106685, uint256("0x00000000000017e88ad9cf01e74941a134434bf0c39ef254498e4cbb754b604f"))
       (106693, uint256("0x00000000000271eb870d1573f3c1e1ba4c33a56f80333b89219d8affb2a8dadc"))
       (106715, uint256("0x0000000000012d109bfe2a5b464defb0214b5b25090acb9cc0fd9ca054c53d6a"))
       (106725, uint256("0x000000000007c734700d0be1f0c2358f57a009752257da8dcc2206185e9a580a"))
       (106748, uint256("0x000000000000148fc833528722f41af106c08ed2da67e777f5a6b8ec2d60d695"))
       (106753, uint256("0x0000000000003cb9976d3785c2960728bdd67d4ddfce640682b9c9c4df8582d9"))
       (106759, uint256("0x000000000002ed231df0f8c4f21f2a73eeacbbc88f411b438ad4fd4b60418c42"))
       (107368, uint256("0x000000000009062fe1b4c0654b3d152282545aa8beba3c0e4981d9dfa71b1eaa"))
       (108106, uint256("0x00000000000282bf4e2bd0571c42165a67ffede3b81f2387e301369162107020"))
       (110197, uint256("0x0000000000002d863064910c8964f5d8e2883aca9760c19368fe043263e2bfdd"))
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1366364155, // * UNIX timestamp of last checkpoint block
        209955,     // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        2500.0      // * estimated number of transactions per day after checkpoint
    };

    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        (    0, uint256("0x00000000d64b490e447fb522682bfa6bcb27886ed1a94d7a4856fb92ab130875"))
        (   10, uint256("0x00000000ccb062054de57305c0218a25c8c6e6314db64dae02ab8e218bbf67cf"))
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1363138087,
        11,
        730
    };

    const CCheckpointData &Checkpoints() {
        if (fTestNet)
            return dataTestnet;
        else
            return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!GetBoolArg("-checkpoints", true))
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        bool is_valid = hash == i->second;

        printf("CheckBlock() CHECKING checkpoint height=%d isValid(1/0)=%d\n", nHeight, (is_valid ? 1 : 0) );

        return (is_valid);
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        if (pindex==NULL)
            return 0.0;

        int64 nNow = time(NULL);

        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (!GetBoolArg("-checkpoints", true))
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!GetBoolArg("-checkpoints", true))
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

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
