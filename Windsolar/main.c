#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "macros.h"
#include "file_reader.h"


char *usage = "Usage: windsolar FILE\n"
              "https://github.com/itay-raveh/windsolar\n";

int main(int argc, char *argv[])
{
    // ~~~~~~~~~~~~~~~~~~ parse args ~~~~~~~~~~~~~~~~~~

    if (argc != 2)
        EXIT_WITH_MSG(EXIT_FAILURE, "%s", usage);

    // if argument is '--help' or '-h'
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
        EXIT_WITH_MSG(EXIT_SUCCESS, "%s", usage);

    char *fname = argv[1];

    // ~~~~~~~~~~~~~~~~~ verify file ~~~~~~~~~~~~~~~~~~
    TRACE("got file %s\n", fname);

    // if the file does not exist
    if (access(fname, F_OK) != 0)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: File '%s' does not exist\n", fname);

    // if there are no read permissions for the file
    if (access(fname, R_OK) != 0)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: No permissions to read file '%s'\n", fname);

    // ~~~~~~~~~~~~~~~~ get file stats ~~~~~~~~~~~~~~~~
    TRACE("stating %s\n", fname);

    struct stat sb;

    // if stats could not be retrieved
    if (stat(fname, &sb) == -1)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: Failed to get stats on %s\n", fname);

    // if the file is not a regular file
    if (!S_ISREG(sb.st_mode))
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: %s is not a regular file\n", fname);

    FileReader *fr = FileReader_init(fname, sb.st_size);
    

    FileReader_free(fr);
}
