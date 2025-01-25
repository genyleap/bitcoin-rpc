#ifndef BITCOINCLIENT_HPP
#define BITCOINCLIENT_HPP

#if __has_include("network.hpp")
#   include "network.hpp"
#else
#   error "Bitcoin's \"common.hpp\" was not found!"
#endif


#if __has_include("logger.hpp")
#   include "logger.hpp"
#else
#   error "Bitcoin's \"logger.hpp\" was not found!"
#endif


#if __has_include(<json/json.h>)
#   include <json/json.h>
#else
#   error "Bitcoin's <json/json.h> was not found!"
#endif


/**
 * @class BitcoinClient
 * @brief A client for interacting with a Bitcoin node via JSON-RPC.
 *
 * This class provides methods to communicate with a Bitcoin node using JSON-RPC.
 * It supports various RPC methods for blockchain, control, generating, mining,
 * network, raw transactions, utility, and wallet operations.
 */
class BitcoinClient {
private:
    std::string rpcUser;       ///< RPC username for authentication.
    std::string rpcPassword;   ///< RPC password for authentication.
    std::string rpcUrl;        ///< URL of the Bitcoin RPC server.
    Network network;           ///< Network instance for handling HTTP requests.

    /**
     * @brief Builds the JSON-RPC request payload.
     * @param method The RPC method to call.
     * @param params The parameters for the RPC method.
     * @return A JSON-RPC formatted string.
     */
    std::string buildRpcRequest(const std::string& method, const Json::Value& params);

    /**
     * @brief Parses the JSON-RPC response.
     * @param response The raw JSON response from the server.
     * @return A Json::Value object containing the parsed response.
     */
    Json::Value parseRpcResponse(const std::string& response);

    /**
     * @brief Helper method to build a Json::Value array from variadic parameters.
     * @tparam Args Types of the parameters.
     * @param args Parameters to append to the JSON array.
     * @return A Json::Value array containing the parameters.
     */
    template<typename... Args>
    Json::Value buildParams(Args&&... args) {
        Json::Value params;
        (params.append(std::forward<Args>(args)), ...);
        return params;
    }

public:
    /**
     * @brief Constructor for BitcoinClient.
     * @param user RPC username.
     * @param password RPC password.
     * @param url RPC server URL (default: http://127.0.0.1:8332/).
     */
    BitcoinClient(const std::string& user, const std::string& password, const std::string& url = "http://127.0.0.1:8332/");

    /**
     * @brief Sends a generic JSON-RPC request to the Bitcoin server.
     * @param method The RPC method to call.
     * @param params The parameters for the RPC method (default: empty).
     * @return A Json::Value object containing the response.
     */
    Json::Value sendRequest(const std::string& method, const Json::Value& params = Json::Value());

           // Blockchain RPCs
    /**
     * @brief Returns the hash of the best (tip) block in the blockchain.
     * @return A Json::Value containing the block hash.
     */
    Json::Value getBestBlockHash();

    /**
     * @brief Returns information about a block.
     * @param blockHash The hash of the block.
     * @param verbose If true, returns full block details; otherwise, returns hex-encoded block data.
     * @return A Json::Value containing the block information.
     */
    Json::Value getBlock(const std::string& blockHash, bool verbose = true);

    /**
     * @brief Returns information about the blockchain.
     * @return A Json::Value containing blockchain information.
     */
    Json::Value getBlockchainInfo();

    /**
     * @brief Returns the number of blocks in the blockchain.
     * @return A Json::Value containing the block count.
     */
    Json::Value getBlockCount();

    /**
     * @brief Returns the filter for a block.
     * @param blockHash The hash of the block.
     * @param filterType The type of filter to retrieve.
     * @return A Json::Value containing the block filter.
     */
    Json::Value getBlockFilter(const std::string& blockHash, const std::string& filterType);

    /**
     * @brief Returns the hash of a block at a specific height.
     * @param height The height of the block.
     * @return A Json::Value containing the block hash.
     */
    Json::Value getBlockHash(int height);

    /**
     * @brief Returns information about a block header.
     * @param blockHash The hash of the block.
     * @param verbose If true, returns full header details; otherwise, returns hex-encoded header data.
     * @return A Json::Value containing the block header information.
     */
    Json::Value getBlockHeader(const std::string& blockHash, bool verbose = true);

    /**
     * @brief Returns statistics for a block.
     * @param blockHash The hash of the block.
     * @param stats A list of statistics to retrieve (default: all).
     * @return A Json::Value containing the block statistics.
     */
    Json::Value getBlockStats(const std::string& blockHash, const std::vector<std::string>& stats = {});

    /**
     * @brief Returns information about all known blockchain tips.
     * @return A Json::Value containing the chain tips.
     */
    Json::Value getChainTips();

