# myshell — A Minimal Unix Shell in C

A small Unix shell built for Operating Systems (CS 4500), implementing the core
process-control and I/O mechanisms a real shell relies on: process creation,
file descriptor manipulation, and inter-process communication via direct
syscalls (`fork`, `execvp`, `pipe`, `dup2`, `waitpid`).

## Authors

Group project by:
- Noah Dinwiddie
- Justin Jenkins
- Olivia Mirley

## Build

​```
gcc myshell.c -o myshell
​```

## Run

​```
./myshell
​```

## Features

- **Foreground execution** via `fork` + `execvp` with parent-side `waitpid`
- **Background execution** — a trailing `&` runs a command without blocking the prompt
- **Pipes** — connects two commands with `|`, wiring `stdout` to `stdin` via `pipe` + `dup2`
- **Input/output redirection** — `<` and `>` remap a child's `stdin`/`stdout` to files
- **Zombie process cleanup** — completed background children reaped each loop with non-blocking `waitpid(..., WNOHANG)`
- **Invalid syntax checking**

## Example

​```
myshell> ls -l | grep .c
myshell> cat < input.txt > output.txt
myshell> sleep 5 &
​```

## Known Limitations

- Only a single pipe is supported (no chaining, e.g. `cmd1 | cmd2 | cmd3`)
- No built-in commands beyond `exit` (e.g. `cd`, `export`)
- No command history — up arrow prints `^[[A` rather than recalling the previous command
