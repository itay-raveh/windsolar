#include <unistd.h>
#include <string.h>
#include "utils.h"
#define DEBUG 1
#include "debug.h"


char *usage = "Usage: windsolar FILE\n"
              "https://github.com/itay-raveh/windsolar\n";

int main(int argc, char *argv[])
{
    if (argc != 2) print_and_exit(usage, EXIT_FAILURE);
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) print_and_exit(usage, EXIT_SUCCESS);

    char* fname = argv[1];

    if (access(fname, F_OK) != 0) print_and_exit("Error: File does not exist", EXIT_FAILURE);
    if (access(fname, R_OK) != 0) print_and_exit("Error: No permissions to read file", EXIT_FAILURE);

    debug("using file: %s", fname);
}
