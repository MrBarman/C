# Replica of 'wc' command in linux
**Here I am trying to create a replica of 'wc' command on linux.<br/>**
**Basic info about 'wc'command is as follows -** 

Usage: wc [OPTION]... [FILE]...<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; or:  wc [OPTION]... --files0-from=F</br>
Print newline, word, and byte counts for each FILE, and a total line if</br>
more than one FILE is specified.  A word is a non-zero-length sequence of</br>
characters delimited by white space.</br>

With no FILE, or when FILE is -, read standard input.</br>

The options below may be used to select which counts are printed, always in</br>
the following order: newline, word, character, byte, maximum line length.</br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -c, --bytes            print the byte counts</br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -m, --chars            print the character counts</br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -l, --lines            print the newline counts</br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;    --files0-from=F    read input from the files specified by</br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;NUL-terminated names in file F;</br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;If F is - then read names from standard input</br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -L, --max-line-length  print the maximum display width</br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; -w, --words            print the word counts</br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     --help     display this help and exit</br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;     --version  output version information and exit</br>

GNU coreutils online help: <http://www.gnu.org/software/coreutils/></br>
Full documentation at: <http://www.gnu.org/software/coreutils/wc></br>
or available locally via: info '(coreutils) wc invocation'</br>
