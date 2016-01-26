
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>


// directions
#define UP 0
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// Constants
#define TILE_W 32
#define TILE_H 32

// Some game objects
typedef struct
{
    ushort xPos, yPos;
    ushort direction;
    
} Block;

typedef Block SnakeBlock;
typedef Block Bait;

typedef struct
{
    int x,y;
    int xPos, yPos;
    int size;
    SnakeBlock *blocks;
    
    ushort direction;
} SnakeHead;




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
                        if (snake->direction == DOWN)
                            break;
                        snake->direction = UP;
                        break;
                    case SDLK_a:
                        if (snake->direction == RIGHT)
                            break;
                        snake->direction = LEFT;
                        break;
                    case SDLK_s:
                        if (snake->direction == UP)
                            break;
                        snake->direction = DOWN;
                        break;
                    case SDLK_d:
                        if (snake->direction == LEFT)
                            break;
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
    for (int i = snake->size - 1; i >= 1; i--) {
        snake->blocks[i].xPos = snake->blocks[i-1].xPos;
        snake->blocks[i].yPos = snake->blocks[i-1].yPos;
        snake->blocks[i].direction = snake->blocks[i-1].direction;
    }
    
    snake->blocks[0].xPos = snake->xPos;
    snake->blocks[0].yPos = snake->yPos;
    snake->blocks[0].direction = snake->direction;

    
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

bool isBaitEaten(SnakeHead *snake, Bait *bait)
{
    return snake->xPos == bait->xPos && snake->yPos == bait->yPos;
}

void updateSnakeBody(SnakeHead *snake, Bait *bait)
{
    if(! isBaitEaten(snake, bait) ) return;

    // generate new Bait
    bait->xPos = rand() % 20;
    bait->yPos = rand() % 15;
    
    // add new block to the end of snake body
    int i = snake->size;
    snake->blocks[i].direction = snake->blocks[i-1].direction;
    snake->blocks[i].xPos = snake->blocks[i-1].xPos;
    snake->blocks[i].yPos = snake->blocks[i-1].yPos;
    
    // calculate correct coordinates offset, depending on direction
    switch (snake->blocks[i].direction) {
        case UP:
            snake->blocks[i].yPos += 1;
            break;
        case DOWN:
            snake->blocks[i].yPos -= 1;
            break;
        case LEFT:
            snake->blocks[i].yPos += 1;
            break;
        case RIGHT:
            snake->blocks[i].yPos -= 1;
            break;
        default:
            break;
    }

    snake->size++;
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

void renderBait(SDL_Renderer *gfx, Bait *bait)
{
    SDL_Rect rect = {bait->xPos * TILE_W, bait->yPos * TILE_H, TILE_H, TILE_W};
    SDL_SetRenderDrawColor(gfx, 255, 255, 0, 255);
    SDL_RenderFillRect(gfx, &rect);
}

// ============================== Main ===============================

int main(int argc, const char * argv[]) {
    srand((unsigned int) time(NULL)); // init random seed
    
    bool is_running = true;
    SnakeHead snake;
    snake.xPos = 10;
    snake.yPos = 9;
    snake.size = 4;
    snake.direction = UP;
    
    SnakeBlock blocks[50] = {
        {10, snake.yPos+1},
        {10, snake.yPos+2},
        {10, snake.yPos+3},
        {10, snake.yPos+4}
    };
    snake.blocks = blocks;
    
    Bait bait = {rand() % 20, rand() % 15};
    
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    
    window = SDL_CreateWindow("My SDL Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    while (is_running) {
        is_running = processEvents(window, &snake);
        update(&snake);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Render
        renderField(renderer);
        renderBait(renderer, &bait);
        renderSnake(renderer, &snake);
        
        SDL_RenderPresent(renderer);
        
        updateSnakeBody(&snake, &bait);
        
        SDL_Delay(300);
    }
    
    // Shutdown
    
    SDL_DestroyRenderer(renderer);
    
    SDL_Quit();
    return 0;
}
