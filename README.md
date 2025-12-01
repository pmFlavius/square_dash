# square_dash
A Geometry Dash clone just for fun to try new skills .

## ✨ Features

- **Player Movement**: Jump mechanics with gravity physics
- **Procedural Obstacles**: Randomly generated obstacles with varying heights and spacing
- **Texture System**: Custom textures for player and obstacles using `shared_ptr` for memory efficiency
- **Collision Detection**: Accurate hit detection

## 🛠️ Technical Highlights

### Smart Obstacle Spawning
- Fixed spawn logic to track rightmost obstacle position
- Ensures proper spacing between obstacles
- Prevents overlapping and off-screen spawns

### Texture Management
- Uses `shared_ptr<Texture>` to prevent texture invalidation
- Loads 4 obstacle textures once at initialization
- Textures shared across multiple obstacles without copying

## 🎯 Classes

- **`Game`**: Main game loop, window management
- **`Player`**: Character physics, jumping, collision bounds
- **`Obstacle`**: Moving obstacles with textures
- **`Level`**: Obstacle spawning, collision detection, game state

## 🚀 How to Build

g++ -std=c++17 main.cpp functii.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o game
./game
