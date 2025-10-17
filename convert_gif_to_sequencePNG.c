/*

    convert_gif_to_sequencePNG.c
    by Juan Yaguaro(aka silverhacker)

*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("convert_gif_to_sequencePNG by Juan Yaguaro(aka silverhacker)\n");

    if (argc != 2) {
        printf("Usage: %s <input_gif_file>\n", argv[0]);
        return 1;
    }

    char *input_gif = argv[1];

    char command[512];
    snprintf(command, sizeof(command), "convert %s frame_%%03d.png", input_gif);
    system(command);

    return 0;
}