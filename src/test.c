#define EMBEDDED
#include "tar.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tar.h>
int main() {
    tar t;

    FILE* fp = fopen("testing.tar", "rb");
    fseek(fp, 0, SEEK_END);
    long t_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* tar_buffer = malloc(t_size);

    if (tar_buffer == NULL) {
        puts("malloc failed");
        return 0;
    }

    fread(tar_buffer, 1, t_size, fp);

    tar_open(&t, tar_buffer, t_size);

    ustar_header hdr;
    char* buffer = NULL;
    while (tar_next(&t, &hdr) != END) {
        uint32_t size = tar_size(&t, &hdr);
        buffer = realloc(buffer, size + 1);

        tar_read(&t, &hdr, buffer, size);
        buffer[size] = '\0';
        printf("%s:\n%s", hdr.file_name, buffer);
    }

    tar_close(&t);

    free(buffer);
    free(tar_buffer);
}
