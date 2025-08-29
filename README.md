# 2x3-2x2Tablegen
a tablegen and tablebase for 2x3 and 2x2 2048
download main.cpp and turn it into an exe. for windows:
Using MinGW-w64 on Windows

Install MinGW-w64

Download from: https://www.mingw-w64.org/

Make sure to include g++ and add MinGW-w64 bin folder to your PATH.

Install SDL2 and SDL2_ttf for Windows

Go to: https://www.libsdl.org/download-2.0.php

Get the development libraries for MinGW.

Extract them to a folder, e.g., C:\SDL2.

You will have include/ and lib/ folders. Copy SDL2.dll and SDL2_ttf.dll from lib/ or bin/ to your project folder.

Open Command Prompt in your project folder and run:
g++ src\main.cpp -o bin\tablegen.exe -IC:\SDL2\include -LC:\SDL2\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++
Copy SDL2.dll and SDL2_ttf.dll to the same folder as tablegen.exe.

Run:
tablegen.exe
___________________________________________
For linux (debian or debian based)
open terminal
run "nano installtablegen.sh"
copy and paste:
#!/bin/bash
# Build tablegen.exe for Windows (x86_64) using MinGW-w64

# 1️⃣ Set variables
SRC="src/main.cpp"
OUT="bin/tablegen.exe"
DLL_DIR="/usr/x86_64-w64-mingw32/bin"

mkdir -p bin

# 2️⃣ Compile
x86_64-w64-mingw32-g++ "$SRC" -o "$OUT" -lSDL2main -lSDL2 -lSDL2_ttf -static-libgcc -static-libstdc++

# 3️⃣ Copy required DLLs
cp "$DLL_DIR/SDL2.dll" bin/
cp "$DLL_DIR/SDL2_ttf.dll" bin/

echo "Build complete! Run with Wine:"
echo "wine $OUT"
_____
then do ctrl+o, enter and ctrl+x
then still in terminal run:
chmod +x installtablegen.sh
run with:
./installtablegen.sh
It will produce bin/tablegen.exe along with SDL2.dll and SDL2_ttf.dll.
