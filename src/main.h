#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdbool.h>
#include<time.h>
#include"data.h"

SDL_Window  *    window;
SDL_Renderer*    renderer;

SDL_Texture *    spritesheet,
            *    menu1,
            *    how1,
            *    game_over1,
            *    pause1,
            *    tragetory;

SDL_Point mouse = {0,0};

int safe_starting;

SDL_Rect        asteroid_textures[] =   {{64,0,16,16},{64,16,16,16},{64+16,0,16,16}},
                sun_texture =           {0,0,64,64},
                sun_render =            {(1280/2)-(32*scale),(720/2)-(32*scale),64*scale,64*scale},
                ship_texture =          {64,32,16,16},
                ship_fire =             {64,32,16,32},
                button_play =           {905,178,285,95},
                button_how =            {905,311,285,95},
                button_exit =           {905,445,285,95},
                button_OK =             {475,552,325,95},
                button_reset =          {152,552,325,95},
                button_menu =           {799,552,325,95},
                button_resume =         {478,452,325,95};

Asteroid        asteroids[asteroids_max];

Ship            ship;

GameState state = menu;

void game_start(void);

void add_asteroid(Asteroid a);
void render(SDL_Renderer* r);
void menu_render(SDL_Renderer*r);

void update(void);
void update_tragectory(int steps,SDL_Renderer* r);

double get_distance(double x1,double y1,double x2,double y2);
double vec_distance(Vector2 v1,Vector2 v2);
double get_angle(Vector2 v);
double to_radians(double degrees);

Vector2 gravity(Vector2 pos,double mass);
Asteroid generate(void);
Vector2 vec_normalized(Vector2 v);

Vector2 vec_add(Vector2 v1,Vector2 v2);
Vector2 vec_compare(Vector2 v1,Vector2 v2);
Vector2 vec_multiply(Vector2 v1,Vector2 v2);
Vector2 vec_multiplyN(Vector2 v,double N);
Vector2 vec_divide(Vector2 v1,Vector2 v2);
Vector2 vec_divideN(Vector2 v1,double N);
