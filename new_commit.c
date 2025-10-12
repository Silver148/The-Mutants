/*
PROGRAMA PARA CREAR COMMITS AUTOMATICAMENTE por Juan Yaguaro

Copyright 2025
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    system("git add .");
    printf("Ingresa nombre del commit:\n");
    char commitMessage[256];

    scanf("%[^\n]", commitMessage); //Escanear nombre del commit

    char command[512];
    snprintf(command, sizeof(command), "git commit -m \"%s\"", commitMessage); //Crear comando

    system(command); //Ejecutar comando

    system("git push -u origin main"); //Hacer push al repositorio
    return 0;
}