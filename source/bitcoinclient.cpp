#include "bitcoinclient.hpp"

BitcoinClient::BitcoinClient(const std::string& user, const std::string& password, const std::string& url)
    : rpcUser(user), rpcPassword(password), rpcUrl(url) {}

std::string BitcoinClient::buildRpcRequest(const std::string& method, const Json::Value& params) {
    Json::Value request;
    request["jsonrpc"] = "1.0";
    request["id"] = "Genyleap-Bitcoin-RPC";
    request["method"] = method;
    request["params"] = params;

    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, request);
}

Json::Value BitcoinClient::parseRpcResponse(const std::string& response) {
    Json::Value jsonResponse;
    Json::CharReaderBuilder reader;
    std::string errors;
    std::istringstream responseStream(response);

    if (!Json::parseFromStream(reader, responseStream, &jsonResponse, &errors)) {
        Logger::formattedError("Failed to parse JSON response: {}", errors);
        return Json::Value();
    }

    if (!jsonResponse["error"].isNull()) {
        Logger::formattedError("RPC error: {}", Json::writeString(Json::StreamWriterBuilder(), jsonResponse["error"]));
        return Json::Value();
    }

    return jsonResponse["result"];
}

Json::Value BitcoinClient::sendRequest(const std::string& method, const Json::Value& params) {
    std::string rpcRequest = buildRpcRequest(method, params);
    Logger::formattedInfo("Sending RPC request: {}", rpcRequest);

    std::map<std::string, std::string> headers = {{"Content-Type", "application/json"}};
    std::string response;

    if (!network.sendPostRequest(rpcUrl, rpcRequest, response, rpcUser, rpcPassword, headers, true)) {
        Logger::error("Failed to send RPC request");
        return Json::Value();
    }

    Logger::json(response);
    return parseRpcResponse(response);
}

Json::Value BitcoinClient::getBestBlockHash() {
    return sendRequest("getbestblockhash");
}

Json::Value BitcoinClient::getBlock(const std::string& blockHash, bool verbose) {
    return sendRequest("getblock", buildParams(blockHash, verbose ? 2 : 1));
}

Json::Value BitcoinClient::getBlockchainInfo() {
    return sendRequest("getblockchaininfo");
}

Json::Value BitcoinClient::getBlockCount() {
    return sendRequest("getblockcount");
}

Json::Value BitcoinClient::getBlockFilter(const std::string& blockHash, const std::string& filterType) {
    return sendRequest("getblockfilter", buildParams(blockHash, filterType));
}

Json::Value BitcoinClient::getBlockHash(int height) {
    return sendRequest("getblockhash", buildParams(height));
}

Json::Value BitcoinClient::getBlockHeader(const std::string& blockHash, bool verbose) {
    return sendRequest("getblockheader", buildParams(blockHash, verbose));
}

Json::Value BitcoinClient::getBlockStats(const std::string& blockHash, const std::vector<std::string>& stats) {
    Json::Value statsArray;
    for (const auto& stat : stats) statsArray.append(stat);
    return sendRequest("getblockstats", buildParams(blockHash, statsArray));
}

Json::Value BitcoinClient::getChainTips() {
    return sendRequest("getchaintips");
}

Json::Value BitcoinClient::getChainTxStats(int nBlocks, const std::string& blockHash) {
    Json::Value params = buildParams(nBlocks);
    if (!blockHash.empty()) params.append(blockHash);
    return sendRequest("getchaintxstats", params);
}

Json::Value BitcoinClient::getDifficulty() {
    return sendRequest("getdifficulty");
}

Json::Value BitcoinClient::getMempoolAncestors(const std::string& txid, bool verbose) {
    return sendRequest("getmempoolancestors", buildParams(txid, verbose));
}

Json::Value BitcoinClient::getMempoolDescendants(const std::string& txid, bool verbose) {
    return sendRequest("getmempooldescendants", buildParams(txid, verbose));
}

Json::Value BitcoinClient::getMempoolEntry(const std::string& txid) {
    return sendRequest("getmempoolentry", buildParams(txid));
}

