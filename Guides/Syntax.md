WINDSOLAR runtime is comprised of a `DataStack (DS)` and a `ProgramStack (PS)`.

A script is comprised of subroutines, each containing a list of instructions.
On execution, any `STRING` or `NUMBER` is the `PS` is pushed to the `DS`, and any command is executed.

```
MAIN ("Hello, World!\n"; WRITE)
```

In this script for example, in the subroutine `MAIN`,
the `STRING` will be pushed to the `DS`,
then the command `WRITE` is executed,
which will pop a `STRING` from the `DS` and print it to stdout.

Thus, this script will print out `Hello, World!`.

See the `Commands.md` file for mor commands and `Examples` folder to see how to use them.
