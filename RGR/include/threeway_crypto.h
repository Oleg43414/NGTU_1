// threeway_crypto.h
#ifndef THREEWAY_CRYPTO_H
#define THREEWAY_CRYPTO_H

#include <cstdint>
#include <string>
#include <vector>

struct ThreeWayKeys {
    uint32_t key[3];
};

#ifdef __cplusplus
extern "C" {
#endif

void run_threeway_crypto();

#ifdef __cplusplus
}
#endif

#endif // THREEWAY_CRYPTO_H