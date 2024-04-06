# Simple Unix shell for cpp

- [x] branch mini: 一个最小实现，不支持Jobs control. (done)
- [x] branch main: 完整实现，支持Jobs control. (doing)

# Build


```bash
cd simple-shell-for-cpp
# Current dictory is simple-shell-for-cpp/
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 4
```


# Usages ???? 

```bash
tsh> /bin/sleep 200
^ZJob [1] (39479) stopped by signal 20
tsh> jobs
[1] (39479) Foreground /bin/sleep 200
tsh> /bin/sleep 200 &
[2] (39785) /bin/sleep 200 &
tsh> jobs
[1] (39479) Foreground /bin/sleep 200
[2] (39785) Running /bin/sleep 200 &
tsh> /bin/ls
CMakeLists.txt  LICENSE  README.md  build  include  lib  src
```
