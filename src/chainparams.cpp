// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>

#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <deploymentinfo.h>
#include <hash.h> // for signet block challenge hash
#include <powdata.h>
#include <util/system.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace
{

constexpr const char pszTimestampTestnet[] = "SpaceXpance Testnet";
constexpr const char pszTimestampMainnet[]
    = "01/Jun/2022: "
      "NASA has hosted the Martian Metaverse Creation Challenge";

/* Premined amount is 199,999.998 ROD.  This is the maximum possible number of
   coins needed to support the project for at least 4 years.  If this is not the case
   and we need to reduce the coin supply, excessive coins will be burnt by
   sending to an unspendable OP_RETURN output.  */
constexpr CAmount premineAmount = 199999998 * COIN;

/*
The premine on testnet, signet and regtest is sent to a 1-of-2 multisig address.

The two addresses and corresponding pubkeys are:
    rFAdSr3RHUfu5DU8KFodnCANZNFpRXZV4A
      02dcc2da82ec53da47647f0765e5a36f81786907deaf6b189f22ac38d70d00c1da
    rNGQ9qazxkUGqqoASTbjrJziTHsmTHRJBc
      0289da4bca18786ac1112d280360c186707d32ef2c08b5960dac4a936042727220

This results in the multisig address: xP7BKZBGDU6j7pTLdyXVGr1MT2egJvBCtL
Redeem script:
  522102dcc2da82ec53da47647f0765e5a36f81786907deaf6b189f22ac38d70d00c1da21
  0289da4bca18786ac1112d280360c186707d32ef2c08b5960dac4a93604272722052ae

The constant below is the HASH160 of the redeem script.  In other words, the
final premine script will be:
  OP_HASH160 hexPremineAddress OP_EQUAL
*/
constexpr const char hexPremineAddressRegtest[]
    = "a25f20bd7dd2d450b5475dc0f27115ce3143427b";

/*
The premine on mainnet is sent to a 2-of-4 multisig address.  The
keys are held by the founding members of the SpaceXpanse team.

The address is:
  XaY1dLJjXr7tPizEQGSgwEMwchW32vpZXu

The hash of the redeem script is the constant below.  With it, the final
premine script is:
  OP_HASH160 hexPremineAddress OP_EQUAL
*/
constexpr const char hexPremineAddressMainnet[]
    = "fe546eafc3574b33f1c9e20a4d44680c4e54074d";

CBlock CreateGenesisBlock(const CScript& genesisInputScript, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = genesisInputScript;
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = 0;
    genesis.nNonce   = 0;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);

    std::unique_ptr<CPureBlockHeader> fakeHeader(new CPureBlockHeader ());
    fakeHeader->nNonce = nNonce;
    fakeHeader->hashMerkleRoot = genesis.GetHash ();
    genesis.pow.setCoreAlgo (PowAlgo::NEOSCRYPT);
    genesis.pow.setBits (nBits);
    genesis.pow.setFakeHeader (std::move (fakeHeader));

    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 */
CBlock
CreateGenesisBlock (const uint32_t nTime, const uint32_t nNonce,
                    const uint32_t nBits,
                    const std::string& timestamp,
                    const uint160& premineP2sh)
{
  const std::vector<unsigned char> timestampData(timestamp.begin (),
                                                 timestamp.end ());
  const CScript genesisInput = CScript () << timestampData;

  std::vector<unsigned char>
    scriptHash (premineP2sh.begin (), premineP2sh.end ());
  std::reverse (scriptHash.begin (), scriptHash.end ());
  const CScript genesisOutput = CScript ()
    << OP_HASH160 << scriptHash << OP_EQUAL;

  const int32_t nVersion = 1;
  return CreateGenesisBlock (genesisInput, genesisOutput, nTime, nNonce, nBits,
                             nVersion, premineAmount);
}

/**
 * Mines the genesis block (by finding a suitable nonce only).  When done, it
 * prints the found nonce and block hash and exits.
 */
void MineGenesisBlock (CBlock& block, const Consensus::Params& consensus) 
{
  std::cout << "Mining genesis block..." << std::endl;

  block.nTime = GetTime ();

  auto& fakeHeader = block.pow.initFakeHeader (block);
  while (!block.pow.checkProofOfWork (fakeHeader, consensus))
    {
      assert (fakeHeader.nNonce < std::numeric_limits<uint32_t>::max ());
      ++fakeHeader.nNonce;
      if (fakeHeader.nNonce % 1000 == 0)
        std::cout << "  nNonce = " << fakeHeader.nNonce << "..." << std::endl;
    }

  std::cout << "Found nonce: " << fakeHeader.nNonce << std::endl;
  std::cout << "nTime: " << block.nTime << std::endl;
  std::cout << "Block hash: " << block.GetHash ().GetHex () << std::endl;
  std::cout << "Merkle root: " << block.hashMerkleRoot.GetHex () << std::endl;
  exit (EXIT_SUCCESS);
}

} // anonymous namespace

