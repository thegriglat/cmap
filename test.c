#include "stdio.h"
#include "cmap.h"

#define NVAL (1024 * 1024)

int main()
{
    CMap *map = cmap_init();
    int a[1024 * 1024];
    char **s;
    s = (char**)malloc(sizeof(char*) * NVAL);
    int i;
    printf("Populating arrays ...\n");
    for (i = 0; i < NVAL; ++i) {
        a[i] = i;
        s[i] = (char*)malloc(sizeof(char) * 32);
        sprintf(s[i], "%d", i);
    }
    printf("done.\n");
    printf("Adding %d values to the map ...\n", NVAL);
    for (i = 0; i < NVAL; ++i)
        cmap_add(map, s[i], &(a[i]));
    printf("done.\n");
    printf("Checking map ...\n");
    unsigned int collisions = 0;
    for (i = 0; i < NVAL; ++i) {
        int val = *(int*)cmap_get(map, s[i]);
        if (val != a[i]) {
            printf("collision: map[\"%s\"] = %d (expected %d)\n", s[i], val, a[i]);
            collisions++;
        }
    }
    printf("done.\n");
    printf("Collisions: %f%\n", (float)collisions * 100. / NVAL);
    cmap_free(map);
}