# rmcmt(1) -- remove comments

## SYNOPSIS
```bash
rmcmt_slaster  # no options, no args
```

## DESCRIPTION
Remove comments.
Read from standard input, and write to standard output.
Implement using finite-state machine (FSM).

## SUPPORTED TYPE OF COMMENTS
rmcmt_slaster recognizes following type of comments. The suffix "_slaster" stands for "slash" + "asterisk".
```cpp
/* C-Style Comments */
```

## BUILD
Just run `make`.
```bash
$ make
```

## TEST
run `make` with argument "test".
```bash
$ make test
```

## EXAMPLE
Issue a command like this:
```bash
$ cat a.c | ./rmcmt > comment_removed.c
```
