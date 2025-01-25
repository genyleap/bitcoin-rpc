#include "network.hpp"
#include "logger.hpp"
#include <curl/curl.h>
#include <format>

std::string Network::buildQueryString(const std::map<std::string, std::string>& params) const {
    std::shared_lock lock(networkMutex);
    std::string queryString;
    for (const auto& [key, value] : params) {
        queryString += std::format("{}={}&", key, value);
    }
    if (!queryString.empty()) queryString.pop_back(); // Remove trailing '&'
    return queryString;
}

size_t Network::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* outBuffer) {
    size_t totalSize = size * nmemb;
    outBuffer->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

bool Network::sendRequest(const std::string& url, std::string& response, bool verbose) {
    std::shared_lock lock(networkMutex);
    Logger::formattedInfo("Constructed URL: {}", url);

    CURL* curl = curl_easy_init();
    if (!curl) {
        Logger::error("Failed to initialize CURL");
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    if (verbose) curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        Logger::formattedError("CURL error: {}", curl_easy_strerror(res));
        return false;
    }

    return true;
}

bool Network::sendPostRequest(
    const std::string& url,
    const std::string& postData,
    std::string& response,
    const std::string& user,
    const std::string& password,
    const std::map<std::string, std::string>& headers,
    bool verbose
    ) {
    std::shared_lock lock(networkMutex);
    Logger::formattedInfo("Sending POST request to: {}", url);

    CURL* curl = curl_easy_init();
    if (!curl) {
        Logger::error("Failed to initialize CURL");
        return false;
    }

    // Set URL and authentication
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (!user.empty() || !password.empty()) {
        curl_easy_setopt(curl, CURLOPT_USERPWD, (user + ":" + password).c_str());
    }

    // Set POST data
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

    // Set headers
    struct curl_slist* curlHeaders = nullptr;
    for (const auto& [key, value] : headers) {
        curlHeaders = curl_slist_append(curlHeaders, (key + ": " + value).c_str());
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curlHeaders);

    // Set callback for response
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Enable verbose output if requested
    if (verbose) curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(curlHeaders); // Free headers
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        Logger::formattedError("CURL error: {}", curl_easy_strerror(res));
        return false;
    }

    return true;
}
