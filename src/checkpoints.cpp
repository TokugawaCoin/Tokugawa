// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

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
		(0, uint256("0x000097f9640078c01cd7b8455288ebdcf792ecb29af4925eacf13ac9749a6c7b"))
		(3, uint256("0xd0397a0caec4fd7e0c3735f784b2f6f95379ddecfd8afcc667155908589d6f16"))
		(5, uint256("0xfa0785f788f5815cfa67a5eb1c46dfff64e51c5dd7babe0f0f10ced223c8d998"))
		(16, uint256("0x0668d98b2442297571c22af05ad4453f2df7a612462d87b5d2e72597bc9d14db"))
		(25, uint256("0xe4d98ae0ed4137acae4ca2bc7b12e07c9efaca042b91bd02190f8914b87093c3"))
		(29, uint256("0x841a9680e3c54af06d176d15632b106704419be0c38f26eae34145b073fd994a"))
		(32, uint256("0x35e3f6713940697e8b44271898e8eafbae9c9a56c1790c89062b1f980f03d038"))
		(99, uint256("0x3fe63a24c923410494a2fb891f5298d4a8db5224b8aa603b964dccbd10510d9d"))
		(582, uint256("0xd17a3e43b94992792c16d705b5a3a90843b9b36d566f46e2348f025eae334510"))
		(2547, uint256("0x8847fa57e1481c07746838d50b74f53437f1c1ba969839d4698a3a6a3f20272c"))
		(8651, uint256("0xc16157cc94ba479b5e957d0d084e901dd1be930492fb2f001a2ac406287372c1"))
		(25423, uint256("0x514200c3298356f923001f5279533ac675120ab27acf8984c42ed9a4d41525b0"))
		(47621, uint256("0xf28a024008678b5b96fe4a8b7ee57be0beac635e085742de521962a04a30db3e"))
		(106010, uint256("0xd68741a52545a76d83c072ed7b3ce9fba4fb6f2aad98f17768860d1fe661bf0f"))
		(186224, uint256("0x7783b6aff5018422e4fa8538bc79d46966d4bac798e85811a5a1dd1fa4312d33"))
		(225846, uint256("0xfb8c589903dfd763f5e02524bd2ba3c12ed6614a04dd6a62595e97009588dce7"))
		(259100, uint256("0x751ad889f5e9629847e1306357d1480d584da88b7dc750f5b5d18591640cfefc"))
		(279840, uint256("0xb2bc7ccc2353a76af72a7391bb5b6e8e35982d53b1939012b0631d4d08fcf7c5"))
		(279841, uint256("0x5229ece8a98673d6e2527fec6984c8191d0df183992d227a755b91b93683f8a3"))
		(279842, uint256("0xd85fe0cb2a5d58cdc12a65197c2d31dac378151a96e737f4f8bc88572d891b00"))
		(279843, uint256("0xcbc70b07451ae68f6bca400665526643382ca435cd4516d8f430628e4eb8dace"))
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
