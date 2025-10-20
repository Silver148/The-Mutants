/*

    convert_gif_to_sequencePNG.c
    by Juan Yaguaro(aka silverhacker)
	//Se requiere ImageMagick para que funcione(ni siquiera se si esta vaina funciona xD)

*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("convert_gif_to_sequencePNG by Juan Yaguaro(aka silverhacker)\n");

    if (argc != 4) {
        printf("Usage: %s <input_gif_file> <output_path> <name_frames>\n", argv[0]);
        return 1;
    }

    char *input_gif = argv[1];
    char *output_path = argv[2];
    char *name_frames = argv[3];

    char command[512];
    snprintf(command, sizeof(command), "convert %s %s%s-frame_%%03d.png", input_gif, output_path, name_frames);
    system(command);

    return 0;
}