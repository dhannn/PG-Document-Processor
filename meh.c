#include <dirent.h>
#include <stdio.h>

int main()
{
    DIR *dir = opendir("./");

    struct dirent *entity;
    entity = readdir(dir);
    

    while(entity != (void*)0) {
        printf("%s (%d)\n", entity->d_name, entity->d_namlen);
        entity = readdir(dir);
    }

    closedir(dir);
    return 0;
}