Json::Value BitcoinClient::getMempoolInfo() {
    return sendRequest("getmempoolinfo");
}

Json::Value BitcoinClient::getRawMempool(bool verbose) {
    return sendRequest("getrawmempool", buildParams(verbose));
}

Json::Value BitcoinClient::getTxOut(const std::string& txid, int n, bool includeMempool) {
    return sendRequest("gettxout", buildParams(txid, n, includeMempool));
}

Json::Value BitcoinClient::getTxOutProof(const std::vector<std::string>& txids, const std::string& blockHash) {
    Json::Value txidsArray;
    for (const auto& txid : txids) txidsArray.append(txid);
    Json::Value params = buildParams(txidsArray);
    if (!blockHash.empty()) params.append(blockHash);
    return sendRequest("gettxoutproof", params);
}

Json::Value BitcoinClient::getTxOutSetInfo() {
    return sendRequest("gettxoutsetinfo");
}

Json::Value BitcoinClient::preciousBlock(const std::string& blockHash) {
    return sendRequest("preciousblock", buildParams(blockHash));
}

Json::Value BitcoinClient::pruneBlockchain(int height) {
    return sendRequest("pruneblockchain", buildParams(height));
}

Json::Value BitcoinClient::saveMempool() {
    return sendRequest("savemempool");
}

Json::Value BitcoinClient::scanTxOutSet(const std::vector<std::string>& descriptors) {
    Json::Value descriptorsArray;
    for (const auto& desc : descriptors) descriptorsArray.append(desc);
    return sendRequest("scantxoutset", buildParams(descriptorsArray));
}

Json::Value BitcoinClient::verifyChain(int checkLevel, int nBlocks) {
    return sendRequest("verifychain", buildParams(checkLevel, nBlocks));
}

Json::Value BitcoinClient::verifyTxOutProof(const std::string& proof) {
    return sendRequest("verifytxoutproof", buildParams(proof));
}

Json::Value BitcoinClient::getMemoryInfo() {
    return sendRequest("getmemoryinfo");
}

Json::Value BitcoinClient::getRpcInfo() {
    return sendRequest("getrpcinfo");
}

Json::Value BitcoinClient::help(const std::string& command) {
    return sendRequest("help", command.empty() ? Json::Value() : buildParams(command));
}

Json::Value BitcoinClient::logging(const std::vector<std::string>& include, const std::vector<std::string>& exclude) {
    Json::Value includeArray, excludeArray;
    for (const auto& inc : include) includeArray.append(inc);
    for (const auto& exc : exclude) excludeArray.append(exc);
    return sendRequest("logging", buildParams(includeArray, excludeArray));
}

Json::Value BitcoinClient::stop() {
    return sendRequest("stop");
}

Json::Value BitcoinClient::uptime() {
    return sendRequest("uptime");
}

Json::Value BitcoinClient::generateBlock(const std::string& outputAddress, const std::vector<std::string>& transactions) {
    Json::Value txArray;
    for (const auto& tx : transactions) txArray.append(tx);
    return sendRequest("generateblock", buildParams(outputAddress, txArray));
}

Json::Value BitcoinClient::generateToAddress(int nBlocks, const std::string& address) {
    return sendRequest("generatetoaddress", buildParams(nBlocks, address));
}

Json::Value BitcoinClient::generateToDescriptor(int nBlocks, const std::string& descriptor) {
    return sendRequest("generatetodescriptor", buildParams(nBlocks, descriptor));
}

Json::Value BitcoinClient::getBlockTemplate(const std::string& templateRequest) {
    return sendRequest("getblocktemplate", templateRequest.empty() ? Json::Value() : buildParams(templateRequest));
}

Json::Value BitcoinClient::getMiningInfo() {
    return sendRequest("getmininginfo");
}

Json::Value BitcoinClient::getNetworkHashPS(int nBlocks, int height) {
    return sendRequest("getnetworkhashps", buildParams(nBlocks, height));
}

