
Tokugawa development tree 

Tokugawa is a PoS-based cryptocurrency. 

Download Wallet : https://github.com/TokugawaCoin/Tokugawa/releases/tag/1.0.0.0

TOK is dependent upon libsecp256k1 by sipa, the sources for which can be found here:
https://github.com/bitcoin/secp256k1

Total POW: 2,000 Blocks
POW Reward: upto 100 Block 10000 TOK per Block, after that 14 TOK
POS Reward: 14 TOK (HiPOS)
Block Spacing: 72 Seconds
Diff Retarget: Every Blocks
Maturity: 46 Blocks
Stake Minimum Age: 24 Hours

3 MegaByte Maximum Block Size (40X Bitcoin Core)

Port: 21117
RPC Port: 21116

Magic Bytes: 0x7e 0xcd 0x1c 0x23

TOK includes an Address Index feature, based on the address index API (searchrawtransactions RPC command) implemented in Bitcoin Core but modified implementation to work with the TOK codebase (PoS coins maintain a txindex by default for instance).

Initialize the Address Index By Running with -reindexaddr Command Line Argument.  It may take 10-15 minutes to build the initial index.