    /**
     * @brief Returns statistics about the blockchain.
     * @param nBlocks The number of blocks to consider (default: 0 for all blocks).
     * @param blockHash The hash of the block to start from (default: empty for the chain tip).
     * @return A Json::Value containing the transaction statistics.
     */
    Json::Value getChainTxStats(int nBlocks = 0, const std::string& blockHash = "");

    /**
     * @brief Returns the current mining difficulty.
     * @return A Json::Value containing the difficulty.
     */
    Json::Value getDifficulty();

    /**
     * @brief Returns the ancestors of a transaction in the mempool.
     * @param txid The transaction ID.
     * @param verbose If true, returns full transaction details; otherwise, returns transaction IDs.
     * @return A Json::Value containing the ancestors.
     */
    Json::Value getMempoolAncestors(const std::string& txid, bool verbose = false);

    /**
     * @brief Returns the descendants of a transaction in the mempool.
     * @param txid The transaction ID.
     * @param verbose If true, returns full transaction details; otherwise, returns transaction IDs.
     * @return A Json::Value containing the descendants.
     */
    Json::Value getMempoolDescendants(const std::string& txid, bool verbose = false);

    /**
     * @brief Returns information about a transaction in the mempool.
     * @param txid The transaction ID.
     * @return A Json::Value containing the mempool entry.
     */
    Json::Value getMempoolEntry(const std::string& txid);

    /**
     * @brief Returns information about the mempool.
     * @return A Json::Value containing the mempool information.
     */
    Json::Value getMempoolInfo();

    /**
     * @brief Returns the raw mempool data.
     * @param verbose If true, returns full transaction details; otherwise, returns transaction IDs.
     * @return A Json::Value containing the raw mempool data.
     */
    Json::Value getRawMempool(bool verbose = false);

    /**
     * @brief Returns details about a transaction output.
     * @param txid The transaction ID.
     * @param n The index of the output.
     * @param includeMempool If true, includes unconfirmed transactions in the mempool.
     * @return A Json::Value containing the transaction output details.
     */
    Json::Value getTxOut(const std::string& txid, int n, bool includeMempool = true);

    /**
     * @brief Returns a proof that a transaction is included in a block.
     * @param txids A list of transaction IDs.
     * @param blockHash The hash of the block (default: empty for the chain tip).
     * @return A Json::Value containing the transaction proof.
     */
    Json::Value getTxOutProof(const std::vector<std::string>& txids, const std::string& blockHash = "");

    /**
     * @brief Returns statistics about the UTXO set.
     * @return A Json::Value containing the UTXO set information.
     */
    Json::Value getTxOutSetInfo();

    /**
     * @brief Treats a block as if it were received before others with the same work.
     * @param blockHash The hash of the block.
     * @return A Json::Value containing the result.
     */
    Json::Value preciousBlock(const std::string& blockHash);

    /**
     * @brief Prunes the blockchain up to a specific height.
     * @param height The height to prune to.
     * @return A Json::Value containing the result.
     */
    Json::Value pruneBlockchain(int height);

    /**
     * @brief Saves the mempool to disk.
     * @return A Json::Value containing the result.
     */
    Json::Value saveMempool();

    /**
     * @brief Scans the UTXO set for descriptors.
     * @param descriptors A list of descriptors to scan.
     * @return A Json::Value containing the scan results.
     */
    Json::Value scanTxOutSet(const std::vector<std::string>& descriptors);

    /**
     * @brief Verifies the blockchain database.
     * @param checkLevel The level of verification (default: 3).
     * @param nBlocks The number of blocks to verify (default: 6).
     * @return A Json::Value containing the verification result.
     */
    Json::Value verifyChain(int checkLevel = 3, int nBlocks = 6);

    /**
     * @brief Verifies a transaction proof.
     * @param proof The transaction proof to verify.
     * @return A Json::Value containing the verification result.
     */
    Json::Value verifyTxOutProof(const std::string& proof);

           // Control RPCs
    /**
     * @brief Returns memory usage information.
     * @return A Json::Value containing memory usage details.
     */
    Json::Value getMemoryInfo();

    /**
     * @brief Returns information about the RPC server.
     * @return A Json::Value containing RPC server information.
     */
    Json::Value getRpcInfo();

    /**
     * @brief Returns help information for a command.
     * @param command The command to get help for (default: empty for general help).
     * @return A Json::Value containing the help information.
     */
    Json::Value help(const std::string& command = "");

    /**
     * @brief Manages logging settings.
     * @param include A list of log categories to include.
     * @param exclude A list of log categories to exclude.
     * @return A Json::Value containing the logging settings.
     */
    Json::Value logging(const std::vector<std::string>& include, const std::vector<std::string>& exclude);

    /**
     * @brief Stops the Bitcoin server.
     * @return A Json::Value containing the result.
     */
    Json::Value stop();

    /**
     * @brief Returns the uptime of the Bitcoin server.
     * @return A Json::Value containing the uptime.
     */
    Json::Value uptime();