/**
 * Main network on which people trade goods and services.
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 1054080; // 4216320;
        consensus.initialSubsidy = 800 * COIN;
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 1;
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 2016; // segwit activation height + miner confirmation window
        consensus.powLimitNeoscrypt = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1815; // 90% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        // Deployment of Taproot (BIPs 340-342)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay

        // The best chain should have at least this much work.
        // The value is the chain work of the SpaceXpanse mainnet chain at height
        // 800'000 with best block hash:
        // 4d26cb0da44a06a2f5dc639e921f49a62714b6156256caf8461840adb66dc83f
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000007d524faed2c48c6c828");
        consensus.defaultAssumeValid = uint256S("0x4d26cb0da44a06a2f5dc639e921f49a62714b6156256caf8461840adb66dc83f"); // 800'000

        consensus.nAuxpowChainId = 1899;

        consensus.rules.reset(new Consensus::MainNetConsensus());

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xa2;
        pchMessageStart[1] = 0xf2;
        pchMessageStart[2] = 0xf6;
        pchMessageStart[3] = 0x93;
        nDefaultPort = 11998;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size = 5;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock (1654336219, 18073, 0x1e0ffff0,
                                      pszTimestampMainnet,
                                      uint160S (hexPremineAddressMainnet));
        consensus.hashGenesisBlock = genesis.GetHash();
/*        
        consensus.hashGenesisBlock = uint256S("0x");
        if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) { 
        std::cout << "Mining Mainnet genesis block..." << std::endl;

        genesis.nTime = GetTime ();

        auto& fakeHeader = genesis.pow.initFakeHeader (genesis);
        while (!genesis.pow.checkProofOfWork (fakeHeader, consensus))
          {
            assert (fakeHeader.nNonce < std::numeric_limits<uint32_t>::max ());
            ++fakeHeader.nNonce;
            if (fakeHeader.nNonce % 1000 == 0)
              std::cout << "  nNonce = " << fakeHeader.nNonce << "..." << std::endl;
          }

        std::cout << "Found nonce: " << fakeHeader.nNonce << std::endl;
        std::cout << "nTime: " << genesis.nTime << std::endl;
        std::cout << "Block hash: " << genesis.GetHash ().GetHex () << std::endl;
        std::cout << "Merkle root: " << genesis.hashMerkleRoot.GetHex () << std::endl;
        }
        std::cout << std::string("Finished calculating Mainnet Genesis Block.\n");        
*/        
        assert(consensus.hashGenesisBlock == uint256S("0x5d4b20be4fc87d2333aea5235d9de1c685696fc935f806a9ffd71c9f9abf3c57"));
        assert(genesis.hashMerkleRoot == uint256S("0xafdbec35a16bea610dafafeee5a8cd072dc74a056894a12165da027079d5e138"));

        vSeeds.emplace_back("seed1.spacexpanse.net");
        vSeeds.emplace_back("seed2.spacexpanse.net");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,60);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,75);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,78);
        /* FIXME: Update these below.  */
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xE4, 0xAD};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0x1E, 0xB2};

        bech32_hrp = "rod";

        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_main), std::end(chainparams_seed_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
//            {{ 0, uint256S("0x0")}}, 

            {
                {0, uint256S("5d4b20be4fc87d2333aea5235d9de1c685696fc935f806a9ffd71c9f9abf3c57")},      
                {48550, uint256S("9f7abe9fa74ea774f66a89beebb9381d1bfb6434c132a2d0b12e50ba8634bf69")},    
                {800001, uint256S("c8f940192478381008b63f6b522aa609060fe8024436e68bb0e2d4f617d1c7f3")},   
            }
          
        };

        m_assumeutxo_data = MapAssumeutxo{
         // TODO to be specified in a future patch.
        };

        chainTxData = ChainTxData{
            // Data from RPC: getchaintxstats 800001 c8f940192478381008b63f6b522aa609060fe8024436e68bb0e2d4f617d1c7f3
            /* nTime    */ 1680320335, // 1626099379,
            /* nTxCount */ 92864, // 4457837,
            /* dTxRate  */ 0.03371758688912199, // 0.034450420845411,
        };
    }

    int DefaultCheckNameDB () const override
    {
        return -1;
    }
};