Json::Value BitcoinClient::prioritiseTransaction(const std::string& txid, double feeDelta) {
    return sendRequest("prioritisetransaction", buildParams(txid, feeDelta));
}

Json::Value BitcoinClient::submitBlock(const std::string& hexData, const std::string& parameters) {
    return parameters.empty() ? sendRequest("submitblock", buildParams(hexData))
                              : sendRequest("submitblock", buildParams(hexData, parameters));
}

Json::Value BitcoinClient::submitHeader(const std::string& hexHeader) {
    return sendRequest("submitheader", buildParams(hexHeader));
}

Json::Value BitcoinClient::addNode(const std::string& node, const std::string& command) {
    return sendRequest("addnode", buildParams(node, command));
}

Json::Value BitcoinClient::clearBanned() {
    return sendRequest("clearbanned");
}

Json::Value BitcoinClient::disconnectNode(const std::string& address) {
    return sendRequest("disconnectnode", buildParams(address));
}

Json::Value BitcoinClient::getAddedNodeInfo(const std::string& node) {
    return sendRequest("getaddednodeinfo", node.empty() ? Json::Value() : buildParams(node));
}

Json::Value BitcoinClient::getConnectionCount() {
    return sendRequest("getconnectioncount");
}

Json::Value BitcoinClient::getNetTotals() {
    return sendRequest("getnettotals");
}

Json::Value BitcoinClient::getNetworkInfo() {
    return sendRequest("getnetworkinfo");
}

Json::Value BitcoinClient::getNodeAddresses(int count) {
    return sendRequest("getnodeaddresses", buildParams(count));
}

Json::Value BitcoinClient::getPeerInfo() {
    return sendRequest("getpeerinfo");
}

Json::Value BitcoinClient::listBanned() {
    return sendRequest("listbanned");
}

Json::Value BitcoinClient::ping() {
    return sendRequest("ping");
}

Json::Value BitcoinClient::setBan(const std::string& subnet, const std::string& command, int banTime, bool absolute) {
    return sendRequest("setban", buildParams(subnet, command, banTime, absolute));
}

Json::Value BitcoinClient::setNetworkActive(bool state) {
    return sendRequest("setnetworkactive", buildParams(state));
}

Json::Value BitcoinClient::analyzePsbt(const std::string& psbt) {
    return sendRequest("analyzepsbt", buildParams(psbt));
}

Json::Value BitcoinClient::combinePsbt(const std::vector<std::string>& psbts) {
    Json::Value psbtsArray;
    for (const auto& psbt : psbts) psbtsArray.append(psbt);
    return sendRequest("combinepsbt", buildParams(psbtsArray));
}

Json::Value BitcoinClient::combineRawTransaction(const std::vector<std::string>& hexStrings) {
    Json::Value hexArray;
    for (const auto& hex : hexStrings) hexArray.append(hex);
    return sendRequest("combinerawtransaction", buildParams(hexArray));
}

Json::Value BitcoinClient::convertToPsbt(const std::string& hexString, bool permitsigdata, bool iswitness) {
    return sendRequest("converttopsbt", buildParams(hexString, permitsigdata, iswitness));
}

Json::Value BitcoinClient::createPsbt(const std::vector<Json::Value>& inputs, const std::map<std::string, double>& outputs) {
    Json::Value inputsArray;
    for (const auto& input : inputs) inputsArray.append(input);

    Json::Value outputsObj;
    for (const auto& [addr, amount] : outputs) outputsObj[addr] = amount;

    return sendRequest("createpsbt", buildParams(inputsArray, outputsObj));
}

Json::Value BitcoinClient::createRawTransaction(const std::vector<Json::Value>& inputs, const std::map<std::string, double>& outputs) {
    Json::Value inputsArray;
    for (const auto& input : inputs) inputsArray.append(input);

    Json::Value outputsObj;
    for (const auto& [addr, amount] : outputs) outputsObj[addr] = amount;

    return sendRequest("createrawtransaction", buildParams(inputsArray, outputsObj));
}

Json::Value BitcoinClient::decodePsbt(const std::string& psbt) {
    return sendRequest("decodepsbt", buildParams(psbt));
}