           // Generating RPCs
    /**
     * @brief Generates a block with the given transactions.
     * @param outputAddress The address to receive the block reward.
     * @param transactions A list of transactions to include in the block.
     * @return A Json::Value containing the generated block.
     */
    Json::Value generateBlock(const std::string& outputAddress, const std::vector<std::string>& transactions);

    /**
     * @brief Generates blocks to a specific address.
     * @param nBlocks The number of blocks to generate.
     * @param address The address to receive the block rewards.
     * @return A Json::Value containing the generated blocks.
     */
    Json::Value generateToAddress(int nBlocks, const std::string& address);

    /**
     * @brief Generates blocks to a specific descriptor.
     * @param nBlocks The number of blocks to generate.
     * @param descriptor The descriptor to receive the block rewards.
     * @return A Json::Value containing the generated blocks.
     */
    Json::Value generateToDescriptor(int nBlocks, const std::string& descriptor);

           // Mining RPCs
    /**
     * @brief Returns a block template for mining.
     * @param templateRequest A JSON object with template parameters (default: empty).
     * @return A Json::Value containing the block template.
     */
    Json::Value getBlockTemplate(const std::string& templateRequest = "");

    /**
     * @brief Returns mining-related information.
     * @return A Json::Value containing mining information.
     */
    Json::Value getMiningInfo();

    /**
     * @brief Returns the estimated network hashes per second.
     * @param nBlocks The number of blocks to consider (default: 120).
     * @param height The block height to estimate from (default: -1 for the chain tip).
     * @return A Json::Value containing the network hashes per second.
     */
    Json::Value getNetworkHashPS(int nBlocks = 120, int height = -1);

    /**
     * @brief Prioritizes a transaction in the mempool.
     * @param txid The transaction ID.
     * @param feeDelta The fee delta to apply.
     * @return A Json::Value containing the result.
     */
    Json::Value prioritiseTransaction(const std::string& txid, double feeDelta);

    /**
     * @brief Submits a block to the network.
     * @param hexData The hex-encoded block data.
     * @param parameters Additional parameters (default: empty).
     * @return A Json::Value containing the result.
     */
    Json::Value submitBlock(const std::string& hexData, const std::string& parameters = "");

    /**
     * @brief Submits a block header to the network.
     * @param hexHeader The hex-encoded block header.
     * @return A Json::Value containing the result.
     */
    Json::Value submitHeader(const std::string& hexHeader);

           // Network RPCs
    /**
     * @brief Adds a node to the connection list.
     * @param node The node to add.
     * @param command The command to execute (e.g., "add", "remove", "onetry").
     * @return A Json::Value containing the result.
     */
    Json::Value addNode(const std::string& node, const std::string& command);

    /**
     * @brief Clears the list of banned nodes.
     * @return A Json::Value containing the result.
     */
    Json::Value clearBanned();

    /**
     * @brief Disconnects a node from the network.
     * @param address The address of the node to disconnect.
     * @return A Json::Value containing the result.
     */
    Json::Value disconnectNode(const std::string& address);

    /**
     * @brief Returns information about added nodes.
     * @param node The node to query (default: empty for all nodes).
     * @return A Json::Value containing the node information.
     */
    Json::Value getAddedNodeInfo(const std::string& node = "");

    /**
     * @brief Returns the number of connections to the network.
     * @return A Json::Value containing the connection count.
     */
    Json::Value getConnectionCount();

    /**
     * @brief Returns network traffic statistics.
     * @return A Json::Value containing the network totals.
     */
    Json::Value getNetTotals();

    /**
     * @brief Returns information about the network.
     * @return A Json::Value containing the network information.
     */
    Json::Value getNetworkInfo();

    /**
     * @brief Returns a list of node addresses.
     * @param count The number of addresses to return (default: 1).
     * @return A Json::Value containing the node addresses.
     */
    Json::Value getNodeAddresses(int count = 1);

    /**
     * @brief Returns information about connected peers.
     * @return A Json::Value containing the peer information.
     */
    Json::Value getPeerInfo();

    /**
     * @brief Returns a list of banned nodes.
     * @return A Json::Value containing the banned nodes.
     */
    Json::Value listBanned();

    /**
     * @brief Pings all connected nodes.
     * @return A Json::Value containing the result.
     */
    Json::Value ping();

    /**
     * @brief Bans or unbans a node.
     * @param subnet The subnet to ban or unban.
     * @param command The command to execute (e.g., "add", "remove").
     * @param banTime The duration of the ban in seconds (default: 0 for permanent).
     * @param absolute If true, the ban time is absolute (default: false).
     * @return A Json::Value containing the result.
     */
    Json::Value setBan(const std::string& subnet, const std::string& command, int banTime = 0, bool absolute = false);

