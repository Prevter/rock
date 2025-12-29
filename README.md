# rock

**A rock-solid UI component library for cocos2d-x/Geode**

## Contents

- [What's included](#whats-included)
    - [Rounded Rectangles](#rounded-rectangles)
- [Installation](#installation)
- [Roadmap](#roadmap)
- [License](#license)

## What's included

### Rounded Rectangles

#### rock::RoundedRect

A simple rounded rectangle, that is similar to
`cocos2d::CCLayerColor` in usage, but has customizable rounded corners.

Example usage:

```cpp
auto rect1 = rock::RoundedRect::create(
    {255, 0, 0, 255},         // color
    {40.f, 20.f, 60.f, 10.f}, // corner radii
    {200.f, 100.f}            // size
);

auto rect2 = rock::RoundedRect::create(
    {0, 255, 0, 255}, // color
    15.f,             // uniform corner radius
    {150.f, 80.f}     // size
);
```

Radii are represented by `rock::Radii` struct, which holds 4 float values
representing the radius of each corner in the order: top-left, top-right,
bottom-right, bottom-left.

#### rock::RoundedSprite

A rounded rectangle sprite, but with customizable rounded corners.
Shares the same corner radius API as
`rock::RoundedRect`.

Example usage:

```cpp
auto sprite = rock::RoundedSprite::create(
    "groundSquare_15_001.png", // texture file
    {30.f, 10.f, 50.f, 5.f}    // corner radii
);

auto frame = rock::RoundedSprite::createWithSpriteFrameName(
    "myImage.png"_spr, // frame name
    10.f               // uniform corner radius
);
```

**More components coming soon!**

## Installation

Add the following to your `CMakeLists.txt`:
```cmake
CPMAddPackage("gh:prevter/rock#main") # or use a specific version/tag instead of `main`
target_link_libraries(${PROJECT_NAME} rock)
```

Include header(s) from the "rock" folder:
```cpp
#include <rock/RoundedRect.hpp>
```

## Roadmap

**Components:**

- [x] Rounded Rectangle/Sprite
- [ ] DrawNode
- [ ] Label
- [ ] TextInput

**Extras:**

- [ ] Add proper documentation
- [ ] Add example images
- [ ] Demo project showcasing the components

## License

This project is licensed under the MIT License - see
the [LICENSE](LICENSE) file for details