/**
 * Testnet (v3): public test network which is reset from time to time.
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 1054080; // 2880;
        consensus.initialSubsidy = 800 * COIN; //10 * COIN;
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 1;
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 2016; // segwit activation height + miner confirmation window
        consensus.MinBIP9WarningHeight = consensus.SegwitHeight + consensus.nMinerConfirmationWindow;
        consensus.powLimitNeoscrypt = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        // Deployment of Taproot (BIPs 340-342)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay

        // The value is the chain work of the SpaceXpanse testnet chain at height
        // 110'000 with best block hash:
        // 01547d538737e01d81d207e7d2f4c8f2510c6b82f0ee5dd8cd6c26bed5a03d0f
        consensus.nMinimumChainWork = uint256S("0x0");
        consensus.defaultAssumeValid = uint256S("0x0"); // 110'000

        consensus.nAuxpowChainId = 1899;

        consensus.rules.reset(new Consensus::TestNetConsensus());

        pchMessageStart[0] = 0xc8;
        pchMessageStart[1] = 0xc3;
        pchMessageStart[2] = 0x95;
        pchMessageStart[3] = 0x87;
        nDefaultPort = 18398;
        nPruneAfterHeight = 1000;
        m_assumed_blockchain_size = 1;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock (1654336227, 2573921, 0x1e0ffff0,
                                      pszTimestampTestnet,
                                      uint160S (hexPremineAddressRegtest));
        consensus.hashGenesisBlock = genesis.GetHash();
/*        
        consensus.hashGenesisBlock = uint256S("0x");
        if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) { 
        std::cout << "Mining TestNet genesis block..." << std::endl;

        genesis.nTime = GetTime ();

        auto& fakeHeader = genesis.pow.initFakeHeader (genesis);
        while (!genesis.pow.checkProofOfWork (fakeHeader, consensus))
          {
            assert (fakeHeader.nNonce < std::numeric_limits<uint32_t>::max ());
            ++fakeHeader.nNonce;
            if (fakeHeader.nNonce % 1000 == 0)
              std::cout << "  nNonce = " << fakeHeader.nNonce << "..." << std::endl;
          }

        std::cout << "Found nonce: " << fakeHeader.nNonce << std::endl;
        std::cout << "nTime: " << genesis.nTime << std::endl;
        std::cout << "Block hash: " << genesis.GetHash ().GetHex () << std::endl;
        std::cout << "Merkle root: " << genesis.hashMerkleRoot.GetHex () << std::endl;
        }
        std::cout << std::string("Finished calculating TestNet Genesis Block.\n");        
*/        
        assert(consensus.hashGenesisBlock == uint256S("0x30d791386d3fce328be9dce11abfb289d8cf6c06d4947950834d1e54044f037c"));
        assert(genesis.hashMerkleRoot == uint256S("0x159f4b9d14e17ebdba22ac6ae5781d5c7f39cb22328bd16486b84202fee9de06"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.emplace_back("seed1.testnet.spacexpanse.net");
        vSeeds.emplace_back("seed2.testnet.spacexpanse.net");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,122);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,137);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,140);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rodtn";

        // FIXME: Namecoin has no fixed seeds for testnet, so that the line
        // below errors out.  Use it once we have testnet seeds.
        //vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_test), std::end(chainparams_seed_test));
        vFixedSeeds.clear();

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
//            {{ 0, uint256S("0x0")}}, 

            {
                {1, uint256S("15d3d57c15dd8dcfdcc03e69c4c0647416789083a17fd1c88902112399855a48")},   
                {2880, uint256S("48d26b2b69c4d70d2284d983e10341c034f7fb3d36205677cd8d6198991764b3")},   
            }
 
        };

        m_assumeutxo_data = MapAssumeutxo{
            // TODO to be specified in a future patch.
        };

        chainTxData = ChainTxData{
            // Data from rpc: getchaintxstats 2880 48d26b2b69c4d70d2284d983e10341c034f7fb3d36205677cd8d6198991764b3
            /* nTime    */ 1655635459, // 1586091497,
            /* nTxCount */ 2882, // 113579,
            /* dTxRate  */ 0.003367995621605692, // 0.002815363095612851,
        };
    }

    int DefaultCheckNameDB () const override
    {
        return -1;
    }
};

