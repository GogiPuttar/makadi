# `makadi`
Trying to make an on-screen pet spider. Let's see where this goes...

## Requirements
```
sudo apt install cmake g++ qt6-base-dev
sudo apt install libgl-dev libglx-dev libopengl-dev mesa-common-dev
sudo apt install libxkbcommon-dev
```

## Instructions
```
cmake -B build -S .
cmake --build build -j
./build/makadi
```