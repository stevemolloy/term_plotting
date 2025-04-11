# Build

```bash
$ make
clang -Wall -Wpedantic -Wextra -std=c11 -ggdb -Wshadow -fsanitize=address  -c src/lib.c -o objs/lib.o
clang -Wall -Wpedantic -Wextra -std=c11 -ggdb -Wshadow -fsanitize=address  -c src/main.c -o objs/main.o
clang -Wall -Wpedantic -Wextra -std=c11 -ggdb -Wshadow -fsanitize=address objs/lib.o objs/main.o -o bin/plot_histogram
```

# Run

```bash
$ ./bin/plot_histogram ./data/brm40_short.csv -h 150 -v 20


./data/brm40_short.csv

+5.38e-01 ^                   X            X
          |                  X X       XXXX XX XXXX
          |                     X  X  X       X    X
          |                      XX X               X
          |                          X               X    XXXX
          |               XXX                          XXX          XX
          |                                           X       X    X                                         X
          |              X                                     X      X
          |             X                                       XXX                                         X
          |                                                            X  X      XX
          |                                                              X X    X                    XX    X
          |            X                                                X      X   X                   XXXX
          |                                                                                        XX
          |     XX  X                                                       X X     XX            X
          |    X   X XX                                                               X         XX
          |X      X                                                          X         X
          |                                                                             XX    XX
          | XXX                                                                           X  X
          |                                                                                X
+5.35e-01 ----------------------------------------------------------------------------------X---------------->
```