/**
 * Signet: test network with an additional consensus parameter (see BIP325).
 */
class SigNetParams : public CChainParams {
public:
    explicit SigNetParams(const ArgsManager& args) {
        std::vector<uint8_t> bin;
        vSeeds.clear();

        if (!args.IsArgSet("-signetchallenge")) {
            /* FIXME: Adjust the default signet challenge to something else if
               we want to use signet for Namecoin.  */
            bin = ParseHex("512103ad5e0edad18cb1f0fc0d28a3d4f1f3e445640337489abb10404f2d1e086be430210359ef5021964fe22d6f8e05b2463c9540ce96883fe3b278760f048f5189f2e6c452ae");
            //vSeeds.emplace_back("178.128.221.177");

            consensus.nMinimumChainWork = uint256S("0x0");
            consensus.defaultAssumeValid = uint256S("0x0"); // 47200
            m_assumed_blockchain_size = 1;
            m_assumed_chain_state_size = 0;
            chainTxData = ChainTxData{
                // Data from RPC: getchaintxstats 4096 000000187d4440e5bff91488b700a140441e089a8aaea707414982460edbfe54
                /* nTime    */ 0, // 1626696658,
                /* nTxCount */ 0, // 387761,
                /* dTxRate  */ 0, // 0.04035946932424404,
            };
        } else {
            const auto signet_challenge = args.GetArgs("-signetchallenge");
            if (signet_challenge.size() != 1) {
                throw std::runtime_error(strprintf("%s: -signetchallenge cannot be multiple values.", __func__));
            }
            bin = ParseHex(signet_challenge[0]);

            consensus.nMinimumChainWork = uint256{};
            consensus.defaultAssumeValid = uint256{};
            m_assumed_blockchain_size = 0;
            m_assumed_chain_state_size = 0;
            chainTxData = ChainTxData{
                0,
                0,
                0,
            };
            LogPrintf("Signet with challenge %s\n", signet_challenge[0]);
        }

        if (args.IsArgSet("-signetseednode")) {
            vSeeds = args.GetArgs("-signetseednode");
        }

        strNetworkID = CBaseChainParams::SIGNET;
        consensus.signet_blocks = true;
        consensus.signet_challenge.assign(bin.begin(), bin.end());
        consensus.nSubsidyHalvingInterval = 2880; // 2880;
        consensus.BIP16Height = 1;
        consensus.BIP34Height = 1;
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 1;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1815; // 90% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimitNeoscrypt = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        // Activation of Taproot (BIPs 340-342)
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay

        consensus.nAuxpowChainId = 1899;

        consensus.rules.reset(new Consensus::TestNetConsensus());

        // message start is defined as the first 4 bytes of the sha256d of the block script
        CHashWriter h(SER_DISK, 0);
        h << consensus.signet_challenge;
        uint256 hash = h.GetHash();
        memcpy(pchMessageStart, hash.begin(), 4);

        nDefaultPort = 38398;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock (1654337344, 20993, 0x1e0ffff0,
                                      pszTimestampTestnet,
                                      uint160S (hexPremineAddressMainnet));
        consensus.hashGenesisBlock = genesis.GetHash();
/*        
        consensus.hashGenesisBlock = uint256S("0x");
        if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) { 
        std::cout << "Mining Signet genesis block..." << std::endl;

        genesis.nTime = GetTime ();

        auto& fakeHeader = genesis.pow.initFakeHeader (genesis);
        while (!genesis.pow.checkProofOfWork (fakeHeader, consensus))
          {
            assert (fakeHeader.nNonce < std::numeric_limits<uint32_t>::max ());
            ++fakeHeader.nNonce;
            if (fakeHeader.nNonce % 1000 == 0)
              std::cout << "  nNonce = " << fakeHeader.nNonce << "..." << std::endl;
          }

        std::cout << "Found nonce: " << fakeHeader.nNonce << std::endl;
        std::cout << "nTime: " << genesis.nTime << std::endl;
        std::cout << "Block hash: " << genesis.GetHash ().GetHex () << std::endl;
        std::cout << "Merkle root: " << genesis.hashMerkleRoot.GetHex () << std::endl;
        }
        std::cout << std::string("Finished calculating Signet Genesis Block.\n");        
*/        
        assert(consensus.hashGenesisBlock == uint256S("0x9aee26a672738ed0546bd5c12e094e808fd05a008be54a295af8e92c5d77a507"));
        assert(genesis.hashMerkleRoot == uint256S("0x340b7fb90cb28c4a4b145785678e0acebc05b662b40cf4018472bc608115b3c2"));

        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,122);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,137);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,140);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rodtb";

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = false;
    }

    int DefaultCheckNameDB () const override
    {
        return -1;
    }
};