    /**
     * @brief Enables or disables network activity.
     * @param state If true, enables network activity; if false, disables it.
     * @return A Json::Value containing the result.
     */
    Json::Value setNetworkActive(bool state);

           // Rawtransactions RPCs
    /**
     * @brief Analyzes a Partially Signed Bitcoin Transaction (PSBT).
     * @param psbt The PSBT to analyze.
     * @return A Json::Value containing the analysis results.
     */
    Json::Value analyzePsbt(const std::string& psbt);

    /**
     * @brief Combines multiple PSBTs into one.
     * @param psbts A list of PSBTs to combine.
     * @return A Json::Value containing the combined PSBT.
     */
    Json::Value combinePsbt(const std::vector<std::string>& psbts);

    /**
     * @brief Combines multiple raw transactions into one.
     * @param hexStrings A list of hex-encoded raw transactions.
     * @return A Json::Value containing the combined transaction.
     */
    Json::Value combineRawTransaction(const std::vector<std::string>& hexStrings);

    /**
     * @brief Converts a raw transaction to a PSBT.
     * @param hexString The hex-encoded raw transaction.
     * @param permitsigdata If true, allows signature data in the PSBT (default: false).
     * @param iswitness If true, treats the transaction as a witness transaction (default: true).
     * @return A Json::Value containing the PSBT.
     */
    Json::Value convertToPsbt(const std::string& hexString, bool permitsigdata = false, bool iswitness = true);

    /**
     * @brief Creates a PSBT from inputs and outputs.
     * @param inputs A list of transaction inputs.
     * @param outputs A map of outputs with their amounts.
     * @return A Json::Value containing the PSBT.
     */
    Json::Value createPsbt(const std::vector<Json::Value>& inputs, const std::map<std::string, double>& outputs);

    /**
     * @brief Creates a raw transaction from inputs and outputs.
     * @param inputs A list of transaction inputs.
     * @param outputs A map of outputs with their amounts.
     * @return A Json::Value containing the raw transaction.
     */
    Json::Value createRawTransaction(const std::vector<Json::Value>& inputs, const std::map<std::string, double>& outputs);

    /**
     * @brief Decodes a PSBT.
     * @param psbt The PSBT to decode.
     * @return A Json::Value containing the decoded PSBT.
     */
    Json::Value decodePsbt(const std::string& psbt);

    /**
     * @brief Decodes a raw transaction.
     * @param hexString The hex-encoded raw transaction.
     * @param iswitness If true, treats the transaction as a witness transaction (default: true).
     * @return A Json::Value containing the decoded transaction.
     */
    Json::Value decodeRawTransaction(const std::string& hexString, bool iswitness = true);

    /**
     * @brief Decodes a script.
     * @param hexString The hex-encoded script.
     * @return A Json::Value containing the decoded script.
     */
    Json::Value decodeScript(const std::string& hexString);

    /**
     * @brief Finalizes a PSBT.
     * @param psbt The PSBT to finalize.
     * @param extract If true, extracts the final transaction (default: true).
     * @return A Json::Value containing the finalized PSBT.
     */
    Json::Value finalizePsbt(const std::string& psbt, bool extract = true);

    /**
     * @brief Funds a raw transaction.
     * @param hexString The hex-encoded raw transaction.
     * @param options A JSON object with funding options (default: empty).
     * @return A Json::Value containing the funded transaction.
     */
    Json::Value fundRawTransaction(const std::string& hexString, const Json::Value& options = Json::Value());

    /**
     * @brief Returns a raw transaction.
     * @param txid The transaction ID.
     * @param verbose If true, returns full transaction details; otherwise, returns hex-encoded data.
     * @return A Json::Value containing the raw transaction.
     */
    Json::Value getRawTransaction(const std::string& txid, bool verbose = false);

    /**
     * @brief Joins multiple PSBTs into one.
     * @param psbts A list of PSBTs to join.
     * @return A Json::Value containing the joined PSBT.
     */
    Json::Value joinPsbts(const std::vector<std::string>& psbts);

    /**
     * @brief Sends a raw transaction to the network.
     * @param hexString The hex-encoded raw transaction.
     * @param allowhighfees If true, allows high fees (default: false).
     * @return A Json::Value containing the result.
     */
    Json::Value sendRawTransaction(const std::string& hexString, bool allowhighfees = false);

    /**
     * @brief Signs a raw transaction with private keys.
     * @param hexString The hex-encoded raw transaction.
     * @param privKeys A list of private keys to sign with.
     * @param prevTxs A JSON object with previous transaction data (default: empty).
     * @return A Json::Value containing the signed transaction.
     */
    Json::Value signRawTransactionWithKey(const std::string& hexString, const std::vector<std::string>& privKeys, const Json::Value& prevTxs = Json::Value());

    /**
     * @brief Tests if a raw transaction would be accepted into the mempool.
     * @param rawTxns A list of hex-encoded raw transactions.
     * @param allowhighfees If true, allows high fees (default: false).
     * @return A Json::Value containing the test results.
     */
    Json::Value testMempoolAccept(const std::vector<std::string>& rawTxns, bool allowhighfees = false);

