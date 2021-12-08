#include <sys/stat.h>       // struct stat, stat(), S_ISREG
#include <unistd.h>         // access(), F_OK, R_OK
#include <string.h>         // strcmp
#include "macros.h"         // EXIT_WITH_MSG()
#include "utils.h"
#include "tokenizer.h"

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

    Tokenizer *t = Tokenizer_init(FileReader_init(fname, fsize));

    char *p_start, *p_end;
    Token tok_type;

    do
    {
        if (EOF == (tok_type = Tokenizer_next(t, &p_start, &p_end))) break;

        size_t len = (p_start != NULL && p_end != NULL) ? p_end - p_start : 0;
        char *str = (char *) malloc_s(len + 1);
        if (len > 0)
            strncpy(str, p_start, len);
        str[len] = '\0';

        printf("%s: %s\n", token_names[tok_type], str);
    } while (!FileReader_isEOF(t->fr));

    Tokenizer_free(t);
}