Json::Value BitcoinClient::decodeRawTransaction(const std::string& hexString, bool iswitness) {
    return sendRequest("decoderawtransaction", buildParams(hexString, iswitness));
}

Json::Value BitcoinClient::decodeScript(const std::string& hexString) {
    return sendRequest("decodescript", buildParams(hexString));
}

Json::Value BitcoinClient::finalizePsbt(const std::string& psbt, bool extract) {
    return sendRequest("finalizepsbt", buildParams(psbt, extract));
}

Json::Value BitcoinClient::fundRawTransaction(const std::string& hexString, const Json::Value& options) {
    Json::Value params = buildParams(hexString);
    if (!options.isNull()) params.append(options);
    return sendRequest("fundrawtransaction", params);
}

Json::Value BitcoinClient::getRawTransaction(const std::string& txid, bool verbose) {
    return sendRequest("getrawtransaction", buildParams(txid, verbose));
}

Json::Value BitcoinClient::joinPsbts(const std::vector<std::string>& psbts) {
    Json::Value psbtsArray;
    for (const auto& psbt : psbts) psbtsArray.append(psbt);
    return sendRequest("joinpsbts", buildParams(psbtsArray));
}

Json::Value BitcoinClient::sendRawTransaction(const std::string& hexString, bool allowhighfees) {
    return sendRequest("sendrawtransaction", buildParams(hexString, allowhighfees));
}

Json::Value BitcoinClient::signRawTransactionWithKey(const std::string& hexString, const std::vector<std::string>& privKeys, const Json::Value& prevTxs) {
    Json::Value keysArray;
    for (const auto& key : privKeys) keysArray.append(key);
    Json::Value params = buildParams(hexString, keysArray);
    if (!prevTxs.isNull()) params.append(prevTxs);
    return sendRequest("signrawtransactionwithkey", params);
}

Json::Value BitcoinClient::testMempoolAccept(const std::vector<std::string>& rawTxns, bool allowhighfees) {
    Json::Value txnArray;
    for (const auto& txn : rawTxns) txnArray.append(txn);
    return sendRequest("testmempoolaccept", buildParams(txnArray, allowhighfees));
}

Json::Value BitcoinClient::utxoUpdatePsbt(const std::string& psbt, const Json::Value& descriptors) {
    Json::Value params = buildParams(psbt);
    if (!descriptors.isNull()) params.append(descriptors);
    return sendRequest("utxoupdatepsbt", params);
}

Json::Value BitcoinClient::createMultiSig(int nRequired, const std::vector<std::string>& keys) {
    Json::Value keysArray;
    for (const auto& key : keys) keysArray.append(key);
    return sendRequest("createmultisig", buildParams(nRequired, keysArray));
}

Json::Value BitcoinClient::deriveAddresses(const std::string& descriptor, const Json::Value& range) {
    Json::Value params = buildParams(descriptor);
    if (!range.isNull()) params.append(range);
    return sendRequest("deriveaddresses", params);
}

Json::Value BitcoinClient::estimateSmartFee(int confTarget, const std::string& estimateMode) {
    return sendRequest("estimatesmartfee", buildParams(confTarget, estimateMode));
}

Json::Value BitcoinClient::getDescriptorInfo(const std::string& descriptor) {
    return sendRequest("getdescriptorinfo", buildParams(descriptor));
}

Json::Value BitcoinClient::getIndexInfo() {
    return sendRequest("getindexinfo");
}

Json::Value BitcoinClient::signMessageWithPrivKey(const std::string& privKey, const std::string& message) {
    return sendRequest("signmessagewithprivkey", buildParams(privKey, message));
}

Json::Value BitcoinClient::validateAddress(const std::string& address) {
    return sendRequest("validateaddress", buildParams(address));
}

Json::Value BitcoinClient::verifyMessage(const std::string& address, const std::string& signature, const std::string& message) {
    return sendRequest("verifymessage", buildParams(address, signature, message));
}

