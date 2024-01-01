#include "hashf.h"

// Хэш-функция djb2
uint32_t djb2(const void *buff, size_t length) {
    uint32_t hash = DJB2_INIT; // Инициализация хеша
    const uint8_t *data = buff; // Приведение данных к типу uint8_t
    for(size_t i = 0; i < length; i++) {
        hash = ((hash << 5) + hash) + data[i]; // Формула хеширования djb2
    }
    return hash;
}

// Хэш-функция sdbm
uint32_t sdbm(const void *buff, size_t length) {
    uint32_t hash = 0; // Инициализация хеша
    const uint8_t *data = buff; // Приведение данных к типу uint8_t
    for(size_t i = 0; i < length; i++) {
        hash = data[i] + (hash << 6) + (hash << 16) - hash; // Формула хеширования sdbm
    }
    return hash;
}
