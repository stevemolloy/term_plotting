# Quick start

```bash
$ make
clang -Wall -Wpedantic -Wextra -std=c11 -ggdb -Wshadow -fsanitize=address  -c src/lib.c -o objs/lib.o
clang -Wall -Wpedantic -Wextra -std=c11 -ggdb -Wshadow -fsanitize=address  -c src/main.c -o objs/main.o
clang -Wall -Wpedantic -Wextra -std=c11 -ggdb -Wshadow -fsanitize=address objs/lib.o objs/main.o -o bin/plot_histogram
$ ./bin/plot_histogram


+1.4e+01 ^   X
         |
         |                  X
         |                 X
         |X               X
         |               X
         |     X        X
         |    X        X
         | X          X
         |           X
         |          X
         |         X
         |        X
         |       X
+0.0e+00 ---X---X----------->
```
