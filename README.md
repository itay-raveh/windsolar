# WINDSOLAR

**WINDSOLAR** is a toy language based on [GASOIL](https://esolangs.org/wiki/GASOIL), created for educational purposes
as my first C project.

It's a stack based language, with a `ProgramStack` (`PS`) containing instructions, and a `DataStack` (`DS`)
containing `STRING`s and `NUMBER`s.

A **WINDSOLAR** script is comprised of named subroutines, such as:

```
MAIN ("Hello, World!\n"; WRITE)
```

Execution starts at the `MAIN` subroutine, and from there `CALL` commands add more of them.

More functions can be found under `Guides/Commands.md`. They are limited, 
but I have found working in this simplistic form a fun little challenge.

Feel free to add interesting examples, and feel even more free to suggest C improvements :) 
