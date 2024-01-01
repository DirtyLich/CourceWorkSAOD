#include <stdio.h>
#include <stdlib.h>

#include "bloom.h"

int main(void) {
    // Создаем фильтр Блума с размером 1024 и двумя хэш-функциями по умолчанию
    bloom_filter *filter = bloom_filter_new_default(1024);

    // Добавляем строку "abc" в фильтр Блума
    bloom_filter_put_str(filter, "abc");

    // Проверяем, содержится ли строка "abc" в фильтре Блума и выводим результат
    printf("%d\n", bloom_filter_test_str(filter, "abc"));

    // Проверяем, содержится ли строка "bcd" в фильтре Блума и выводим результат
    printf("%d\n", bloom_filter_test_str(filter, "bcd"));

    // Проверяем, содержится ли строка "0" в фильтре Блума и выводим результат
    printf("%d\n", bloom_filter_test_str(filter, "0"));

    // Проверяем, содержится ли строка "1" в фильтре Блума и выводим результат
    printf("%d\n", bloom_filter_test_str(filter, "1"));

    // Добавляем строку "2" в фильтр Блума
    bloom_filter_put_str(filter, "2");

    // Проверяем, содержится ли строка "2" в фильтре Блума и выводим результат
    printf("%d\n", bloom_filter_test_str(filter, "2"));
    
    return EXIT_SUCCESS;
}
