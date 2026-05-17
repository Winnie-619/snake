
---

## **5️⃣ SnakeGame.h**

```cpp
#pragma once
#include <vector>
#include <queue>
#include <ftxui/component/component.hpp>

struct Position {
    int x;
    int y;
};

enum Direction { UP, DOWN, LEFT, RIGHT };

class SnakeGame {
public:
    SnakeGame(int width, int height);
    void Update();
    void ChangeDirection(Direction dir);
    ftxui::Element Render();

private:
    int width_;
    int height_;
    std::vector<Position> snake_;
    Position food_;
    Direction dir_;
    bool grow_ = false;
    bool game_over_ = false;
    bool speed_boost_ = false;

    void Move();
    void GenerateFood();
    bool Collision(Position p);
};