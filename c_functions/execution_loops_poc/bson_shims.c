#include <time.h>
#include <unistd.h>
#include <stddef.h>

// Stub time()
time_t time(time_t *t) {
    return 0;
}

// Stub gethostname()
int gethostname(char *name, size_t len) {
    return 0;
}
