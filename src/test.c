#include "tar.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE* fp = fopen("testing.tar", "rb");
    fseek(fp, 0, SEEK_END);
    long t_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* tar_buffer = malloc(t_size + 1);

    fread(tar_buffer, 1, t_size, fp);
    fclose(fp);

    // Declare the tar object and initialize it with the tar buffer and its size
    tar t;
    tar_open(&t, tar_buffer, t_size);

    // Declare the header which we will copy to
    ustar_header hdr;
    // Read until the end of the tar buffer
    while (tar_next(&t, &hdr) != END) {
        printf("%s\n", hdr.file_name);
    }

    tar_close(&t);
    free(tar_buffer);
}
