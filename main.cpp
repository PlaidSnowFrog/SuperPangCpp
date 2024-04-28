#include "raylib.h"
#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <ctime>

class Bullet {
public:
  int x;
  int y;
  int w = 25;
  int h = 25;
  int speed = 13;
  bool ex;

  Bullet(int x, int y, bool ex) {
    this->x = x;
    this->y = y;
    this->ex = ex;
  }

  void Move(Bullet &bullet) { bullet.y -= bullet.speed; }

  void Check(Bullet &bullet) {
    if (bullet.y == 0) {
      bullet.ex = false;
    }
  }
};

class Player {
public:
  int x;
  int y;
  int w = 50;
  int h = 50;
  int speed = 10;
  unsigned int life;

  // Constructor
  Player(int x, int y, unsigned int life) {
    this->x = x;
    this->y = y;
    this->life = life;
  }

  // Functions
  void Move(const int screenWidth) {
    if (IsKeyDown(KEY_A)) {
      this->x -= this->speed;
    }
    if (IsKeyDown(KEY_D)) {
      this->x += this->speed;
    }
  }

  void Gravity(Player &player, int ground) {
    if (player.y > ground) {
      player.y += player.speed;
    }
  }
};

class Bubble {
public:
  int x;
  int y;
  int w;
  int h;
  int speed = 5;
  unsigned int size;
  bool directionX;
  bool directionY;
  bool ex;

  // Constructor
  Bubble(int x, int y, unsigned int size) {
    this->x = x;
    this->y = y;
    this->size = size;

    ex = true;
  }

  void Update() {
    switch (this->size) {
    case 1:
      this->w = 15;
      this->h = 15;
      break;
    case 2:
      this->w = 30;
      this->h = 30;
      break;
    case 3:
      this->w = 50;
      this->h = 50;
      break;
    }
  }

  void Move(int screenWidth) {
    if (this->x == screenWidth) {
      this->directionX = false;
    } else if (this->x == 0) {
      this->directionX = true;
    }
    switch (this->directionX) {
    case true:
      this->x += this->speed;
      break;
    case false:
      this->x -= this->speed;
      break;
    }
  }

  void Gravity(int screenHeight) {
    if (this->y == 0) {
      this->directionY = true;
    } else if (this->y == screenHeight) {
      this->directionY = false;
    }

    switch (directionY) {
    case true:
      this->y += this->speed;
      break;
    case false:
      this->y -= this->speed;
      break;
    }
  }
};

bool checkCollision(const Bubble rect1, const Bullet rect2);
bool checkCollision(const Player rect1, const Bubble rect2);

int main(void) {
  srand(time(NULL));

  const int screenWidth = 800;
  const int screenHeight = 450;

  bool isGameOver = false;
  bool hasPlayerWon = false;
  Bullet bullet(0, 0, false);
  Player player(0, screenHeight - 50, 10);
  Bubble bubbles[] = {
      Bubble(0, 0, (rand() % 3) + 1),     Bubble(60, 60, (rand() % 3) + 1),
      Bubble(120, 120, (rand() % 3) + 1), Bubble(180, 180, (rand() % 3) + 1),
      Bubble(240, 240, (rand() % 3) + 1), Bubble(300, 300, (rand() % 3) + 1),
      Bubble(360, 360, (rand() % 3) + 1), Bubble(420, 420, (rand() % 3) + 1)};

  InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

  SetTargetFPS(60);

  // Game loop
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(LIGHTGRAY);

    if (!isGameOver) {
      DrawRectangle(player.x, player.y, player.w, player.h, BLUE);

      for (int i = 0; i < 8; i++) {
        if (bubbles[i].ex) {
          bubbles[i].Update();
          DrawRectangle(bubbles[i].x, bubbles[i].y, bubbles[i].w, bubbles[i].h,RED);
          bubbles[i].Move(screenWidth);
          bubbles[i].Gravity(screenHeight);
        }
      }

      if (bullet.ex) {
        DrawRectangle(bullet.x, bullet.y, bullet.w, bullet.h, GREEN);
        bullet.Move(bullet);
      }
      bullet.Check(bullet);

      if (IsKeyDown(KEY_SPACE)) {
        bullet.x = player.x;
        bullet.y = player.y;
        bullet.ex = true;
      }

      player.Gravity(player, screenHeight);
      player.Move(screenWidth);

      for (int i = 0; i < 8; i++) {
        if (bubbles[i].ex && checkCollision(player, bubbles[i])) {
          hasPlayerWon = false;
          isGameOver = true;
        }

        if (bullet.ex && checkCollision(bubbles[i], bullet)) {
          bullet.ex = false;
          if (bubbles[i].ex && bubbles[i].size == 1) {
            bubbles[i].ex = false;
          } else if (bubbles[i].ex && bubbles[i].size > 1) {
            bubbles[i].size--;
          }
        }
      }

      if (!bubbles[0].ex && !bubbles[1].ex && !bubbles[2].ex && !bubbles[3].ex && !bubbles[4].ex && !bubbles[5].ex && !bubbles[6].ex && !bubbles[7].ex) {
        hasPlayerWon =  true;
        isGameOver = true;
      }

    } else if (!hasPlayerWon) {
      DrawText("Game Over!", screenWidth / 2, screenHeight / 2, 20, BLACK);
    } else if (hasPlayerWon) {
      DrawText("You Won!", screenWidth / 2, screenHeight / 2, 20, BLACK);
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}

bool checkCollision(const Player rect1, const Bubble rect2) {
  // Check if rect1 is to the left of rect2
  if (rect1.x + 50 < rect2.x) {
    return false;
  }

  // Check if rect1 is to the right of rect2
  if (rect1.x > rect2.x + 50) {
    return false;
  }

  // Check if rect1 is above rect2
  if (rect1.y + 50 < rect2.y) {
    return false;
  }

  // Check if rect1 is below rect2
  if (rect1.y > rect2.y + 50) {
    return false;
  }

  // If none of the above conditions are true, rectangles overlap and there is a
  // collision
  return true;
}

bool checkCollision(const Bubble rect1, const Bullet rect2) {
  // Check if rect1 is to the left of rect2
  if (rect1.x + 50 < rect2.x) {
    return false;
  }

  // Check if rect1 is to the right of rect2
  if (rect1.x > rect2.x + 50) {
    return false;
  }

  // Check if rect1 is above rect2
  if (rect1.y + 50 < rect2.y) {
    return false;
  }

  // Check if rect1 is below rect2
  if (rect1.y > rect2.y + 50) {
    return false;
  }

  // If none of the above conditions are true, rectangles overlap and there is a
  // collision
  return true;
}
