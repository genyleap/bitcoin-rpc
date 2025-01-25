#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <string>
#include <map>
#include <shared_mutex>

/**
 * @class Network
 * @brief A thread-safe utility class for handling network requests and query string construction.
 *
 * Provides functionality to send HTTP requests using `libcurl` and to build
 * query strings from key-value parameter pairs. Ensures thread safety with `std::shared_mutex`.
 */
class Network {
private:
    /**
     * @brief Mutex for ensuring thread-safe operations in the `Network` class.
     *
     * Allows multiple readers but only one writer at a time.
     */
    mutable std::shared_mutex networkMutex;

    /**
     * @brief Callback function to handle data received from CURL.
     *
     * This function is invoked by CURL to write the response data to a string buffer.
     *
     * @param contents Pointer to the data received.
     * @param size Size of each data element.
     * @param nmemb Number of elements in the data.
     * @param outBuffer Pointer to the string where the data will be stored.
     * @return The total size of the data processed (size * nmemb).
     */
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* outBuffer);

public:
    /**
     * @brief Constructs a query string from a map of key-value pairs.
     *
     * This method takes a map of parameters and generates a query string
     * suitable for use in an HTTP GET request.
     *
     * @param params A map containing key-value pairs representing query parameters.
     * @return A string representing the query string (e.g., `key1=value1&key2=value2`).
     */
    std::string buildQueryString(const std::map<std::string, std::string>& params) const;

    /**
     * @brief Sends an HTTP GET request to the specified URL.
     *
     * Uses `libcurl` to send an HTTP request. The response is written to the provided
     * `response` string. Optionally, verbose output can be enabled for debugging.
     *
     * @param url The full URL to which the request will be sent.
     * @param response A reference to a string where the server's response will be stored.
     * @param verbose A boolean flag to enable verbose output (default: false).
     * @return `true` if the request was successful; `false` otherwise.
     */
    bool sendRequest(const std::string& url, std::string& response, bool verbose = false);

    /**
     * @brief Sends an HTTP POST request to the specified URL with the provided data.
     *      * This function sends an HTTP POST request to the given URL, including optional
     * authentication, custom headers, and verbose logging. The response from the server
     * is stored in the `response` parameter.
     *      * @param url The URL to which the POST request is sent.
     * @param postData The data to be sent in the body of the POST request.
     * @param[out] response A string to store the server's response.
     * @param user Optional username for HTTP authentication (default is empty).
     * @param password Optional password for HTTP authentication (default is empty).
     * @param headers Optional map of custom HTTP headers to include in the request (default is empty).
     * @param verbose If true, enables verbose logging for debugging purposes (default is false).
     *      * @return bool True if the request was successful and a response was received, false otherwise.
     *      * @note The function does not throw exceptions but returns false on failure.
     * @warning Ensure that the `url` is properly formatted and accessible.
     * @endcode
     */
    bool sendPostRequest(
        const std::string& url, const std::string& postData,
        std::string& response,
        const std::string& user = "",
        const std::string& password = "",
        const std::map<std::string, std::string>& headers = {},
        bool verbose = false);
};

#endif // NETWORK_HPP
