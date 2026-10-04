[CMAKE]

CXX means that this project is using specifically C++ language. By specifying that in the 'project()' it makes it so,
that CMake only checks and configures C++ compiler, instead of trying to go through C compilers.

Setting the CMake standard means that to run this program, a specified version of CMake is required, in this case 4.20
Setting the extensions to 'OFF' also means, that compiler-specific vendor extensions are disabled, and that the code
will follow standard ISO-compliant C++.

-Wall & -Wextra → turns on standard and extra compiler warnings.
-Wpedantic → enforces standard ISO-compliant C++.
-Wconversion → warns during specific data conversions, which might end in loss of said data.

Adding a library of 'kvstore_core' formed of 'src/kvstore.cpp' and 'src/ttl_worker.cpp' combines both files into one
target library.

target_include_directories → tells the compiler where to find '/include' files. Making it public, means that 'kvstore_server'
can inherit the access to these files.

add_executable(a b) → tells CMake to run a binary executable 'a', starting from file 'b' (in this case executable
'kvstore_server' and file 'src/main.cpp').

[HEADER FILES]

Both of the header files are made with pragma preprocessor, to make sure that the compiler includes those files only once per process, and replaces the "#ifdef" macro patterns in both of them.

std::optional → management of optional values that may or may not be represented. 

std::unordered_map 
→ associative container, containing key values.

std::shared_mutex → implementation of reader-writer lock, allowing multiple thread readers of rarely modified data.