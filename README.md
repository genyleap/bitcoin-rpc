# Bitcoin RPC Client
A lightweight and easy-to-use C++ client for interacting with a Bitcoin node via JSON-RPC. This library provides a comprehensive interface to communicate with a Bitcoin node, enabling developers to perform various blockchain operations, manage wallets, and interact with the Bitcoin network.
![splash](https://github.com/user-attachments/assets/9c6d1243-c826-4fe3-8a38-609aae710929)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-23-red.svg)](https://en.cppreference.com/w/cpp/23)

---

## Features

- **Full JSON-RPC Support**: Implements all Bitcoin Core RPC methods.
- **Easy Integration**: Simple and intuitive API for interacting with a Bitcoin node.
- **Cross-Platform**: Works on Linux, macOS, and Windows.
- **Modular Design**: Extensible and customizable for advanced use cases.
- **Comprehensive Documentation**: Well-documented code with examples.

---

## Installation

### Prerequisites

- **Bitcoin Core**: Ensure you have a running Bitcoin node with JSON-RPC enabled.
- **C++ Compiler**: Supports C++23 or later (e.g., GCC, Clang, or MSVC).
- **Dependencies**:
  - [JSONCPP](https://github.com/open-source-parsers/jsoncpp) for JSON parsing.
  - [cURL](https://curl.se/libcurl/) for HTTP requests.

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/genyleap/bitcoin-rpc.git
   cd bitcoin-rpc
   ```

2. Build the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

3. Install the library (optional):
   ```bash
   sudo make install
   ```

---

## Usage

### Basic Example

Here’s how to use the `BitcoinClient` to interact with a Bitcoin node:

```cpp
#include "bitcoinclient.hpp"
#include <iostream>

int main() {
    // Initialize the Bitcoin client
    BitcoinClient client("rpcuser", "rpcpassword", "http://127.0.0.1:8332/");

    // Get the best block hash
    Json::Value bestBlockHash = client.getBestBlockHash();
    std::cout << "Best Block Hash: " << bestBlockHash.asString() << std::endl;

    // Get blockchain info
    Json::Value blockchainInfo = client.getBlockchainInfo();
    std::cout << "Blockchain Height: " << blockchainInfo["blocks"].asInt() << std::endl;

    return 0;
}
```

### Available Methods

The `BitcoinClient` class supports all Bitcoin Core RPC methods, including:

- **Blockchain RPCs**: `getBestBlockHash`, `getBlock`, `getBlockchainInfo`, etc.
- **Wallet RPCs**: `getBalance`, `sendToAddress`, `createWallet`, etc.
- **Network RPCs**: `getNetworkInfo`, `addNode`, `ping`, etc.
- **Mining RPCs**: `getMiningInfo`, `getBlockTemplate`, `submitBlock`, etc.
- **Raw Transactions RPCs**: `createRawTransaction`, `signRawTransaction`, `sendRawTransaction`, etc.

For a full list of methods, refer to the [documentation](#documentation).

---

## Configuration

To connect to your Bitcoin node, update the following parameters in your code:

- **RPC Username**: Set in `bitcoin.conf` as `rpcuser`.
- **RPC Password**: Set in `bitcoin.conf` as `rpcpassword`.
- **RPC URL**: Default is `http://127.0.0.1:8332/`.

Example `bitcoin.conf`:
```ini
rpcuser=yourusername
rpcpassword=yourpassword
rpcallowip=127.0.0.1
server=1
```

---

## Documentation

For detailed documentation, refer to the [header file](include/BitcoinClient.hpp). Each method is documented with Doxygen-style comments, including descriptions, parameters, return values, and examples.

To generate the documentation locally:
```bash
doxygen Doxyfile
```

---

## Contributing

Contributions are welcome! If you’d like to contribute, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bugfix.
3. Commit your changes and push to your fork.
4. Submit a pull request with a detailed description of your changes.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Support

If you find this project useful, please consider giving it a ⭐️ on GitHub. For questions or issues, open an [issue](https://github.com/genyleap/bitcoin-rpc/issues).

---

## Acknowledgments

- [Bitcoin Core](https://bitcoincore.org/) for the JSON-RPC API.
- [JSONCPP](https://github.com/open-source-parsers/jsoncpp) for JSON parsing.
- [cURL](https://curl.se/libcurl/) for HTTP requests.
