#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "bloom.h"
#include "hashf.h"

/**
 * @brief Выделяет память в куче для нового фильтра Блума.
 *
 * @param size Размер внутреннего массива bit_vect
 * @param num_functions Количество хэш-функций, используемых
 * @param ... Хэш-функции в виде переменного числа аргументов
 * 
 * @return bloom_filter* Динамически выделенный фильтр Блума
 */
bloom_filter *bloom_filter_new(size_t size, size_t num_functions, ...) {
    va_list argp;
    bloom_filter *filter = malloc(sizeof(*filter));
    if (NULL == filter) {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        exit(EXIT_FAILURE);
    }
    filter->num_items = 0;
    filter->vect = bit_vect_new(size);
    filter->num_functions = num_functions;
    filter->hash_functions = malloc(sizeof(hash32_func) * num_functions);
    if (NULL == filter->hash_functions) {
        fprintf(stderr, "Ошибка выделения памяти.\n");
        exit(EXIT_FAILURE);
    }
    va_start(argp, num_functions);
    for (unsigned i = 0; i < num_functions; i++) {
        filter->hash_functions[i] = va_arg(argp, hash32_func);
    }
    va_end(argp);
    return filter;
}

/**
 * @brief Выделяет память в куче для нового фильтра Блума
 *
 * Фильтр Блума использует 2 хэш-функции по умолчанию (из hash.h): djb2 и sdbm.
 *
 * @param size Размер внутреннего массива bit_vect
 * @return bloom_filter* Динамически выделенный фильтр Блума
 */
bloom_filter *bloom_filter_new_default(size_t size) {
    return bloom_filter_new(size, 2, djb2, sdbm);
}

/**
 * @brief Освобождает память, связанную с фильтром Блума (включая внутренний массив bit_vect)
 *
 * @param filter
 */
void bloom_filter_free(bloom_filter *filter) {
    bit_vect_free(filter->vect);
    free(filter->hash_functions);
    free(filter);
}

/**
 * @brief Добавляет элемент в фильтр Блума
 *
 * @param filter Фильтр Блума
 * @param data Элемент для добавления в виде указателя const void*
 * @param length Длина элемента (данные читаются как uint8_t)
 */
void bloom_filter_put(bloom_filter *filter, const void *data, size_t length) {
    for (unsigned i = 0; i < filter->num_functions; i++) {
        uint32_t cur_hash = filter->hash_functions[i](data, length);
        bit_vect_set1(filter->vect, cur_hash % filter->vect->size);
        filter->num_items++;
    }
}

/**
 * @brief Добавляет строку (const char *str) в фильтр Блума
 *
 * @param filter Фильтр Блума
 * @param str Строка, которую добавляют
 */
void bloom_filter_put_str(bloom_filter *filter, const char *str) {
    bloom_filter_put(filter, str, strlen(str));
}

/**
 * @brief Проверяет, найден ли элемент в фильтре Блума
 *
 * @param filter Фильтр Блума
 * @param data Элемент для проверки
 * @param lentgth Длина элемента (данные читаются как uint8_t)
 * @return true Если элемент найден
 * @return false Если элемент не найден
 */
bool bloom_filter_test(bloom_filter *filter, const void *data, size_t length) {
    for (unsigned i = 0; i < filter->num_functions; i++) {
        uint32_t cur_hash = filter->hash_functions[i](data, length);
        if (!bit_vect_get(filter->vect, cur_hash % filter->vect->size)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Проверяет, найдена ли строка (const char *str) в фильтре Блума
 *
 * @param filter  Фильтр Блума
 * @param str Строка, которую проверяют
 * @return true Если строка найдена
 * @return false Если строка не найдена
 */
bool bloom_filter_test_str(bloom_filter *filter, const char *str) {
    return bloom_filter_test(filter, str, strlen(str));
}
