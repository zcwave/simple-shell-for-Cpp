# Simple Unix shell for cpp
 
# Build

```bash
# Current dictory is simple-shell-for-cpp/
cd simple-shell-for-cpp
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 4
```

# Functionality showcase

## `Usage`

```bash
➜  simple-shell-for-cpp git:(mini) ./build/TShell -h
Usage: shell [-hvp]
   -h   print this message
   -v   print additional diagnostic information
   -p   do not emit a command prompt
```

## Function
```bash
➜  simple-shell-for-cpp git:(mini) ./build/TShell
tsh> ls
ls: Command not found. 
tsh> /bin/ls 
CMakeLists.txt  README.md  include  src
LICENSE         build      lib
tsh> /bin/ls -l
total 28
-rwxr-xr-x 1 zcwave zcwave  782 Apr  3 21:56 CMakeLists.txt
-rw-r--r-- 1 zcwave zcwave 1494 Mar 29 10:00 LICENSE
-rw-r--r-- 1 zcwave zcwave  462 Apr  3 22:05 README.md
drwxr-xr-x 4 zcwave zcwave 4096 Apr  3 21:56 build
drwxr-xr-x 2 zcwave zcwave 4096 Apr  3 21:49 include
drwxr-xr-x 2 zcwave zcwave 4096 Apr  3 21:40 lib
drwxr-xr-x 2 zcwave zcwave 4096 Apr  3 21:49 src
tsh> /bin/echo hello
hello
tsh> ⏎     # using Ctrl + D(EOF).
```
