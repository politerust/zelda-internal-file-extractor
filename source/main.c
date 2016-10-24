#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"

#define OOTDBG_FILENAME_LIST_OFFSET 0xBE80

#define OOTDBG_FILE_TABLE_OFFSET 0x12F70
#define OOTDBG_FILE_TABLE_END_OFFSET 0x19030
#define OOTDBG_FILE_TABLE_LENGTH (OOTDBG_FILE_TABLE_END_OFFSET-OOTDBG_FILE_TABLE_OFFSET)
#define FILE_TABLE_ENTRY_LENGTH 0x10

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s  <ROM path>  [<output path>]\n", program_name);
}

const char *get_filename(const uint8_t *filename_list) {
    assert(filename_list != NULL);
    static char filename[1024];
    size_t i = 0;
    
    while (isprint(filename_list[i])) {
        filename[i] = filename_list[i];
        i++;
    }
    filename[i] = '\0';

    if (strstr(filename, "ovl") || strstr(filename, "object")) {
        return strcat(filename, ".zobj");
    } else if (strstr(filename, "room")) {
        return strcat(filename, ".zmap");
    } else if (strstr(filename, "scene")) {
        return strcat(filename, ".zscene");
    } else if (strstr(filename, "code")) {
        return strcat(filename, ".zasm");
    } else {
        return strcat(filename, ".zdata");
    }
}

const uint8_t *find_next_filename(const uint8_t *filename_list) {
    assert(filename_list != NULL);

    // Skip past the name it's currently pointing to...
    while (isprint(*filename_list))
        filename_list++;

    // ...and skip over the null characters between the names
    while (!isprint(*filename_list))
        filename_list++;

    return filename_list;
}

void extract_files(const uint8_t *rom, const char *output_dir) {
    const uint8_t *file_table = &rom[OOTDBG_FILE_TABLE_OFFSET];
    const uint8_t *filename_list = &rom[OOTDBG_FILENAME_LIST_OFFSET];

    for (size_t i = 0; i < OOTDBG_FILE_TABLE_LENGTH; i += FILE_TABLE_ENTRY_LENGTH) {
        uint32_t current_file_start = bytes_to_dword(file_table[i], file_table[i + 1],
                                                     file_table[i + 2], file_table[i + 3]);
        uint32_t current_file_end = bytes_to_dword(file_table[i + 4], file_table[i + 5],
                                                   file_table[i + 6], file_table[i + 7]);

        // There are a few blank entries at the end of the list
        if (0 == current_file_end)
            break;

        const char *filename = get_filename(filename_list);

        if (strlen(output_dir) > 0) {
            char *output_file = malloc(strlen(output_dir) + strlen(filename) + 2);

#ifdef _WIN32
            sprintf(output_file, "%s\\%s", output_dir, filename);
#else
            sprintf(output_file, "%s/%s", output_dir, filename);
#endif

            write_file_to_disk(&rom[current_file_start],
                               current_file_end - current_file_start,
                               output_file);

            free(output_file);
        } else {
            write_file_to_disk(&rom[current_file_start],
                               current_file_end - current_file_start,
                               filename);
        }
        
        filename_list = find_next_filename(filename_list);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    uint8_t *rom = read_file_into_memory(argv[1]);

    // Use the specified output directory if the user gave one
    if (argc == 3) {
        extract_files(rom, argv[2]);
    } else {
        extract_files(rom, "");
    }

    free(rom);

    return EXIT_SUCCESS;
}