    /**
     * @brief Updates a PSBT with UTXO information.
     * @param psbt The PSBT to update.
     * @param descriptors A JSON object with descriptor information (default: empty).
     * @return A Json::Value containing the updated PSBT.
     */
    Json::Value utxoUpdatePsbt(const std::string& psbt, const Json::Value& descriptors = Json::Value());

           // Util RPCs
    /**
     * @brief Creates a multi-signature address.
     * @param nRequired The number of signatures required.
     * @param keys A list of public keys.
     * @return A Json::Value containing the multi-signature address.
     */
    Json::Value createMultiSig(int nRequired, const std::vector<std::string>& keys);

    /**
     * @brief Derives addresses from a descriptor.
     * @param descriptor The descriptor to derive from.
     * @param range A JSON object with derivation range (default: empty).
     * @return A Json::Value containing the derived addresses.
     */
    Json::Value deriveAddresses(const std::string& descriptor, const Json::Value& range = Json::Value());

    /**
     * @brief Estimates a smart fee for a transaction.
     * @param confTarget The confirmation target in blocks.
     * @param estimateMode The estimation mode (default: "CONSERVATIVE").
     * @return A Json::Value containing the estimated fee.
     */
    Json::Value estimateSmartFee(int confTarget, const std::string& estimateMode = "CONSERVATIVE");

    /**
     * @brief Returns information about a descriptor.
     * @param descriptor The descriptor to analyze.
     * @return A Json::Value containing the descriptor information.
     */
    Json::Value getDescriptorInfo(const std::string& descriptor);

    /**
     * @brief Returns information about the indexes.
     * @return A Json::Value containing the index information.
     */
    Json::Value getIndexInfo();

    /**
     * @brief Signs a message with a private key.
     * @param privKey The private key to sign with.
     * @param message The message to sign.
     * @return A Json::Value containing the signature.
     */
    Json::Value signMessageWithPrivKey(const std::string& privKey, const std::string& message);

    /**
     * @brief Validates a Bitcoin address.
     * @param address The address to validate.
     * @return A Json::Value containing the validation result.
     */
    Json::Value validateAddress(const std::string& address);

    /**
     * @brief Verifies a signed message.
     * @param address The address associated with the signature.
     * @param signature The signature to verify.
     * @param message The message to verify.
     * @return A Json::Value containing the verification result.
     */
    Json::Value verifyMessage(const std::string& address, const std::string& signature, const std::string& message);

           // Wallet RPCs
    /**
     * @brief Abandons a transaction.
     * @param txid The transaction ID.
     * @return A Json::Value containing the result.
     */
    Json::Value abandonTransaction(const std::string& txid);

    /**
     * @brief Aborts an ongoing rescan.
     * @return A Json::Value containing the result.
     */
    Json::Value abortRescan();

    /**
     * @brief Adds a multi-signature address to the wallet.
     * @param nRequired The number of signatures required.
     * @param keys A list of public keys.
     * @param label A label for the address (default: empty).
     * @return A Json::Value containing the multi-signature address.
     */
    Json::Value addMultiSigAddress(int nRequired, const std::vector<std::string>& keys, const std::string& label = "");

    /**
     * @brief Backs up the wallet to a file.
     * @param destination The destination file path.
     * @return A Json::Value containing the result.
     */
    Json::Value backupWallet(const std::string& destination);

    /**
     * @brief Bumps the fee of a transaction.
     * @param txid The transaction ID.
     * @param options A JSON object with bump fee options (default: empty).
     * @return A Json::Value containing the result.
     */
    Json::Value bumpFee(const std::string& txid, const Json::Value& options = Json::Value());

    /**
     * @brief Creates a new wallet.
     * @param walletName The name of the wallet.
     * @param disablePrivateKeys If true, disables private keys (default: false).
     * @param blank If true, creates a blank wallet (default: false).
     * @return A Json::Value containing the result.
     */
    Json::Value createWallet(const std::string& walletName, bool disablePrivateKeys = false, bool blank = false);

    /**
     * @brief Dumps the private key for an address.
     * @param address The address to dump.
     * @return A Json::Value containing the private key.
     */
    Json::Value dumpPrivKey(const std::string& address);

    /**
     * @brief Dumps the wallet to a file.
     * @param filename The file to dump to.
     * @return A Json::Value containing the result.
     */
    Json::Value dumpWallet(const std::string& filename);

    /**
     * @brief Encrypts the wallet with a passphrase.
     * @param passphrase The passphrase to encrypt with.
     * @return A Json::Value containing the result.
     */
    Json::Value encryptWallet(const std::string& passphrase);