/**
 * Regression test: intended for private networks only. Has minimal difficulty to ensure that
 * blocks can be found instantly.
 */
class CRegTestParams : public CChainParams {
public:
    explicit CRegTestParams(const ArgsManager& args) {
        strNetworkID =  CBaseChainParams::REGTEST;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.nSubsidyHalvingInterval = 360; // 150;
        // The subsidy for regtest net is kept same as upstream Bitcoin, so
        // that we don't have to update many of the tests unnecessarily.
        consensus.initialSubsidy = 800 * COIN; //50 * COIN;
        consensus.BIP16Height = 0;
        consensus.BIP34Height = 500; // BIP34 activated on regtest (Used in functional tests)
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in functional tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in functional tests)
        consensus.CSVHeight = 432; // CSV activated on regtest (Used in rpc activation tests)
        consensus.SegwitHeight = 0; // SEGWIT is always activated on regtest unless overridden
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimitNeoscrypt = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay

        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};

        consensus.nAuxpowChainId = 1899;

        consensus.rules.reset(new Consensus::RegTestConsensus());

        pchMessageStart[0] = 0xce;
        pchMessageStart[1] = 0xb3;
        pchMessageStart[2] = 0xbb;
        pchMessageStart[3] = 0xd4;
        nDefaultPort = 18498;
        nPruneAfterHeight = args.GetBoolArg("-fastprune", false) ? 100 : 1000;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        UpdateActivationParametersFromArgs(args);

        genesis = CreateGenesisBlock (1654337353, 0, 0x207fffff,
                                      pszTimestampTestnet,
                                      uint160S (hexPremineAddressRegtest));
        consensus.hashGenesisBlock = genesis.GetHash();
/*        
        consensus.hashGenesisBlock = uint256S("0x");
        if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) { 
        std::cout << "Mining RegTest genesis block..." << std::endl;

        genesis.nTime = GetTime ();

        auto& fakeHeader = genesis.pow.initFakeHeader (genesis);
        while (!genesis.pow.checkProofOfWork (fakeHeader, consensus))
          {
            assert (fakeHeader.nNonce < std::numeric_limits<uint32_t>::max ());
            ++fakeHeader.nNonce;
            if (fakeHeader.nNonce % 1000 == 0)
              std::cout << "  nNonce = " << fakeHeader.nNonce << "..." << std::endl;
          }

        std::cout << "Found nonce: " << fakeHeader.nNonce << std::endl;
        std::cout << "nTime: " << genesis.nTime << std::endl;
        std::cout << "Block hash: " << genesis.GetHash ().GetHex () << std::endl;
        std::cout << "Merkle root: " << genesis.hashMerkleRoot.GetHex () << std::endl;
        }
        std::cout << std::string("Finished calculating RegTest Genesis Block.\n");        
*/        
        assert(consensus.hashGenesisBlock == uint256S("0xfa630c42f5e250d75191914d8d894e3f1e8fd54f750430e279becf86c42abd12"));
        assert(genesis.hashMerkleRoot == uint256S("0x159f4b9d14e17ebdba22ac6ae5781d5c7f39cb22328bd16486b84202fee9de06"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = true;

        checkpointData =  {
            {{ 0, uint256S("0x0")}}, 
/*
            {
                {0, uint256S("fa37a72ecf6241368fafcb4a4c49abe2ba06614f9bd06cb62fa05a5975303765")},
            }
*/ 
        };

        m_assumeutxo_data = MapAssumeutxo{
/*            {
                110,
                {AssumeutxoHash{uint256S("0xdc81af66a58085fe977c6aab56b49630d87b84521fc5a8a5c53f2f4b23c8d6d5")}, 110},
            },
            {
                200,
                {AssumeutxoHash{uint256S("0x51c8d11d8b5c1de51543c579736e786aa2736206d1e11e627568029ce092cf62")}, 200},
            },  */
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,122);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,137);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,140);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "rodrt";
    }

    int DefaultCheckNameDB () const override
    {
        return 0;
    }

    /**
     * Allows modifying the Version Bits regtest parameters.
     */
    void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout, int min_activation_height)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
        consensus.vDeployments[d].min_activation_height = min_activation_height;
    }
    void UpdateActivationParametersFromArgs(const ArgsManager& args);
};

