#ifndef UTILITY_H___
#define UTILITY_H___

#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

size_t get_file_size(FILE *);

size_t get_unopened_file_size(const char *);

uint8_t *read_file_into_memory(const char *);

void write_file_to_disk(const uint8_t *, uint32_t , const char *);

uint32_t bytes_to_dword(uint8_t, uint8_t, uint8_t, uint8_t);

#endif
