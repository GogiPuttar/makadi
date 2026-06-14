# Makadi

**Makadi** is a lightweight desktop creature framework written in modern C++.

The project provides a transparent desktop overlay that can render animated entities and drive their behavior through configurable, reusable behavior modules. The long-term goal is to support desktop creatures such as spiders, insects, pets, and other interactive agents that can react to user input and their environment.

The name **Makadi** comes from the Hindi word for *spider*.
The goal is to make this crawl around your screen like your own pet spider!

<br>

https://github.com/user-attachments/assets/1578430e-ec74-4840-8666-a37ad66dc261

<br>

---

## Features

Current capabilities:

* Transparent desktop overlay rendering
* X11 click-through overlay support
* Global mouse position tracking
* Static and animated sprite assets
* YAML-driven configuration
* Multiple configurable behaviors
* Speed-driven animation playback
* Custom 2D math types (`Pose2D`, `Angle`)
* Unit testing with GoogleTest
* GitHub Actions continuous integration

---

## Platform Support

| Platform        | Status       |
| --------------- | ------------ |
| Linux (X11)     | Supported    |
| Linux (Wayland) | Experimental |
| Windows         | Planned      |
| macOS           | Planned      |

Makadi currently relies on X11 features to support click-through overlays while simultaneously tracking global mouse position.

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

Run with debugging enabled:

```bash
./build/makadi --debug
```

Specify a configuration file:

```bash
./build/makadi --config configs/default.yaml
```

---

## Configuration

Makadi is configured entirely through YAML files.

Example:

```yaml
asset:
  type: image
  path: ../assets/images/tarantula.png
  radius: 64

behavior:
  type: flee_from_pointer_and_turn_away

  flee_radius: 60
  safe_radius: 160

  min_speed: 300
  max_speed: 1000

  turn_gain: 12.0
  max_turn_speed: 12.0

  velocity_tracking_gain: 12.0

  pointer_filter_alpha: 0.15

animation:
  walking:
    frames_folder: ../assets/animation/tarantula_walking_default
    speed_to_fps: 0.08
    min_fps: 10
    max_fps: 80
```

---

## Assets

### Circle

```yaml
asset:
  type: circle
  radius: 24
  color: black
```

### Image

```yaml
asset:
  type: image
  path: ../assets/images/tarantula.png
  radius: 64
```

Paths are resolved relative to the configuration file location.

---

## Behaviors

### flee_from_pointer

Moves directly away from the mouse pointer.

### flee_from_pointer_and_turn_away

A more natural spider-like response:

* Becomes "spooked" inside `flee_radius`
* Continues fleeing until outside `safe_radius`
* Turns to face away from the pointer
* Enforces configurable minimum movement speed
* Uses velocity tracking to avoid orbiting
* Supports configurable pointer low-pass filtering

---

---

## Path Resolution

Resource paths are resolved relative to the configuration file.

Example:

```text
configs/default.yaml
assets/images/tarantula.png
```

```yaml
asset:
  path: ../assets/images/tarantula.png
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

## Animation

Makadi supports animated sprite sequences.

Frames are loaded from a directory and automatically sorted alphabetically:

```text
walk_00.png
walk_01.png
walk_02.png
...
walk_07.png
```

Animation playback speed is driven by entity movement speed:

```text
fps = clamp(
  speed * speed_to_fps,
  min_fps,
  max_fps)
```

When an entity stops moving, the last animation frame is preserved to create a natural stepping pose instead of snapping back to an idle image.

---

## Architecture

```text
makadi_core
├── Configuration
├── Behaviors
├── Animation
├── Rendering
├── Input
├── Platform
└── Core math types
```

The executable itself contains only application startup and configuration loading logic.

---

## Testing

Run all tests:

```bash
ctest --test-dir build --output-on-failure
```

Current coverage includes:

* Angle normalization and arithmetic
* Pose2D
* Path resolution
* YAML configuration parsing
* Animated sprite loading
* Animation frame selection
* Flee behavior
* Turn-away behavior
* Hysteresis (`flee_radius` / `safe_radius`)
* Pointer filtering
* Velocity tracking
* Heading offsets

---

## Continuous Integration

Every pull request automatically:

1. Configures the project
2. Builds the project
3. Runs the complete test suite

Pull requests must pass CI before being merged.