    /**
     * @brief Returns addresses associated with a label.
     * @param label The label to query.
     * @return A Json::Value containing the addresses.
     */
    Json::Value getAddressesByLabel(const std::string& label);

    /**
     * @brief Returns information about an address.
     * @param address The address to query.
     * @return A Json::Value containing the address information.
     */
    Json::Value getAddressInfo(const std::string& address);

    /**
     * @brief Returns the wallet balance.
     * @param dummy A dummy parameter (default: "*").
     * @param minconf The minimum number of confirmations (default: 0).
     * @param includeWatchonly If true, includes watch-only addresses (default: false).
     * @return A Json::Value containing the balance.
     */
    Json::Value getBalance(const std::string& dummy = "*", int minconf = 0, bool includeWatchonly = false);

    /**
     * @brief Returns the wallet balances.
     * @return A Json::Value containing the balances.
     */
    Json::Value getBalances();

    /**
     * @brief Returns a new address for receiving payments.
     * @param label A label for the address (default: empty).
     * @return A Json::Value containing the new address.
     */
    Json::Value getNewAddress(const std::string& label = "");

    /**
     * @brief Returns a new change address.
     * @param addressType The type of address to generate (default: empty).
     * @return A Json::Value containing the change address.
     */
    Json::Value getRawChangeAddress(const std::string& addressType = "");

    /**
     * @brief Returns the total amount received by an address.
     * @param address The address to query.
     * @param minconf The minimum number of confirmations (default: 1).
     * @return A Json::Value containing the total amount received.
     */
    Json::Value getReceivedByAddress(const std::string& address, int minconf = 1);

    /**
     * @brief Returns the total amount received by a label.
     * @param label The label to query.
     * @param minconf The minimum number of confirmations (default: 1).
     * @return A Json::Value containing the total amount received.
     */
    Json::Value getReceivedByLabel(const std::string& label, int minconf = 1);

    /**
     * @brief Returns information about a transaction.
     * @param txid The transaction ID.
     * @param includeWatchonly If true, includes watch-only addresses (default: false).
     * @return A Json::Value containing the transaction information.
     */
    Json::Value getTransaction(const std::string& txid, bool includeWatchonly = false);

    /**
     * @brief Returns the unconfirmed wallet balance.
     * @return A Json::Value containing the unconfirmed balance.
     */
    Json::Value getUnconfirmedBalance();

    /**
     * @brief Returns information about the wallet.
     * @return A Json::Value containing the wallet information.
     */
    Json::Value getWalletInfo();

    /**
     * @brief Imports an address into the wallet.
     * @param address The address to import.
     * @param label A label for the address (default: empty).
     * @param rescan If true, rescans the blockchain (default: true).
     * @return A Json::Value containing the result.
     */
    Json::Value importAddress(const std::string& address, const std::string& label = "", bool rescan = true);

    /**
     * @brief Imports descriptors into the wallet.
     * @param requests A JSON object with descriptor requests.
     * @return A Json::Value containing the result.
     */
    Json::Value importDescriptors(const Json::Value& requests);

    /**
     * @brief Imports multiple addresses or scripts into the wallet.
     * @param requests A JSON object with import requests.
     * @param options A JSON object with import options (default: empty).
     * @return A Json::Value containing the result.
     */
    Json::Value importMulti(const Json::Value& requests, const Json::Value& options = Json::Value());

    /**
     * @brief Imports a private key into the wallet.
     * @param privKey The private key to import.
     * @param label A label for the address (default: empty).
     * @param rescan If true, rescans the blockchain (default: true).
     * @return A Json::Value containing the result.
     */
    Json::Value importPrivKey(const std::string& privKey, const std::string& label = "", bool rescan = true);

    /**
     * @brief Imports pruned funds into the wallet.
     * @param rawTransaction The raw transaction to import.
     * @param txOutProof The transaction output proof.
     * @return A Json::Value containing the result.
     */
    Json::Value importPrunedFunds(const std::string& rawTransaction, const std::string& txOutProof);

    /**
     * @brief Imports a public key into the wallet.
     * @param pubKey The public key to import.
     * @param label A label for the address (default: empty).
     * @param rescan If true, rescans the blockchain (default: true).
     * @return A Json::Value containing the result.
     */
    Json::Value importPubKey(const std::string& pubKey, const std::string& label = "", bool rescan = true);

    /**
     * @brief Imports a wallet from a file.
     * @param filename The file to import.
     * @return A Json::Value containing the result.
     */
    Json::Value importWallet(const std::string& filename);

    /**
     * @brief Refills the key pool.
     * @param newSize The new size of the key pool (default: 100).
     * @return A Json::Value containing the result.
     */
    Json::Value keyPoolRefill(int newSize = 100);

    /**
     * @brief Returns a list of address groupings.
     * @return A Json::Value containing the address groupings.
     */
    Json::Value listAddressGroupings();

    /**
     * @brief Returns a list of labels in the wallet.
     * @return A Json::Value containing the labels.
     */
    Json::Value listLabels();

