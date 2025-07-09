# linux-dsh-extension
Assignment for the "Operating Systems and Networks" course (2nd Year Bachelor's in Computer Science, Bicocca University).

This project is a basic shell implementation that offers essential command-line functionalities. It allows users to execute programs, manage input/output, and work with custom environment variables.

---

## Key Features

* **Command Execution**: The shell can run external programs. It intelligently finds commands by searching directories listed in a `PATH` environment variable, much like standard shells do.
* **I/O Redirection**:
    * `>`: Redirects a command's output to a specified file, overwriting existing content.
    * `>>`: Appends a command's output to a specified file.
* **Piping (`|`)**: Connects the output of one command to the input of another, enabling complex command sequences.

---

## Environment Variable Management

The shell includes built-in commands for handling custom environment variables:

* **`set <VARIABLE_NAME> <VALUE>`**:
    * Lets users define or update custom shell variables.
    * Special handling for the `PATH` variable allows users to modify the directories where the shell looks for executables.
    * If a variable already exists, its value is updated; otherwise, a new variable is created.
    * Calling `set <VARIABLE_NAME>` without a value will unset (remove) the variable.
* **`echo $<VARIABLE_NAME>`**:
    * Prints the value of a specified environment variable.
    * Supports retrieving values for both the special `PATH` variable and any custom variables you've set.