Json::Value BitcoinClient::abandonTransaction(const std::string& txid) {
    return sendRequest("abandontransaction", buildParams(txid));
}

Json::Value BitcoinClient::abortRescan() {
    return sendRequest("abortrescan");
}

Json::Value BitcoinClient::addMultiSigAddress(int nRequired, const std::vector<std::string>& keys, const std::string& label) {
    Json::Value keysArray;
    for (const auto& key : keys) keysArray.append(key);
    Json::Value params = buildParams(nRequired, keysArray);
    if (!label.empty()) params.append(label);
    return sendRequest("addmultisigaddress", params);
}

Json::Value BitcoinClient::backupWallet(const std::string& destination) {
    return sendRequest("backupwallet", buildParams(destination));
}

Json::Value BitcoinClient::bumpFee(const std::string& txid, const Json::Value& options) {
    Json::Value params = buildParams(txid);
    if (!options.isNull()) params.append(options);
    return sendRequest("bumpfee", params);
}

Json::Value BitcoinClient::createWallet(const std::string& walletName, bool disablePrivateKeys, bool blank) {
    return sendRequest("createwallet", buildParams(walletName, disablePrivateKeys, blank));
}

Json::Value BitcoinClient::dumpPrivKey(const std::string& address) {
    return sendRequest("dumpprivkey", buildParams(address));
}

Json::Value BitcoinClient::dumpWallet(const std::string& filename) {
    return sendRequest("dumpwallet", buildParams(filename));
}

Json::Value BitcoinClient::encryptWallet(const std::string& passphrase) {
    return sendRequest("encryptwallet", buildParams(passphrase));
}

Json::Value BitcoinClient::getAddressesByLabel(const std::string& label) {
    return sendRequest("getaddressesbylabel", buildParams(label));
}

Json::Value BitcoinClient::getAddressInfo(const std::string& address) {
    return sendRequest("getaddressinfo", buildParams(address));
}

Json::Value BitcoinClient::getBalance(const std::string& dummy, int minconf, bool includeWatchonly) {
    return sendRequest("getbalance", buildParams(dummy, minconf, includeWatchonly));
}

Json::Value BitcoinClient::getBalances() {
    return sendRequest("getbalances");
}

Json::Value BitcoinClient::getNewAddress(const std::string& label) {
    return sendRequest("getnewaddress", label.empty() ? Json::Value() : buildParams(label));
}

Json::Value BitcoinClient::getRawChangeAddress(const std::string& addressType) {
    return sendRequest("getrawchangeaddress", addressType.empty() ? Json::Value() : buildParams(addressType));
}

Json::Value BitcoinClient::getReceivedByAddress(const std::string& address, int minconf) {
    return sendRequest("getreceivedbyaddress", buildParams(address, minconf));
}

Json::Value BitcoinClient::getReceivedByLabel(const std::string& label, int minconf) {
    return sendRequest("getreceivedbylabel", buildParams(label, minconf));
}

Json::Value BitcoinClient::getTransaction(const std::string& txid, bool includeWatchonly) {
    return sendRequest("gettransaction", buildParams(txid, includeWatchonly));
}

Json::Value BitcoinClient::getUnconfirmedBalance() {
    return sendRequest("getunconfirmedbalance");
}

Json::Value BitcoinClient::getWalletInfo() {
    return sendRequest("getwalletinfo");
}

Json::Value BitcoinClient::importAddress(const std::string& address, const std::string& label, bool rescan) {
    Json::Value params = buildParams(address);
    if (!label.empty()) params.append(label);
    params.append(rescan);
    return sendRequest("importaddress", params);
}

Json::Value BitcoinClient::importDescriptors(const Json::Value& requests) {
    return sendRequest("importdescriptors", buildParams(requests));
}

Json::Value BitcoinClient::importMulti(const Json::Value& requests, const Json::Value& options) {
    Json::Value params = buildParams(requests);
    if (!options.isNull()) params.append(options);
    return sendRequest("importmulti", params);
}

