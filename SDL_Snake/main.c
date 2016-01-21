
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

// directions
#define UP 0
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define TILE_W 32
#define TILE_H 32

typedef struct
{
    int xPos, yPos;
    unsigned short direction;
    
} SnakeBlock;

typedef struct
{
    int x,y;
    int xPos, yPos;
    int size;
    SnakeBlock *blocks;
    
    unsigned short direction;
} SnakeHead;

typedef struct
{
    int x,y;
} Bait;

unsigned char field[100] = {0};

bool processEvents(SDL_Window *window, SnakeHead *snake);
void render(SDL_Renderer *gfx, SnakeHead *snake);
void initGame();
void update(SnakeHead *snake);
void renderField(SDL_Renderer *gfx);
void renderSnake(SDL_Renderer *gfx, SnakeHead *snake);

int main(int argc, const char * argv[]) {
    bool is_runnung = true;
    SnakeHead snake;
    snake.xPos = 10;
    snake.yPos = 9;
    snake.size = 3;
    snake.direction = UP;
    
    SnakeBlock blocks[50] = {{10, snake.yPos+1, UP}, {10, snake.yPos+2, UP}, {10, snake.yPos+3, UP}};
    snake.blocks = blocks;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    window = SDL_CreateWindow("My SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    
    while (is_runnung) {
        is_runnung = processEvents(window, &snake);
        update(&snake);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Render
        renderField(renderer);
        renderSnake(renderer, &snake);
        
        SDL_RenderPresent(renderer);
        
        SDL_Delay(300);
    }
    
    // Shutdown

    SDL_DestroyRenderer(renderer);
    
    SDL_Quit();
    return 0;
}

bool processEvents(SDL_Window *window, SnakeHead *snake)
{
    SDL_Event event;
    bool is_running = true;
    
    // Check for events
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_WINDOWEVENT_CLOSE:
                SDL_DestroyWindow(window);
                window = NULL;
                is_running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        is_running = false;
                        break;
                    case SDLK_w:
                        snake->direction = UP;
                        break;
                    case SDLK_a:
                        snake->direction = LEFT;
                        break;
                    case SDLK_s:
                        snake->direction = DOWN;
                        break;
                    case SDLK_d:
                        snake->direction = RIGHT;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                is_running = false;
                break;
        }
        
        
    }
    
    return is_running;
}

void update(SnakeHead *snake)
{
    snake->blocks[0].xPos = snake->xPos;
    snake->blocks[0].yPos = snake->yPos;
    snake->blocks[0].direction = snake->direction;
    
    for (int i = 1; i < snake->size; i++) {
        snake->blocks[i].direction = snake->blocks[i-1].direction;
        
        snake->blocks[i].xPos = snake->blocks[i-1].xPos;
        snake->blocks[i].yPos = snake->blocks[i-1].yPos;
        
        switch (snake->blocks[i].direction) {
            case UP:
                snake->blocks[i].yPos += 1;
                break;
            case DOWN:
                snake->blocks[i].yPos -= 1;
                break;
            case LEFT:
                snake->blocks[i].xPos += 1;
                break;
            case RIGHT:
                snake->blocks[i].xPos -= 1;
                break;
            default:
                break;
        }

    }

    switch (snake->direction) {
        case UP:
            snake->yPos -= 1;
            break;
        case DOWN:
            snake->yPos += 1;
            break;
        case LEFT:
            snake->xPos -= 1;
            break;
        case RIGHT:
            snake->xPos += 1;
            break;
        default:
            break;
    }
    
}

void renderField(SDL_Renderer *gfx)
{
    SDL_SetRenderDrawColor(gfx, 128, 128, 128, 128);
    int x = 0;
    int y = 0;
    SDL_Rect rect = {0, 0, TILE_H, TILE_W};
    
    for (int i = 0; i < 300; i++) {
        if (i != 0 && i % 20 == 0){
            x = 0;
            y += TILE_H;
        }
        rect.x = x;
        rect.y = y;
        SDL_RenderDrawRect(gfx, &rect);
        
        x = x + TILE_W;
    }
}

void renderSnake(SDL_Renderer *gfx, SnakeHead *snake)
{
    snake->x = snake->xPos * TILE_W;
    snake->y = snake->yPos * TILE_H;
    SDL_Rect rect = {snake->x, snake->y, TILE_H, TILE_W};
    SDL_SetRenderDrawColor(gfx, 200, 0, 0, 255);
    SDL_RenderFillRect(gfx, &rect);
    
    SDL_SetRenderDrawColor(gfx, 255, 255, 255, 255);
    for (int i = 0; i < snake->size; i++) {
        rect.x = snake->blocks[i].xPos * TILE_W;
        rect.y = snake->blocks[i].yPos * TILE_H;
        SDL_RenderFillRect(gfx, &rect);
    }
}