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
		(279945, uint256("0xc862c7328044d40f1b432e65014513d4d7c9ff051425afcd85f959147cbbc4a7"))
		(279999, uint256("0x5c16afcf3e832537fedd0c5c3ab34cf2c3815e83fd965b58186a4ed7232d74b7"))
		(280013, uint256("0x158e681ef127e348b40ada27c99b72445870c9861500f462572bec4e1c863e3f"))
		(280026, uint256("0x5c54bf0eb4f5f4b520e513daa73ab060a485cb4cf17003872eda4a81a5c412ef"))
		(280069, uint256("0xeac0b4e06c3ef1ac3bc21f9990df82df52337ec2a9332b3b2c4b1b35b0095830"))
		(280148, uint256("0x15ec37658e0dee92448188f957b7ef47601a2fea3de9d043a14ef8a8fb98894f"))
		(280193, uint256("0x09f89c1a01f3c30f42b74c0ec26414a6941417d8f1229aa25f7d5fd0a767b728"))
		(280231, uint256("0x2a93fd7d062af8f0c18ed56d1a729b46c9a3ede22b82ac6a6f6e44b63b1a911c"))
		(280254, uint256("0x4bdcd89183c3815bbbd9f56fcc29bd7aeef27cbb2a5b2b966f22abf558c2cf3e"))
		(280301, uint256("0xfe852af73fcff5144cc054fa5fd43d56d562520fc4a7408362c42e69f37ba5c9"))
		(280362, uint256("0x66512351e0f99dfc13146e38d61106124b8b7281948fa8e525f19b03814ef0a8"))
		(280389, uint256("0xc78268fd736cceb3796e40141120bd8fc1e85d889fbc92a1126b36b81b78a8dd"))
		(280412, uint256("0x364708fbfd28a154e89be1f29d156975c0d538c02bd1cbac77429f6694895e11"))
		(280557, uint256("0x3a47e8710693855dd273d1f0b4367e6d15de809ab067d3421794deded67ad5b6"))
		(280802, uint256("0x1480afc694e2001bbfa8b7ed4b564440fbe40fa638dcc21e52315c487511f8b1"))
		(281176, uint256("0x4e10e9c3b08ddab21f584c2e7a15fbc3118354bd4c32699642f274a56da1351a"))
		(281574, uint256("0x9023e165b92d64c8f7577a9348a72baf2b3b62a100ea27107791a2a65b2c31aa"))
		(281789, uint256("0x15daa022214864be2422bff3b335864479169b33016614a56289e65fdafb1b04"))
		(281903, uint256("0xd6b0c537386caeb710e474823c0c9afdfbf78e94b3db0aca1f12e9139736e068"))
		(282342, uint256("0x08ab5b5b3c21694d5d870ccda797d7943bd4eaf0a89228168cc453802327e7ec"))
		(282519, uint256("0x6c1a408f40415c34f5e4f36bdeb933ce29e1538f8cdc120860077597d068a74d"))
		(282810, uint256("0x3a30aa3ae41e2812d5129a90098ddadd8a9b042b2cc4cf6148dbbef0a5102426"))


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
