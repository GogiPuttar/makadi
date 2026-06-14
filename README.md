# Makadi

**Makadi** is a lightweight desktop creature framework written in modern C++.

The project provides a transparent desktop overlay that can render animated entities and drive their behavior through configurable, reusable behavior modules. The long-term goal is to support desktop creatures such as spiders, insects, pets, and other interactive agents that can react to user input and their environment.

The name **Makadi** comes from the Hindi word for *spider*.

---

## Features

Current capabilities:

* Transparent desktop overlay rendering
* X11 input passthrough
* Global mouse position tracking
* Configurable entity assets

  * Colored circles
  * PNG image assets
* Configurable behaviors loaded from YAML
* Custom 2D pose representation

  * `Pose2D`
  * `Angle`
* Unit test suite with GoogleTest
* GitHub Actions CI for pull requests

---

## Platform Support

Current support:

| Platform        | Status                     |
| --------------- | -------------------------- |
| Linux (X11)     | Supported                  |
| Linux (Wayland) | Experimental / Unsupported |
| Windows         | Not implemented            |
| macOS           | Not implemented            |

Makadi currently relies on X11 features to support click-through overlays while simultaneously tracking the global mouse position.

---

## Building

### Dependencies

Ubuntu / Pop!_OS:

```bash
sudo apt install \
  build-essential \
  cmake \
  qt6-base-dev \
  yaml-cpp \
  libyaml-cpp-dev \
  libx11-dev \
  libxfixes-dev \
  libxext-dev \
  libxkbcommon-dev \
  libgl-dev \
  libglx-dev \
  libopengl-dev \
  mesa-common-dev
```

### Build

```bash
cmake -B build -S .
cmake --build build -j
```

### Run

```bash
./build/makadi
```

Run with debugging:

```bash
./build/makadi --debug
```

Specify a config:

```bash
./build/makadi \
  --config configs/default.yaml
```

---

## Configuration

Makadi is configured entirely through YAML files.

Example:

```yaml
asset:
  type: image
  path: ../assets/spiders/spider.png
  radius: 48

behavior:
  type: flee_from_pointer_and_turn_away
  flee_radius: 160
  max_speed: 500
  damping: 0.90
  turn_gain: 12.0
```

### Asset Types

#### Circle

```yaml
asset:
  type: circle
  radius: 24
  color: black
```

#### Image

```yaml
asset:
  type: image
  path: ../assets/spiders/spider.png
  radius: 48
```

### Behavior Types

#### Flee From Pointer

```yaml
behavior:
  type: flee_from_pointer
```

Moves away from the mouse cursor.

#### Flee From Pointer And Turn Away

```yaml
behavior:
  type: flee_from_pointer_and_turn_away
```

Moves away from the mouse cursor while rotating to face away from it.

---

## Path Resolution

Resource paths are resolved relative to the configuration file.

Example:

```text
configs/default.yaml
assets/spiders/spider.png
```

```yaml
asset:
  path: ../assets/spiders/spider.png
```

This works regardless of the directory from which Makadi is launched.

---

## Core Types

### Angle

Makadi provides a custom `Angle` type that automatically normalizes values to:

```text
(-π, π]
```

Examples:

```cpp
using namespace makadi::core;

Angle a = Angle::fromDegrees(170);
Angle b = Angle::fromDegrees(30);

Angle c = a + b;
```

All arithmetic operations automatically normalize the result.

### Pose2D

```cpp
Pose2D pose;

pose.x = 100.0;
pose.y = 50.0;
pose.theta = Angle::fromDegrees(90);
```

---

## Project Structure

```text
makadi/
├── assets/
├── configs/
├── include/
├── src/
├── tests/
├── .github/workflows/
└── CMakeLists.txt
```

### Core Library

Most functionality lives inside:

```text
makadi_core
```

This includes:

* Configuration
* Behaviors
* Rendering
* Input handling
* Platform abstractions
* Core math types

The executable itself is intentionally minimal.

---

## Testing

Build and run tests:

```bash
ctest --test-dir build --output-on-failure
```

Current test coverage includes:

* Angle normalization
* Angle arithmetic
* Pose2D
* Path resolution
* YAML configuration parsing
* Behavior execution
* Behavior parameter handling

---

## Continuous Integration

All pull requests automatically run:

1. Configure
2. Build
3. Execute unit tests

A pull request cannot be merged if the test workflow fails.

---