void CRegTestParams::UpdateActivationParametersFromArgs(const ArgsManager& args)
{
    if (args.IsArgSet("-bip16height")) {
        int64_t height = args.GetArg("-bip16height", consensus.BIP16Height);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for BIP16 is out of valid range. Use -1 to disable BIP16.", height));
        } else if (height == -1) {
            LogPrintf("BIP16 disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.BIP16Height = static_cast<int>(height);
    }
    if (args.IsArgSet("-segwitheight")) {
        int64_t height = args.GetArg("-segwitheight", consensus.SegwitHeight);
        if (height < -1 || height >= std::numeric_limits<int>::max()) {
            throw std::runtime_error(strprintf("Activation height %ld for segwit is out of valid range. Use -1 to disable segwit.", height));
        } else if (height == -1) {
            LogPrintf("Segwit disabled for testing\n");
            height = std::numeric_limits<int>::max();
        }
        consensus.SegwitHeight = static_cast<int>(height);
    }

    if (!args.IsArgSet("-vbparams")) return;

    for (const std::string& strDeployment : args.GetArgs("-vbparams")) {
        std::vector<std::string> vDeploymentParams;
        boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
        if (vDeploymentParams.size() < 3 || 4 < vDeploymentParams.size()) {
            throw std::runtime_error("Version bits parameters malformed, expecting deployment:start:end[:min_activation_height]");
        }
        int64_t nStartTime, nTimeout;
        int min_activation_height = 0;
        if (!ParseInt64(vDeploymentParams[1], &nStartTime)) {
            throw std::runtime_error(strprintf("Invalid nStartTime (%s)", vDeploymentParams[1]));
        }
        if (!ParseInt64(vDeploymentParams[2], &nTimeout)) {
            throw std::runtime_error(strprintf("Invalid nTimeout (%s)", vDeploymentParams[2]));
        }
        if (vDeploymentParams.size() >= 4 && !ParseInt32(vDeploymentParams[3], &min_activation_height)) {
            throw std::runtime_error(strprintf("Invalid min_activation_height (%s)", vDeploymentParams[3]));
        }
        bool found = false;
        for (int j=0; j < (int)Consensus::MAX_VERSION_BITS_DEPLOYMENTS; ++j) {
            if (vDeploymentParams[0] == VersionBitsDeploymentInfo[j].name) {
                UpdateVersionBitsParameters(Consensus::DeploymentPos(j), nStartTime, nTimeout, min_activation_height);
                found = true;
                LogPrintf("Setting version bits activation parameters for %s to start=%ld, timeout=%ld, min_activation_height=%d\n", vDeploymentParams[0], nStartTime, nTimeout, min_activation_height);
                break;
            }
        }
        if (!found) {
            throw std::runtime_error(strprintf("Invalid deployment (%s)", vDeploymentParams[0]));
        }
    }
}

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN) {
        return std::unique_ptr<CChainParams>(new CMainParams());
    } else if (chain == CBaseChainParams::TESTNET) {
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    } else if (chain == CBaseChainParams::SIGNET) {
        return std::unique_ptr<CChainParams>(new SigNetParams(args));
    } else if (chain == CBaseChainParams::REGTEST) {
        return std::unique_ptr<CChainParams>(new CRegTestParams(args));
    }
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(gArgs, network);
}

int64_t
AvgTargetSpacing (const Consensus::Params& params, const unsigned height)
{
  /* The average target spacing for any block (all algorithms combined) is
     computed by dividing some common multiple timespan of all spacings
     by the number of blocks expected (all algorithms together) in that
     time span.

     The numerator is simply the product of all block times, while the
     denominator is a sum of products that just excludes the current
     algorithm (i.e. of all (N-1) tuples selected from the N algorithm
     block times).  */
  int64_t numer = 1;
  int64_t denom = 0;
  for (const PowAlgo algo : {PowAlgo::SHA256D, PowAlgo::NEOSCRYPT})
    {
      const int64_t spacing = params.rules->GetTargetSpacing(algo, height);

      /* Multiply all previous added block counts by this target spacing.  */
      denom *= spacing;

      /* Add the number of blocks for the current algorithm to the denominator.
         This starts off with the product of all already-processed algorithms
         (excluding the current one), and will be multiplied later on by
         the still-to-be-processed ones (in the line above).  */
      denom += numer;

      /* The numerator is the product of all spacings.  */
      numer *= spacing;
    }

  assert (denom > 0);
  assert (numer % denom == 0);
  return numer / denom;
}
