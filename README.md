# rmcmt(1) -- remove comments

## SYNOPSIS
```bash
rmcmt  # no options, no args
```

## DESCRIPTION
Remove comments.
Read from standard input, and write to standard output.
Implement using finite-state machine (FSM).

## SUPPORTED TYPE OF COMMENTS
This command recognizes following type of comments.
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
