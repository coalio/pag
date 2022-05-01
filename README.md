# pag\[inate\]

Allows you to paginate through a list of lines

## Usage

```
$ pag -h
Usage: ./pag [options] <file>
Options:
  -h, --help                            Display this help message
  -m, --maxlines <lines>                Set how many lines per page will be displayed
  -s, --startline <line>                Set the line to start from
  -e, --endline <line>                  Set the line to end at
  -l, --maxlength <length>              Set the max length of the line
```

`-h [--help]` will display the help message.

`-m [--maxlines] <lines>` will set how many lines per page will be displayed (default 10).

`-s [--startline] <line>` will set the line to start from (default 0).

`-e [--endline] <line>` will set the line to end at (default -1 | last line).

`-l [--maxlength] <length>` will set the max length of the line (default -1 | end of line).

## Install

To compile this utility, you can simply run `make`. After compiling, you can install it using `sudo make install`.

## Can I use pipes with this?

No.

The reason for this is not that I am lazy (I am lazy), but because it's a design decision. Also not my decision, but of the Unix standard. Pipelines stream the output (std::cout) to the input of the following program (std::cin). This means `cin` only listens to the pipe, therefore rendering terminal interaction unusable.

It is technically possible to use `pag` with pipes, but it's not recommended, and it's not even possible without re-opening the terminal input device (tty or CONIN$).

If you wish to read the output of a process in `pag`, you can use `>` to write the output to a file and then use `pag` to read the file.

```
$ echo "Hello World" > file.txt && pag file.txt
```
