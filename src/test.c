#include "tar.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    tar t;

    FILE* fp = fopen("testing.tar", "rb");
    fseek(fp, 0, SEEK_END);
    long t_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* tar_buffer = malloc(t_size + 1);

    fread(tar_buffer, 1, t_size, fp);
    fclose(fp);

    tar_open(&t, tar_buffer, t_size);

    ustar_header hdr;
    char* data = NULL;
    while (tar_next(&t, &hdr) != END) {
        uint32_t tar_len = tar_size(&t, &hdr);
        data = malloc(tar_len + 1);
        tar_read(&t, &hdr, data, 0);
        data[tar_len] = '\0';
        printf("%s\n", data);
        free(data);
    }

    tar_close(&t);
    free(tar_buffer);
}
