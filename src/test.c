#include "tar.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tar.h>
int main() {
    tar t;
    tar_open(&t, "testing.tar", "rb");

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
}
