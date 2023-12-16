# The comment remover
the comment remover of source code files, using finite-state machine (FSM).

## Build
Run `make` within the project directory to build the project.
```bash
make
```

## Usage
Input is read from `stdin` and output is written to `stdout`.
Issue a command like this:
```bash
cat some.c | rmcmt > comment_removed.c
```
