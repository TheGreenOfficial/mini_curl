#include "mini_curl.h" // stdlib.h is inside this for free() & stdio also...

int main() {
    char *resp = mini_curl("example.com");
    if (resp) {
        printf("%s\n", resp);
        free(resp);
    }
    return 0;
}

