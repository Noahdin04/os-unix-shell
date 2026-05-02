Course Name: Operating Systems (CS 4500)
Group Member Names:
    - Justin Jenkins: 700-727-484
    - Noah Dinwiddie: 700-731-945
    - Olivia Mirley: 700-749-696

To compile:
    gcc myshell.c -o myshell

To run:
    ./myshell

Phase 2 Features Implemented:
    - Foreground command execution
    - Background command execution (trailing &)
    - Output redirection (>)
    - Input redirection (<)
    - Pipes (|)
    - Zombie process cleanup
    - Invalid syntax checking

Known Limitations:
    - Only a single pipe is supported (no chaining, e.g. cmd1 | cmd2 | cmd3)
    - No built-in commands (e.g. cd, export) beyond exit
    - No command history so using up arrows just types ^[[A instead of going to the previous command