Json::Value BitcoinClient::importPrivKey(const std::string& privKey, const std::string& label, bool rescan) {
    Json::Value params = buildParams(privKey);
    if (!label.empty()) params.append(label);
    params.append(rescan);
    return sendRequest("importprivkey", params);
}

Json::Value BitcoinClient::importPrunedFunds(const std::string& rawTransaction, const std::string& txOutProof) {
    return sendRequest("importprunedfunds", buildParams(rawTransaction, txOutProof));
}

Json::Value BitcoinClient::importPubKey(const std::string& pubKey, const std::string& label, bool rescan) {
    Json::Value params = buildParams(pubKey);
    if (!label.empty()) params.append(label);
    params.append(rescan);
    return sendRequest("importpubkey", params);
}

Json::Value BitcoinClient::importWallet(const std::string& filename) {
    return sendRequest("importwallet", buildParams(filename));
}

Json::Value BitcoinClient::keyPoolRefill(int newSize) {
    return sendRequest("keypoolrefill", buildParams(newSize));
}

Json::Value BitcoinClient::listAddressGroupings() {
    return sendRequest("listaddressgroupings");
}

Json::Value BitcoinClient::listLabels() {
    return sendRequest("listlabels");
}

Json::Value BitcoinClient::listLockUnspent() {
    return sendRequest("listlockunspent");
}

Json::Value BitcoinClient::listReceivedByAddress(int minconf, bool includeEmpty, bool includeWatchonly) {
    return sendRequest("listreceivedbyaddress", buildParams(minconf, includeEmpty, includeWatchonly));
}

Json::Value BitcoinClient::listReceivedByLabel(int minconf, bool includeEmpty, bool includeWatchonly) {
    return sendRequest("listreceivedbylabel", buildParams(minconf, includeEmpty, includeWatchonly));
}

Json::Value BitcoinClient::listSinceBlock(const std::string& blockHash, int targetConfirmations, bool includeWatchonly) {
    Json::Value params;
    if (!blockHash.empty()) params.append(blockHash);
    params.append(targetConfirmations);
    params.append(includeWatchonly);
    return sendRequest("listsinceblock", params);
}

Json::Value BitcoinClient::listTransactions(const std::string& label, int count, int skip, bool includeWatchonly) {
    Json::Value params;
    if (!label.empty()) params.append(label);
    params.append(count);
    params.append(skip);
    params.append(includeWatchonly);
    return sendRequest("listtransactions", params);
}

Json::Value BitcoinClient::listUnspent(int minconf, int maxconf, const std::vector<std::string>& addresses, bool includeUnsafe) {
    Json::Value addrArray;
    for (const auto& addr : addresses) addrArray.append(addr);
    return sendRequest("listunspent", buildParams(minconf, maxconf, addrArray, includeUnsafe));
}

Json::Value BitcoinClient::listWalletDir() {
    return sendRequest("listwalletdir");
}

Json::Value BitcoinClient::listWallets() {
    return sendRequest("listwallets");
}

Json::Value BitcoinClient::loadWallet(const std::string& walletName) {
    return sendRequest("loadwallet", buildParams(walletName));
}

Json::Value BitcoinClient::lockUnspent(bool unlock, const Json::Value& transactions) {
    Json::Value params = buildParams(unlock);
    if (!transactions.isNull()) params.append(transactions);
    return sendRequest("lockunspent", params);
}

Json::Value BitcoinClient::psbtBumpFee(const std::string& txid, const Json::Value& options) {
    Json::Value params = buildParams(txid);
    if (!options.isNull()) params.append(options);
    return sendRequest("psbtbumpfee", params);
}

Json::Value BitcoinClient::removePrunedFunds(const std::string& txid) {
    return sendRequest("removeprunedfunds", buildParams(txid));
}

Json::Value BitcoinClient::rescanBlockchain(int startHeight, int stopHeight) {
    Json::Value params;
    if (startHeight > 0) params.append(startHeight);
    if (stopHeight > 0) params.append(stopHeight);
    return sendRequest("rescanblockchain", params);
}

