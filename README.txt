# Novel shell
CS270 Project 4 - Creating our own Linux shell

### Authors: 
Tanner Palin and Will Shapiro

### `Makefile` Methods


#### Use 'make' to compile shell and 'make clean' to remove the shell's executable.

#### Limitations
This program has limitations for larger inputs as well as extensive background processes. Things such as running gdb and vim in the background have been known to cause issues such as freezing the program.

#### Acceptable command list:

! anyText

The ! command introduces a comment. novsh ignores all text after the ! to the end of the line. The ! must occur as the first token on the line.

variable = value

Here, variable is any reasonable variable name (starting with a letter, continuing with letters or numbers), and the value is a single token. The spaces around the = token are required. The effect of this command is to store the value of the variable inside novsh.

newprompt prompt

Set the shell prompt to prompt, which is a token. The default prompt (including the space at the end) is novsh > .

dir directoryName

This command changes novsh's current directory to directoryName. You do not need to handle dir with no parameters; a shell like bash understands the absense of parameters to mean your home directory. See the getwd(3) and chdir(2) system calls. After this command, any program that novsh starts will run with the new directory as its working directory.

listprocs

This built-in command requests that novsh list all processes running in the background, in any format you like. novsh should keep a list and not rely on external programs like ps to create its output.
bye
