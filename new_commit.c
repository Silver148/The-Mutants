#include <stdio.h>
#include <stdlib.h>

int main()
{
    system("git add .");
    printf("Ingresa nombre del commit:\n");
    char commitMessage[256];

    scanf("%[^\n]", commitMessage);

    char command[512];
    snprintf(command, sizeof(command), "git commit -m \"%s\"", commitMessage);

    system(command);

    system("git push -u origin main");
    return 0;
}