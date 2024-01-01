/**
 * @file bitutil.h
 * @author nomemory
 * @brief Файл, содержащий макросы для манипуляций с битами в массиве бит
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef BITUTIL_H

#include <stdlib.h>
#include <stdint.h>

// Структура для представления массива битов
typedef struct bit_vect_s {
    uint32_t *mem; // Указатель на массив битов (память)
    size_t size;   // Размер массива в битах
} bit_vect;

// Прототипы функций для работы с массивом битов
bit_vect *bit_vect_new(size_t num_bits);
void bit_vect_free(bit_vect *vect);
bool bit_vect_get(bit_vect *vect, size_t bit_idx);
void bit_vect_set(bit_vect *vect, size_t bit_idx, bool val);
void bit_vect_set1(bit_vect *vect, size_t bit_idx);
void bit_vect_set0(bit_vect *vect, size_t bit_idx);
void bit_vect_printf(FILE *f); // Функция для вывода массива битов

#define BITUTIL_H
#endif
