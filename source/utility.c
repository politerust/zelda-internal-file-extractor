#include "utility.h"

size_t get_file_size(FILE *file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    assert(size > 0);
    rewind(file);
    return size;
}

size_t get_unopened_file_size(const char *filename) {
    FILE *file_handle = fopen(filename, "rb");
    assert(file_handle != NULL);
    
    size_t file_size = get_file_size(file_handle);

    int close_success = fclose(file_handle);
    assert(close_success == 0);

    return file_size;
}

uint8_t *read_file_into_memory(const char *filename) {
    FILE *file_handle = fopen(filename, "rb");
    assert(file_handle != NULL);
    
    size_t file_size = get_file_size(file_handle);
    uint8_t *file_buffer = calloc(file_size, sizeof(uint8_t));
    assert(file_buffer != NULL);

    size_t items_read = fread(file_buffer, sizeof(uint8_t), file_size, file_handle);
    assert(items_read == file_size*sizeof(uint8_t));

    int close_success = fclose(file_handle);
    assert(close_success == 0);

    return file_buffer;
}

void write_file_to_disk(const uint8_t *data, uint32_t length, const char *filename) {
    FILE *file_handle = fopen(filename, "wb");
    assert(file_handle != NULL);

    size_t items_written = fwrite(data, sizeof(uint8_t), length, file_handle);
    assert(items_written == length*sizeof(uint8_t));

    int close_success = fclose(file_handle);
    assert(close_success == 0);
}

uint32_t bytes_to_dword(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
    return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}

