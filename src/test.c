#include "tar.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tar.h>
int main() {
    tar t;

    FILE* fp = fopen("testing.tar", "rb");
    fseek(fp, 0, SEEK_END);
    long tar_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* tar_buffer = malloc(tar_size);
    fread(tar_buffer, 1, tar_size, fp);

    tar_open(&t, tar_buffer, tar_size);

    ustar_header hdr;
    char* buffer = NULL;
    while (tar_next(&t, &hdr) != END) {
        uint32_t size = tar_size(&t, &hdr);
        buffer = realloc(buffer, size + 1);

        tar_read(&t, &hdr, buffer, size);
        buffer[size] = '\0';
        printf("%s:\n%s", hdr.file_name, buffer);
    }
    free(buffer);
    free(tar_buffer);
}
