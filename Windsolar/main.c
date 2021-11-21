#define DEBUG

#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "macros.h"


char *usage = "Usage: windsolar FILE\n"
              "https://github.com/itay-raveh/windsolar\n";

int main(int argc, char *argv[])
{
    // ~~~~~~~~~~~~~~~~~ get filename ~~~~~~~~~~~~~~~~~

    if (argc != 2)
        EXIT_WITH_MSG(EXIT_FAILURE, "%s", usage);

    // if argument is '--help' or '-h'
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        EXIT_WITH_MSG(EXIT_SUCCESS, "%s", usage);

    char *fname = argv[1];

    if (access(fname, F_OK) != 0)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: File '%s' does not exist\n", fname);

    if (access(fname, R_OK) != 0)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: No permissions to read file '%s'\n", fname);

    TRACE("using file: %s\n", fname);
}
