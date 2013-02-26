#!/bin/sh
#
# contrib/check-trc-chain-source.sh :
#
# Ensure TRC blockchain paramaters are still present in source tree.
#
# run me from contrib directory.
#

exit_fail() {
    echo "FAILURE: "$1
    exit 1
}

grep 'static const int64 MAX_MONEY = 42000000 \* COIN;' ../src/main.h || exit_fail "MAX_MONEY"

grep 'int64 nSubsidy = 20 \* COIN;' ../src/main.cpp || exit_fail "block reward"

grep 'nSubsidy >>= (nHeight \/ 1050000);' ../src/main.cpp || exit_fail "subsidy"

grep 'static const int64 nTargetTimespan = 60 \* 60;' ../src/main.cpp || exit_fail "nTargetTimespan"

grep 'static const int64 nTargetSpacing = 2 \* 60;' ../src/main.cpp || exit_fail "nTargetSpacing"

grep 'pchMessageStart\[4\] = { 0x42, 0xba, 0xbe, 0x56 };' ../src/main.cpp || exit_fail "magic"

grep 'block.nTime    = 1351242683;' ../src/main.cpp || exit_fail "genesis nTime"

grep 'block.nBits    = 0x1d00ffff;' ../src/main.cpp || exit_fail "genesis nBits"

grep 'block.nNonce   = 2820375594;' ../src/main.cpp || exit_fail "genesis nNonce"

grep 'pszTimestamp = "June 4th 1978 - March 6th 2009 ; Rest In Peace, Stephanie.";' ../src/main.cpp || exit_fail "genesis timestamp"

grep 'uint256 hashGenesisBlock("0x00000000804bbc6a621a9dbb564ce469f492e1ccf2d70f8a6b241e26a277afa2");' ../src/main.cpp || exit_fail "genesis hash"

grep 'assert(block.hashMerkleRoot == uint256("0f8b09f93803b067580c16c3f3a6aaa901be06ad892cea9f02d8a4f93628f196"));' ../src/main.cpp || exit_fail "genesis merkleroot"

grep '(    0, uint256("0x00000000804bbc6a621a9dbb564ce469f492e1ccf2d70f8a6b241e26a277afa2"))' ../src/checkpoints.cpp || exit_fail "checkpoints 0"
grep '(77505, uint256("0x00000000003be70f239212ba753a1fdd985fee027e276556619eeb40a771c151"))' ../src/checkpoints.cpp || exit_fail "checkpoints 77505"






