#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_ttf.h>

void moving(SDL_Renderer* renderer, int* RIGHTLEFT, int* UPDOWN, SDL_Rect* rectangle);
void keywordevent(SDL_Event event, bool* down, bool* up, bool* left, bool* right);
void choosedirection(int* RIGHTLEFT, int* UPDOWN, bool* down, bool* up, bool* left, bool* right);
void generateRandomPoint(SDL_Rect* point);
bool checkCollision(SDL_Rect* rectangle, SDL_Rect* point);
void movePoint(SDL_Rect* point);

int main(int argc, char* argv[]) {
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);

  // Create a window
  SDL_Window* window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

  // Create a renderer
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  TTF_Font* font;

  // Initialize the TTF library
  if (TTF_Init() < 0) {
    printf("TTF initialization failed: %s\n", TTF_GetError());
    return 1;
  }

  font = TTF_OpenFont("C:\\Users\\shaha\\CLionProjects\\packman\\Mulish\\static\\Mulish-Black.ttf", 24);
  if (font == NULL) {
    printf("Failed to load font: %s\n", TTF_GetError());
    TTF_Quit();
    return 1;
  }

  // Load the background image
  SDL_Surface* backgroundImage = SDL_LoadBMP("C:\\Users\\shaha\\CLionProjects\\packman\\image2.bmp");
  if (backgroundImage == NULL) {
    printf("Failed to load background image: %s\n", SDL_GetError());
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  // Create a texture from the background image
  SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundImage);
  if (backgroundTexture == NULL) {
    printf("Failed to create background texture: %s\n", SDL_GetError());
    SDL_FreeSurface(backgroundImage);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  // Free the background surface
  SDL_FreeSurface(backgroundImage);

  // Game loop
  bool isRunning = true;
  int RIGHTLEFT = 0;
  int UPDOWN = 550;
  bool up = false;
  bool down = false;
  bool right = false;
  bool left = false;
  SDL_Rect* rectangle = malloc(sizeof(SDL_Rect));
  if (rectangle == NULL) {
    printf("ERROR");
    SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 1;
  }
  SDL_Rect* point = malloc(sizeof(SDL_Rect));
  if (point == NULL) {
    printf("ERROR");
    free(rectangle);
    SDL_DestroyTexture(backgroundTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  srand(time(NULL)); // Seed the random number generator
  generateRandomPoint(point);

  // Score counter
  int score = 0;

  while (isRunning) {
    // Event handling
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        isRunning = false;
        break;
      }
      keywordevent(event, &down, &up, &left, &right);
    }
    choosedirection(&RIGHTLEFT, &UPDOWN, &down, &up, &left, &right);

    // Clear the screen
    SDL_Delay(20);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render the background texture
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    moving(renderer, &RIGHTLEFT, &UPDOWN, rectangle);

    // Draw game objects here
    // ...

    // Generate a random point

    // Check if the rectangle touches the point
    if (checkCollision(rectangle, point)) {
      // Move the point to another place
      movePoint(point);

      // Increment the score
      score++;
    }

    // Render the point
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, point);

    // Print the score
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);

    SDL_Color textColor = { 255, 255, 255 }; // White color
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    if (textSurface == NULL) {
      printf("Failed to render text surface: %s\n", TTF_GetError());
      SDL_DestroyTexture(backgroundTexture);
      TTF_CloseFont(font);
      TTF_Quit();
      SDL_Quit();
      return 1;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
      printf("Failed to create text texture: %s\n", SDL_GetError());
      SDL_FreeSurface(textSurface);
      SDL_DestroyTexture(backgroundTexture);
      TTF_CloseFont(font);
      TTF_Quit();
      SDL_Quit();
      return 1;
    }

    SDL_Rect textRect;
    textRect.x = 10;
    textRect.y = 10;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;

    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);

    // Update the screen
    SDL_RenderPresent(renderer);
  }

  // Cleanup and quit
  SDL_DestroyTexture(backgroundTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();

  free(rectangle);
  free(point);

  return 0;
}

void moving(SDL_Renderer* renderer, int* RIGHTLEFT, int* UPDOWN, SDL_Rect* rectangle) {
  if (*RIGHTLEFT >= 800) {
    *RIGHTLEFT = 0;
  }
  if (*RIGHTLEFT < 0) {
    *RIGHTLEFT = 800;
  }
  if (*UPDOWN >= 600) {
    *UPDOWN = 0;
  }
  if (*UPDOWN < 0) {
    *UPDOWN = 600;
  }
  rectangle->x = *RIGHTLEFT;
  rectangle->y = *UPDOWN;
  rectangle->w = 20;
  rectangle->h = 20;
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  SDL_RenderFillRect(renderer, rectangle);
}

void choosedirection(int* RIGHTLEFT, int* UPDOWN, bool* down, bool* up, bool* left, bool* right) {
  if (*down == true) {
    *UPDOWN += 10;
  }
  if (*up == true) {
    *UPDOWN -= 10;
  }
  if (*left == true) {
    *RIGHTLEFT -= 10;
  }
  if (*right == true) {
    *RIGHTLEFT += 10;
  }
}

void keywordevent(SDL_Event event, bool* down, bool* up, bool* left, bool* right) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_DOWN) {
      *down = true;
      *up = false;
      *left = false;
      *right = false;
    }
    if (event.key.keysym.sym == SDLK_UP) {
      *up = true;
      *down = false;
      *left = false;
      *right = false;
    }
    if (event.key.keysym.sym == SDLK_RIGHT) {
      *right = true;
      *up = false;
      *down = false;
      *left = false;
    }
    if (event.key.keysym.sym == SDLK_LEFT) {
      *left = true;
      *up = false;
      *right = false;
      *down = false;
    }
  }
}

void generateRandomPoint(SDL_Rect* point) {
  int maxX = 780;
  int maxY = 580;
  point->x = rand() % maxX + 10;
  point->y = rand() % maxY + 10;
  point->w = 10;
  point->h = 10;
}

bool checkCollision(SDL_Rect* rectangle, SDL_Rect* point) {
  if (rectangle->x < point->x + point->w &&
      rectangle->x + rectangle->w > point->x &&
      rectangle->y < point->y + point->h &&
      rectangle->y + rectangle->h > point->y) {
    return true; // Collision detected
  }
  return false; // No collision
}

void movePoint(SDL_Rect* point) {
  int maxX = 780;
  int maxY = 580;
  point->x = rand() % maxX + 10;
  point->y = rand() % maxY + 10;
  point->w = 10;
  point->h = 10;
}
