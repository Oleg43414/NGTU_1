#ifndef RSA_CRYPTO_H
#define RSA_CRYPTO_H

#include <cstdint>
#include <vector>
#include <string>

#ifdef _WIN32
    #ifdef RSA_EXPORTS
        #define RSA_API __declspec(dllexport)
    #else
        #define RSA_API __declspec(dllimport)
    #endif
#else
    #define RSA_API __attribute__((visibility("default")))
#endif

// Структура для RSA ключей
struct RSAKeys {
    int64_t publicKey;
    int64_t privateKey;
    int64_t n;
};

// ВАЖНО: extern "C" отключает name mangling
extern "C" {
    RSA_API RSAKeys generateRSAKeys();
    RSA_API std::vector<int64_t> encryptMessageRSA(const std::string& message, int64_t e, int64_t n);
    RSA_API std::string decryptMessageRSA(const std::vector<int64_t>& encrypted, int64_t d, int64_t n);
    RSA_API void encryptFileRSA(const std::string& inputFile, const std::string& outputFile, int64_t e, int64_t n);
    RSA_API void decryptFileRSA(const std::string& inputFile, const std::string& outputFile, int64_t d, int64_t n);
    RSA_API void run_rsa_crypto();
}

#endif