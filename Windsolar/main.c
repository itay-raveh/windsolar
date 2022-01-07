#include <stdio.h>
#include <stdlib.h>
#include "args.h"
#include "runtime.h"


int main(int argc, char *argv[])
{
    Args *a = Args_fromArgv(argc, argv);
    if (a == NULL)
    {
        fprintf(stderr, "USAGE\n");
        return EXIT_FAILURE;
    }

    Reader *r = Reader_fromFile(a->inputFileName);
    if (r == NULL)
    {
        perror("Input File");
        return EXIT_FAILURE;
    }

    Tokenizer *t = Tokenizer_fromReader(r);
    LabelNode *pt = ParseTree_fromTokenizer(t, a->printTokens);
    if (a->printTokens)
    {
        printf("Enter to continue...");
        getc(stdin);
    }

    Reader_free(r);
    Tokenizer_free(t);

    if (a->printParseTree)
    {
        ParseTree_print(pt);
        printf("Enter to continue...");
        getc(stdin);
    }

    Stack *ps = Stack_new(), *ds = Stack_new();
    bool res = mainloop(pt, ps, ds, a->printStacks, a->stackPrintLength);

    Args_free(a);
    LabelNode_free(pt);
    Stack_free(ps, (void (*)(void *)) ProgramFrame_free);
    Stack_free(ds, (void (*)(void *)) DataFrame_free);

    return res ? EXIT_SUCCESS : EXIT_FAILURE;
}