    /**
     * @brief Returns a list of locked unspent transaction outputs.
     * @return A Json::Value containing the locked UTXOs.
     */
    Json::Value listLockUnspent();

    /**
     * @brief Returns a list of received payments by address.
     * @param minconf The minimum number of confirmations (default: 1).
     * @param includeEmpty If true, includes addresses with no payments (default: false).
     * @param includeWatchonly If true, includes watch-only addresses (default: false).
     * @return A Json::Value containing the received payments.
     */
    Json::Value listReceivedByAddress(int minconf = 1, bool includeEmpty = false, bool includeWatchonly = false);

    /**
     * @brief Returns a list of received payments by label.
     * @param minconf The minimum number of confirmations (default: 1).
     * @param includeEmpty If true, includes labels with no payments (default: false).
     * @param includeWatchonly If true, includes watch-only addresses (default: false).
     * @return A Json::Value containing the received payments.
     */
    Json::Value listReceivedByLabel(int minconf = 1, bool includeEmpty = false, bool includeWatchonly = false);

    /**
     * @brief Returns a list of transactions since a specific block.
     * @param blockHash The block hash to start from (default: empty for the chain tip).
     * @param targetConfirmations The number of confirmations to target (default: 1).
     * @param includeWatchonly If true, includes watch-only addresses (default: false).
     * @return A Json::Value containing the transactions.
     */
    Json::Value listSinceBlock(const std::string& blockHash = "", int targetConfirmations = 1, bool includeWatchonly = false);

    /**
     * @brief Returns a list of transactions in the wallet.
     * @param label The label to filter by (default: "*" for all labels).
     * @param count The number of transactions to return (default: 10).
     * @param skip The number of transactions to skip (default: 0).
     * @param includeWatchonly If true, includes watch-only addresses (default: false).
     * @return A Json::Value containing the transactions.
     */
    Json::Value listTransactions(const std::string& label = "*", int count = 10, int skip = 0, bool includeWatchonly = false);

    /**
     * @brief Returns a list of unspent transaction outputs.
     * @param minconf The minimum number of confirmations (default: 1).
     * @param maxconf The maximum number of confirmations (default: 9999999).
     * @param addresses A list of addresses to filter by (default: empty for all addresses).
     * @param includeUnsafe If true, includes unsafe outputs (default: true).
     * @return A Json::Value containing the unspent outputs.
     */
    Json::Value listUnspent(int minconf = 1, int maxconf = 9999999, const std::vector<std::string>& addresses = {}, bool includeUnsafe = true);

    /**
     * @brief Returns a list of wallet directories.
     * @return A Json::Value containing the wallet directories.
     */
    Json::Value listWalletDir();

    /**
     * @brief Returns a list of loaded wallets.
     * @return A Json::Value containing the loaded wallets.
     */
    Json::Value listWallets();

    /**
     * @brief Loads a wallet.
     * @param walletName The name of the wallet to load.
     * @return A Json::Value containing the result.
     */
    Json::Value loadWallet(const std::string& walletName);

    /**
     * @brief Locks or unlocks unspent transaction outputs.
     * @param unlock If true, unlocks the outputs; if false, locks them.
     * @param transactions A JSON object with transactions to lock or unlock (default: empty).
     * @return A Json::Value containing the result.
     */
    Json::Value lockUnspent(bool unlock, const Json::Value& transactions = Json::Value());

    /**
     * @brief Bumps the fee of a PSBT.
     * @param txid The transaction ID.
     * @param options A JSON object with bump fee options (default: empty).
     * @return A Json::Value containing the result.
     */
    Json::Value psbtBumpFee(const std::string& txid, const Json::Value& options = Json::Value());

    /**
     * @brief Removes pruned funds from the wallet.
     * @param txid The transaction ID.
     * @return A Json::Value containing the result.
     */
    Json::Value removePrunedFunds(const std::string& txid);

    /**
     * @brief Rescans the blockchain for wallet transactions.
     * @param startHeight The height to start rescanning from (default: 0).
     * @param stopHeight The height to stop rescanning at (default: -1 for the chain tip).
     * @return A Json::Value containing the result.
     */
    Json::Value rescanBlockchain(int startHeight = 0, int stopHeight = -1);

    /**
     * @brief Sends funds to multiple outputs.
     * @param outputs A JSON object with outputs and amounts.
     * @param confTarget The confirmation target in blocks (default: 6).
     * @param estimateMode The estimation mode (default: "UNSET").
     * @param replaceable If true, allows the transaction to be replaced (default: false).
     * @return A Json::Value containing the result.
     */
    Json::Value send(const Json::Value& outputs, int confTarget = 6, const std::string& estimateMode = "UNSET", bool replaceable = false);

