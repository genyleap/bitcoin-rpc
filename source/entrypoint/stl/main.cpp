#include "bitcoinclient.hpp"
#include <print>

auto main() -> int {

    // Initialize BitcoinClient with RPC credentials
    BitcoinClient client("root", "rpcpassword", "http://127.0.0.1:8332/");

    // Fetch blockchain information
    Json::Value blockchainInfo = client.getBlockchainInfo();
    if (!blockchainInfo.isNull()) {
        std::print("Blockchain Information:\n");
        std::print("Chain: {}\n", blockchainInfo["chain"].asString());
        std::print("Blocks: {}\n", blockchainInfo["blocks"].asInt());
        std::print("Headers: {}\n", blockchainInfo["headers"].asInt());
        std::print("Best Block Hash: {}\n", blockchainInfo["bestblockhash"].asString());
        std::print("Difficulty: {}\n", blockchainInfo["difficulty"].asFloat());
        std::print("Verification Progress: {}\n", blockchainInfo["verificationprogress"].asFloat());
    } else {
        std::print(stderr, "Failed to fetch blockchain information.\n");
    }

    return 0;
}
