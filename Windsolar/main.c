#include <sys/stat.h>       // struct stat, stat(), S_ISREG
#include <unistd.h>         // access(), F_OK, R_OK
#include <string.h>         // strcmp
#include "macros.h"         // EXIT_WITH_MSG()
#include "reader.h"
#include "tokenizer.h"
#include "parse_tree.h"
#include "stacks.h"
#include "runtime.h"

/**
 * Parse command line args.
 * <br><br>
 * Usage: windsolar FILE
 * <br>
 * --help - print usage and exit
 * @param argc
 * @param argv
 * @return fname if one was found
 */
char *parse_args(const int argc, char *restrict argv[])
{
    char *usage = "Usage: windsolar FILE\n"
                  "\n"
                  "--help   -   print usage and exit\n"
                  "https://github.com/itay-raveh/windsolar\n";

    if (argc != 2)
        EXIT_WITH_MSG(EXIT_FAILURE, "%s", usage);

    if (strcmp(argv[1], "--help") == 0)
        EXIT_WITH_MSG(EXIT_SUCCESS, "%s", usage);

    TRACE("input-file name: %s\n", argv[1]);
    return argv[1];
}

/**
 * Verify that a given file:
 *  <br>1. exists
 *  <br>2. is readable
 *  <br>3. is a regular file
 * @param fname
 * @return size of the file
 */
size_t verify_file(const char *fname)
{
    // if the file does not exist
    if (access(fname, F_OK) != 0)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: File '%s' does not exist\n", fname);

    // if there are no read permissions for the file
    if (access(fname, R_OK) != 0)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: No permissions to read file '%s'\n", fname);

    struct stat sb;

    // if stats could not be retrieved
    if (stat(fname, &sb) == -1)
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: Failed to get stats on %s\n", fname);

    // if the file is not a regular file
    if (!S_ISREG(sb.st_mode))
        EXIT_WITH_MSG(EXIT_FAILURE, "Error: %s is not a regular file\n", fname);

    TRACE("input-file size: %ld\n", sb.st_size);
    return sb.st_size;
}

int main(int argc, char *argv[])
{
    char *fname = parse_args(argc, argv);
    size_t fsize = verify_file(fname);

    Reader *r = Reader_fromFile(fname, fsize);
    Tokenizer *t = Tokenizer_fromReader(r);
    LabelNode *pt = ParseTree_fromTokenizer(t, true);

    Reader_free(r);
    Tokenizer_free(t);

    ParseTree_print(pt);

    ProgramStack *ps = ProgramStack_new();
    DataStack *ds = DataStack_new();

    bool res = mainloop(pt, ps, ds);

    ParseTree_free(pt);
    ProgramStack_free(ps);
    DataStack_free(ds);

    return res ? EXIT_SUCCESS : EXIT_FAILURE;
}