    /**
     * @brief Sends funds to multiple addresses.
     * @param dummy A dummy parameter (default: "*").
     * @param amounts A map of addresses and amounts.
     * @param minconf The minimum number of confirmations (default: 1).
     * @param comment A comment for the transaction (default: empty).
     * @param subtractFeeFrom A list of addresses to subtract fees from (default: empty).
     * @return A Json::Value containing the result.
     */
    Json::Value sendMany(const std::string& dummy, const std::map<std::string, double>& amounts, int minconf = 1, const std::string& comment = "", const std::vector<std::string>& subtractFeeFrom = {});

    /**
     * @brief Sends funds to a single address.
     * @param address The address to send to.
     * @param amount The amount to send.
     * @param comment A comment for the transaction (default: empty).
     * @param commentTo A comment for the recipient (default: empty).
     * @param subtractFeeFromAmount If true, subtracts the fee from the amount (default: false).
     * @return A Json::Value containing the result.
     */
    Json::Value sendToAddress(const std::string& address, double amount, const std::string& comment = "", const std::string& commentTo = "", bool subtractFeeFromAmount = false);

    /**
     * @brief Sets the HD seed for the wallet.
     * @param seed The seed to set (default: empty for a new seed).
     * @param rescan If true, rescans the blockchain (default: true).
     * @return A Json::Value containing the result.
     */
    Json::Value setHdSeed(const std::string& seed = "", bool rescan = true);

    /**
     * @brief Sets a label for an address.
     * @param address The address to label.
     * @param label The label to set.
     * @return A Json::Value containing the result.
     */
    Json::Value setLabel(const std::string& address, const std::string& label);

    /**
     * @brief Sets the transaction fee for the wallet.
     * @param amount The fee amount to set.
     * @return A Json::Value containing the result.
     */
    Json::Value setTxFee(double amount);

    /**
     * @brief Sets a wallet flag.
     * @param flag The flag to set.
     * @param value The value to set the flag to.
     * @return A Json::Value containing the result.
     */
    Json::Value setWalletFlag(const std::string& flag, bool value);

    /**
     * @brief Signs a message with an address.
     * @param address The address to sign with.
     * @param message The message to sign.
     * @return A Json::Value containing the signature.
     */
    Json::Value signMessage(const std::string& address, const std::string& message);

    /**
     * @brief Signs a raw transaction with the wallet.
     * @param hexString The hex-encoded raw transaction.
     * @param prevTxs A JSON object with previous transaction data (default: empty).
     * @return A Json::Value containing the signed transaction.
     */
    Json::Value signRawTransactionWithWallet(const std::string& hexString, const Json::Value& prevTxs = Json::Value());

    /**
     * @brief Unloads a wallet.
     * @param walletName The name of the wallet to unload (default: empty for the default wallet).
     * @return A Json::Value containing the result.
     */
    Json::Value unloadWallet(const std::string& walletName = "");

    /**
     * @brief Upgrades the wallet to the latest version.
     * @param walletName The name of the wallet to upgrade (default: empty for the default wallet).
     * @return A Json::Value containing the result.
     */
    Json::Value upgradeWallet(const std::string& walletName = "");

    /**
     * @brief Creates a funded PSBT.
     * @param inputs A list of transaction inputs.
     * @param outputs A map of outputs with their amounts.
     * @param locktime The locktime for the transaction (default: 0).
     * @param options A JSON object with funding options (default: empty).
     * @return A Json::Value containing the funded PSBT.
     */
    Json::Value walletCreateFundedPsbt(const std::vector<Json::Value>& inputs, const std::map<std::string, double>& outputs, int locktime = 0, const Json::Value& options = Json::Value());

    /**
     * @brief Locks the wallet.
     * @return A Json::Value containing the result.
     */
    Json::Value walletLock();

    /**
     * @brief Unlocks the wallet with a passphrase.
     * @param passphrase The passphrase to unlock with.
     * @param timeout The duration in seconds to keep the wallet unlocked (default: 0 for indefinite).
     * @return A Json::Value containing the result.
     */
    Json::Value walletPassphrase(const std::string& passphrase, int timeout);

    /**
     * @brief Changes the wallet passphrase.
     * @param oldPassphrase The current passphrase.
     * @param newPassphrase The new passphrase.
     * @return A Json::Value containing the result.
     */
    Json::Value walletPassphraseChange(const std::string& oldPassphrase, const std::string& newPassphrase);

    /**
     * @brief Processes a PSBT with the wallet.
     * @param psbt The PSBT to process.
     * @param sign If true, signs the PSBT (default: true).
     * @param sighashType The signature hash type (default: true).
     * @param bip32derivs If true, includes BIP32 derivation paths (default: true).
     * @return A Json::Value containing the processed PSBT.
     */
    Json::Value walletProcessPsbt(const std::string& psbt, bool sign = true, bool sighashType = true, bool bip32derivs = true);
};

#endif // BITCOINCLIENT_HPP
