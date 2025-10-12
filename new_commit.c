/*
PROGRAMA PARA CREAR COMMITS AUTOMATICAMENTE por Juan Yaguaro

Copyright 2025
*/

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char filesToAdd[256];
    printf("Ingresa los archivos a agregar (o . para todos):\n");
    scanf("%s", filesToAdd); //Escanear archivos a agregar

    if(filesToAdd[0] == '\0') //Si no se ingresa nada
    {
        printf("No se ingresaron archivos. Saliendo...\n");
        return 1;
    }

    char addCommand[512];
    snprintf(addCommand, sizeof(addCommand), "git add %s", filesToAdd); //Crear comando
    system(addCommand); //Ejecutar comando

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