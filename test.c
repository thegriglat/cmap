#include "stdio.h"
#include "cmap.h"

int main()
{
    CMap *map = cmap_init();
    int a[1024 * 1024];
    char **s;
    s = (char**)malloc(sizeof(char*) * 1024 * 1024);
    int i;
    printf("Populating arrays ...\n");
    for (i = 0; i < 1024 * 1024; ++i) {
        a[i] = i;
        s[i] = (char*)malloc(sizeof(char) * 10);
        sprintf(s[i], "%d", i);
    }
    printf("done.\n");
    printf("Adding values to the map ...\n");
    for (i = 0; i < 1024 * 1024; ++i)
        cmap_add(map, s[i], &(a[i]));
    printf("done.\n");
    printf("Checking map ...\n");
    unsigned int collisions = 0;
    for (i = 0; i < 1024 * 1024; ++i) {
        int val = *(int*)cmap_get(map, s[i]);
        if (val != a[i]) {
            printf("collision: map[\"%s\"] = %d (expected %d)\n", s[i], val, a[i]);
            collisions++;
        }
    }
    printf("done.\n");
    printf("Collisions: %f%\n", (float)collisions * 100. / 1024 / 1024);
    cmap_free(map);
}