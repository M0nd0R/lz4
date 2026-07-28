/*
 * Regression: LZ4IO_compressMultipleFilenames must allocate
 * ifnSize+suffixSize+1, not ifnSize+20 (heap overflow when suffix > 20).
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lz4io.h"

int main(void)
{
    char inpath[] = "/tmp/lz4_longsuf_XXXXXX";
    int fd = mkstemp(inpath);
    char suffix[65];
    const char *files[1];
    LZ4IO_prefs_t *prefs;
    int rc;
    char outpath[512];

    assert(fd >= 0);
    assert(write(fd, "abcdefghijklmnopqrstuvwxyz\n", 27) == 27);
    close(fd);

    memset(suffix, 's', 64);
    suffix[64] = '\0';
    assert(strlen(suffix) > 20);

    files[0] = inpath;
    prefs = LZ4IO_defaultPreferences();
    assert(prefs);

    rc = LZ4IO_compressMultipleFilenames(files, 1, suffix, 1, prefs);
    snprintf(outpath, sizeof(outpath), "%s%s", inpath, suffix);
    printf("rc=%d out_exists=%d out=%s\n", rc, access(outpath, F_OK) == 0, outpath);

    if (access(outpath, F_OK) == 0) unlink(outpath);
    unlink(inpath);
    LZ4IO_freePreferences(prefs);

    /* Expect zero missed files and successful output creation */
    return (rc != 0);
}