Json::Value BitcoinClient::send(const Json::Value& outputs, int confTarget, const std::string& estimateMode, bool replaceable) {
    return sendRequest("send", buildParams(outputs, confTarget, estimateMode, replaceable));
}

Json::Value BitcoinClient::sendMany(const std::string& dummy, const std::map<std::string, double>& amounts, int minconf, const std::string& comment, const std::vector<std::string>& subtractFeeFrom) {
    Json::Value amountsObj;
    for (const auto& [addr, amt] : amounts) amountsObj[addr] = amt;

    Json::Value params = buildParams(dummy, amountsObj, minconf);
    if (!comment.empty()) params.append(comment);

    Json::Value feeArray;
    for (const auto& addr : subtractFeeFrom) feeArray.append(addr);
    params.append(feeArray);

    return sendRequest("sendmany", params);
}

Json::Value BitcoinClient::sendToAddress(const std::string& address, double amount, const std::string& comment, const std::string& commentTo, bool subtractFeeFromAmount) {
    Json::Value params = buildParams(address, amount);
    if (!comment.empty()) params.append(comment);
    if (!commentTo.empty()) params.append(commentTo);
    params.append(subtractFeeFromAmount);
    return sendRequest("sendtoaddress", params);
}

Json::Value BitcoinClient::setHdSeed(const std::string& seed, bool rescan) {
    Json::Value params;
    if (!seed.empty()) params.append(seed);
    params.append(rescan);
    return sendRequest("sethdseed", params);
}

Json::Value BitcoinClient::setLabel(const std::string& address, const std::string& label) {
    return sendRequest("setlabel", buildParams(address, label));
}

Json::Value BitcoinClient::setTxFee(double amount) {
    return sendRequest("settxfee", buildParams(amount));
}

Json::Value BitcoinClient::setWalletFlag(const std::string& flag, bool value) {
    return sendRequest("setwalletflag", buildParams(flag, value));
}

Json::Value BitcoinClient::signMessage(const std::string& address, const std::string& message) {
    return sendRequest("signmessage", buildParams(address, message));
}

Json::Value BitcoinClient::signRawTransactionWithWallet(const std::string& hexString, const Json::Value& prevTxs) {
    Json::Value params = buildParams(hexString);
    if (!prevTxs.isNull()) params.append(prevTxs);
    return sendRequest("signrawtransactionwithwallet", params);
}

Json::Value BitcoinClient::unloadWallet(const std::string& walletName) {
    return sendRequest("unloadwallet", walletName.empty() ? Json::Value() : buildParams(walletName));
}

Json::Value BitcoinClient::upgradeWallet(const std::string& walletName) {
    return sendRequest("upgradewallet", walletName.empty() ? Json::Value() : buildParams(walletName));
}

Json::Value BitcoinClient::walletCreateFundedPsbt(const std::vector<Json::Value>& inputs, const std::map<std::string, double>& outputs, int locktime, const Json::Value& options) {
    Json::Value inputsArray;
    for (const auto& input : inputs) inputsArray.append(input);

    Json::Value outputsObj;
    for (const auto& [addr, amt] : outputs) outputsObj[addr] = amt;

    Json::Value params = buildParams(inputsArray, outputsObj, locktime);
    if (!options.isNull()) params.append(options);
    return sendRequest("walletcreatefundedpsbt", params);
}

Json::Value BitcoinClient::walletLock() {
    return sendRequest("walletlock");
}

Json::Value BitcoinClient::walletPassphrase(const std::string& passphrase, int timeout) {
    return sendRequest("walletpassphrase", buildParams(passphrase, timeout));
}

Json::Value BitcoinClient::walletPassphraseChange(const std::string& oldPassphrase, const std::string& newPassphrase) {
    return sendRequest("walletpassphrasechange", buildParams(oldPassphrase, newPassphrase));
}

Json::Value BitcoinClient::walletProcessPsbt(const std::string& psbt, bool sign, bool sighashType, bool bip32derivs) {
    return sendRequest("walletprocesspsbt", buildParams(psbt, sign, sighashType, bip32derivs));
}
