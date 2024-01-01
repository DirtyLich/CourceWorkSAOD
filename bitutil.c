#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "bitutil.h"

#define BITS_IN_BYTE 8
#define BITS_IN_TYPE(type) (BITS_IN_BYTE * (sizeof(type)))

/**
 * @brief Выделяет динамическую память для новой структуры bit_vect в куче
 * 
 * @param num_bits Количество битов в bit_vect
 * @return bit_vect* Указатель на вновь выделенную структуру bit_vect
 */

bit_vect *bit_vect_new(size_t num_bits) {
    // Выделяем память для структуры bit_vect
    bit_vect *vect = malloc(sizeof(*vect));
    if (NULL == vect) {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        exit(EXIT_FAILURE);
    }

    // Рассчитываем размер памяти, необходимый для массива бит
    size_t mem_size = num_bits / BITS_IN_TYPE(uint32_t);
    if (!(num_bits % BITS_IN_TYPE(uint32_t))) {
        mem_size++;
    }

    // Выделяем память для массива бит
    vect->mem = calloc(mem_size, sizeof(*(vect->mem)));
    if (NULL == vect->mem) {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        exit(EXIT_FAILURE);
    }

    // Устанавливаем размер bit_vect
    vect->size = num_bits;

    return vect;
}

/**
 * @brief Освобождает память, связанную с структурой bit_vect, выделенной в куче
 * 
 * @param vect Память, которую нужно освободить
 */

void bit_vect_free(bit_vect *vect) {
    free(vect->mem);
    free(vect);
}

/**
 * @brief Получает значение, связанное с битом по индексу
 * 
 * @param vect bit_vect, в котором хранятся биты
 * @param bit_idx Бит, который мы хотим получить
 * @return true Если бит равен 1
 * @return false Если бит равен 0
 */
bool bit_vect_get(bit_vect *vect, size_t bit_idx) {
    if (bit_idx >= vect->size) {
        fprintf(stderr, "Индекс за пределами границ: bit_idx=%zu, vect->size=%zu\n",
                bit_idx, vect->size);
        exit(EXIT_FAILURE);
    }

    // Рассчитываем смещение в массиве значений uint32_t
    size_t chunk_offset = bit_idx / BITS_IN_TYPE(uint32_t);
    // Рассчитываем смещение внутри значения uint32_t
    size_t bit_offset = bit_idx & (BITS_IN_TYPE(uint32_t) - 1);
    uint32_t byte = vect->mem[chunk_offset];

    return (byte >> bit_offset) & 1;
}

/**
 * @brief Устанавливает бит по индексу в 1 или 0
 * 
 * @param vect bit_vect, в котором хранятся биты
 * @param bit_idx Индекс бита, который мы хотим изменить
 * @param val 0 (false) или 1 (true)
 */

void bit_vect_set(bit_vect *vect, size_t bit_idx, bool val) {
    if (bit_idx >= vect->size) {
        fprintf(stderr, "Индекс за пределами границ: bit_idx=%zu, vect->size=%zu\n",
                bit_idx, vect->size);
        exit(EXIT_FAILURE);
    }

    // Рассчитываем смещение в массиве значений uint32_t
    size_t chunk_offset = bit_idx / BITS_IN_TYPE(uint32_t);
    // Рассчитываем смещение внутри значения uint32_t
    size_t bit_offset = bit_idx & (BITS_IN_TYPE(uint32_t) - 1);
    uint32_t *byte = &(vect->mem[chunk_offset]);

    if (val) {
        // Устанавливаем `bit_idx` в 1 (true)
        *byte |= ((uint32_t)1) << bit_offset;
    } else {
        // Устанавливаем бит `bit_idx` в 0 (false)
        *byte &= ~(1 << bit_offset);
    }
}

/**
 * @brief Устанавливает бит по индексу внутри bit_vect в 1 (true)
 */
void bit_vect_set1(bit_vect *vect, size_t bit_idx) {
    bit_vect_set(vect, bit_idx, true);
}

/**
 * @brief Устанавливает бит по индексу внутри bit_vect в 0 (false)
 */
void bit_vect_set0(bit_vect *vect, size_t bit_idx) {
    bit_vect_set(vect, bit_idx, false);
}
