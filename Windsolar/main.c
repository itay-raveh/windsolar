#include <stdio.h>
#include <stdlib.h>
#include "args.h"
#include "runtime.h"
#include "frames.h"


const char *usage = "windsolar FILENAME [-t] [-p] [-s FRAMES]\n"
                    "\n"
                    "A simple, stack-based, toy scripting language.\n"
                    "See https://github.com/itay-raveh/windsolar for syntax and more.\n"
                    "\n"
                    "-t, --print-tokens          While tokenizing the file, print tokens, then wait for user to confirm.\n"
                    "-p, --print-parse-tree      After parsing the file, print the parse-tree, then wait for user to confirm.\n"
                    "-s, --print-stacks          Before each instruction is executed, print both stacks, up to FRAMES frames.\n";


int windsolar(int argc, char *argv[])
{
    Args *a = Args_fromArgv(argc, argv);
    if (a == NULL)
    {
        fputs(usage, stderr);
        Args_free(a);
        return EXIT_FAILURE;
    }

    Reader *r = Reader_fromFile(a->inputFileName);
    if (r == NULL)
    {
        perror("Input File");
        Args_free(a);
        Reader_free(r);
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

int main(int argc, char *argv[])
{
    return windsolar(argc, argv);
}
