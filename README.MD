run task window:
"args": [
    "-I",
    "${workspaceFolder}/include/",
    "${workspaceFolder}/src/*.cpp",
    "${workspaceFolder}/main.cpp",
    "-o",
    "${workspaceFolder}/bin/main.exe",
    "-lws2_32"
],

run task linux:
"args": [
    "-I",
    "${workspaceFolder}/include/",
    "${workspaceFolder}/src/*.cpp",
    "-o",
    "${workspaceFolder}/bin/main",
],