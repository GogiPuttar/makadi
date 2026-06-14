# `makadi`
Trying to make an on-screen pet spider. Let's see where this goes...

## Requirements
```
sudo apt install cmake g++ qt6-base-dev
sudo apt install libgl-dev libglx-dev libopengl-dev mesa-common-dev
sudo apt install libxkbcommon-dev
sudo apt install libx11-dev libxfixes-dev libxext-dev
sudo apt install libyaml-cpp-dev
```

```
sudo apt install \
  cmake \
  g++ \
  qt6-base-dev \
  libgl-dev \
  libglx-dev \
  libopengl-dev \
  mesa-common-dev \
  libxkbcommon-dev \
  libx11-dev \
  libxfixes-dev \
  libxext-dev \
  libyaml-cpp-dev \
  libgtest-dev
```

## Instructions
```
cmake -B build -S .
cmake --build build -j
./build/makadi
```