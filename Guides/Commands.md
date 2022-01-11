## Index

`#1` is at the top of the `DS`, `#2` is below and so on.

## Flow Control

`CALL` - Load instructions from a subroutine named `#1 STRING` into the `PS`.

`CCALL` - Conditional CALL. If `#2 NUMBER` is truthy (non-zero), `CALL` on `#1 STRING`.

`BRANCH` - If `#3 NUMBER` is truthy `CALL` on `#2 STRING`, else `CALL` on `#1 STRING`.

## Arithmetic

`ADD`, `SUB`, `MUL`, `DIV`, `MOD` - Calculate `#2 NUMBER` `OP` `#1 NUMBER` and push back the result.

## Logic

`NOT` - Push back the boolean opposite of `#1 NUMBER`.

`EQ`, `NE`, `GT`, `GE`, `LT`, `LE`, `AND`, `OR` - Calculate `#2 NUMBER` `OP` `#1 NUMBER` and push back the result.

## Stack

`POP` - Remove `#1` from the `DS`.

`DUP` - Push back a copy of `#1`.

`DUP2` - Push bach copies of `#1` and `#2`.

`SWAP12` - Swap `#1` and `#2`.

`SWAP13` - Swap `#1` and `#3`.

## IO

`WRITE` - Write `#1` to stdout.

`READ` - Read a `STRING` from the user and push it back.

## System

`SLEEP` - Sleep for `#1 NUMBER` seconds.
