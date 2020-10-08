#define DEBUG
#define shipb ship.body
#include"main.h"

int main(int argc,char*argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Orbitoor",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,720,0);

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    #ifndef DEBUG
    spritesheet = IMG_LoadTexture(renderer,"assets\\sprites.png");
    menu1 = IMG_LoadTexture(renderer,"assets\\menu.png");
    how1 = IMG_LoadTexture(renderer,"assets\\how.png");
    game_over1 = IMG_LoadTexture(renderer,"assets\\game_over.png");
    pause1 = IMG_LoadTexture(renderer,"assets\\pause.png");
    #else
    spritesheet = IMG_LoadTexture(renderer,"C:\\dev\\projects\\LD-47\\Orbitoor\\bin\\assets\\sprites.png");
    menu1 = IMG_LoadTexture(renderer,"C:\\dev\\projects\\LD-47\\Orbitoor\\bin\\assets\\menu.png");
    how1 = IMG_LoadTexture(renderer,"C:\\dev\\projects\\LD-47\\Orbitoor\\bin\\assets\\how.png");
    game_over1 = IMG_LoadTexture(renderer,"C:\\dev\\projects\\LD-47\\Orbitoor\\bin\\assets\\game_over.png");
    pause1 = IMG_LoadTexture(renderer,"C:\\dev\\projects\\LD-47\\Orbitoor\\bin\\assets\\pause.png");
    #endif

    if(window==NULL||renderer==NULL||spritesheet==NULL||menu1==NULL||how1==NULL||game_over1==NULL||pause1==NULL)
    {SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"ERROR",SDL_GetError(),window);exit(-1);}
    
    SDL_SetTextureBlendMode(spritesheet,SDL_BLENDMODE_BLEND);

    tragectory = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_TARGET,width,height);

    int frame_interval = 1000/FPS,start_MS,end_MS,delay = frame_interval;

    running = true;
    SDL_Event handler;
    while (running){
        start_MS = SDL_GetTicks();
        while (SDL_PollEvent(&handler)){
            switch (handler.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch (handler.key.keysym.sym)
                    {
                        case SDLK_SPACE:
                            if(state==play)
                                ship.engines = true;
                            break;
                        case SDLK_RIGHT:
                            if(state==play)
                                ship.right = true;
                            break;
                        case SDLK_LEFT:
                            if(state==play)
                                ship.left = true;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (handler.key.keysym.sym){
                        case SDLK_SPACE:
                            if(state==play)
                                ship.engines = false;
                            break;
                        case SDLK_RIGHT:
                            if(state==play)
                                ship.right = false;
                        break;
                        case SDLK_LEFT:
                            if(state==play)
                                ship.left = false;
                            break;
                        case SDLK_ESCAPE:
                            update_state(pause);
                            break;
                        case SDLK_p:
                            #ifdef DEBUG
                            if(state == play || state == pause) break;
                            if(!potato){
                                potato = true;
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Potato Mode","potato mode on\ndecreases graphics for better performance",window);
                                tragectory_steps = 100;
                            }else{
                                potato = false;
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Potato Mode","potato mode off\nincreases graphics",window);
                                tragectory_steps = 1000;
                            }
                            #endif
                            break;
                    }
                case SDL_MOUSEMOTION:
                    mouse = (SDL_Point){handler.button.x,handler.button.y};
                    break;
                case SDL_MOUSEBUTTONUP:
                    get_menu_input();
                    break;
                case SDL_WINDOWEVENT:
                    switch (handler.window.event){
                        case SDL_WINDOWEVENT_MINIMIZED:
                            while (SDL_WaitEvent(&handler)){
                                if (handler.window.event == SDL_WINDOWEVENT_RESTORED){
                                    break;
                                }
                            }
                            break;
                    }
                    break;
                }
        }

        switch (state){
        case play:
            update();
            render();
            tragectory_request = false;
            break;
        default:
            menu_render();
            break;
        }
        end_MS = SDL_GetTicks();
        if(frame_interval > end_MS - start_MS){
            delay = frame_interval - (end_MS - start_MS);
            SDL_Delay(delay);
        }
    }

    SDL_DestroyTexture(spritesheet);
    SDL_DestroyTexture(menu1);
    SDL_DestroyTexture(how1);
    SDL_DestroyTexture(game_over1);
    SDL_DestroyTexture(pause1);
    
    SDL_DestroyTexture(menu1);
    SDL_DestroyTexture(tragectory);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
void get_menu_input(void){
    switch (state){
    case menu:
        if (SDL_PointInRect(&mouse,&button_play)){
            update_state( play);
            game_start();
        }else if (SDL_PointInRect(&mouse,&button_how)){
            update_state( how);
        }else if (SDL_PointInRect(&mouse,&button_exit)){
            running = false;
        }
        break;
    case how:
        if (SDL_PointInRect(&mouse,&button_OK)){
            update_state( menu);
        }
    case game_over:
        if(SDL_PointInRect(&mouse,&button_reset)){
            update_state(play);
            game_start();
        }else if (SDL_PointInRect(&mouse,&button_menu)){
            update_state(menu);
        }
        break;
    case pause:
        if(SDL_PointInRect(&mouse,&button_reset)){
            update_state(play);
            game_start();
        }else if (SDL_PointInRect(&mouse,&button_menu)){
            update_state(menu);
        }else if (SDL_PointInRect(&mouse,&button_resume)){
            update_state(play);
        }
        break;
    }
}

void update_state(GameState s){
    state = s;
    switch (s)
    {
    case play:
        break;
    default:
        menu_rendered = false;
        break;
    }
}

void menu_render(){
    if(menu_rendered)
        return;
    switch (state)
    {
    case menu:
        SDL_RenderCopy(renderer,menu1,NULL,NULL);
        break;
    case how:
        SDL_RenderCopy(renderer,how1,NULL,NULL);
        break;
    case game_over:
        SDL_RenderCopy(renderer,game_over1,NULL,NULL);
        break;
    case pause:
        SDL_RenderCopy(renderer,pause1,NULL,NULL);
        break;
    }

    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);
    menu_rendered = true;
}

double to_radians(double degrees){
    return degrees*(3.14159265359/180);
}

void add_asteroid(Asteroid a){
    for (size_t i = 0; i < asteroids_max; i++){
        if(!asteroids[i].show){
            asteroids[i] = a;
            return;
        }
    }
}

void update(void){

    tragectory_update_timer--;
    if(tragectory_update_timer==0){
        tragectory_request = true;
        tragectory_update_timer = 10;
    }

    for (size_t i = 0; i < asteroids_max; i++){
        
        Asteroid actual = asteroids[i];
        if(!actual.show)
            continue;
        actual.body.velocity = vec_add(actual.body.velocity,gravity(actual.body.center_pos,actual.body.mass));

        actual.body.pos.x+=actual.body.velocity.x;
        actual.body.pos.y+=actual.body.velocity.y;
        actual.body.center_pos = (Vector2){actual.body.pos.x+(8*scale),actual.body.pos.y+(8*scale)};

        
        if (actual.body.pos.x < 0     ||
            actual.body.pos.x > 1280  ||
            actual.body.pos.y > 720)    actual.show = false;
        else if(vec_distance(actual.body.center_pos,sun_pos)-(30*scale)<1)
            actual.show = false;
        
        Vector2 to_ship = vec_compare(actual.body.center_pos,shipb.center_pos);
        if(vec_distance(zero,to_ship) - (10*scale) < 1){
            shipb.velocity = vec_add(shipb.velocity,vec_divideN(actual.body.velocity,2));
            actual.show = false;
        }
        asteroids[i] = actual;
    }
    
    if(ship.engines){

        Vector2 engine_force = {SDL_cos(to_radians(shipb.angle-90))/100/shipb.mass,SDL_sin(to_radians(shipb.angle-90))/100/shipb.mass};
        shipb.velocity = vec_add(shipb.velocity,engine_force);
    }

    shipb.velocity = vec_add(shipb.velocity,gravity(shipb.center_pos,shipb.mass));

    if(ship.right){
        shipb.angle+=3;
    }else if(ship.left){
        shipb.angle-=3;
    }

    if(vec_distance(shipb.center_pos,sun_pos)-(40*scale)<1){
        update_state(game_over);
    }else if(   shipb.pos.x < 0     ||
                shipb.pos.x > 1280  ||
                shipb.pos.y < 0     ||
                shipb.pos.y > 720) update_state(game_over);
    
    shipb.pos = vec_add(shipb.pos,shipb.velocity);
    shipb.center_pos = (Vector2){shipb.pos.x+(8*scale),shipb.pos.y+(8*scale)};

    if(safe_starting>0)
        safe_starting--;

    if(!safe_starting)
    if(rand()%20==1){
        double angle = to_radians((rand()%90)+45);
        double speed = ((rand()%3)+2);

        Asteroid new_asteroid = {{{(rand()%1000)+140,0},{new_asteroid.body.pos.x+8,new_asteroid.body.pos.y+8},
                                {SDL_cos(angle)*speed,SDL_sin(angle)*speed},(rand()%50)+40,rand()%360} ,rand()%3,true};
        add_asteroid(new_asteroid);
    }
    if(tragectory_request)
        update_tragectory();
}

void render(){
    if(tragectory_request)
        render_tragectory();
    SDL_RenderCopy(renderer,tragectory,NULL,NULL);
    SDL_RenderCopy(renderer,spritesheet,&sun_texture,&sun_render);
    
    SDL_Rect ship_render = {shipb.pos.x,shipb.pos.y,16*scale,((ship.engines)?32:16)*scale};
    SDL_Point center = {8*scale,8*scale};

    SDL_RenderCopyEx(renderer,spritesheet,(ship.engines)?&ship_fire:&ship_texture,&ship_render,shipb.angle,&center,SDL_FLIP_NONE);

    for (size_t i = 0; i < asteroids_max; i++){
        Asteroid actual = asteroids[i];
        if(actual.show){
            
            SDL_Rect render = {actual.body.pos.x,actual.body.pos.y,16*scale,16*scale};
            if(!potato)
                SDL_RenderCopyEx(renderer,spritesheet,&asteroid_textures[actual.type],&render,actual.body.angle,NULL,SDL_FLIP_NONE);
            else
                SDL_RenderCopy(renderer,spritesheet,&asteroid_textures[actual.type],&render);
        }
    }

    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer,0,0,0,0);
    SDL_RenderClear(renderer);
}

Vector2 gravity(Vector2 pos,double mass){
    Vector2 difference = {sun_pos.x-pos.x,sun_pos.y-pos.y};
    
    double distance = vec_distance(pos,sun_pos);
    distance*=distance;

    Vector2 dir = vec_normalized(difference);
    Vector2 force = vec_multiplyN(dir,(G*mass*sun_mass)/distance);
    Vector2 acceleration = vec_divideN(force,mass);
    
    return acceleration;
}

void update_tragectory(){
    Vector2 pos = shipb.center_pos;
    Vector2 tmp_force = vec_multiplyN(shipb.velocity,(potato)?12:2);

    for (size_t i = 0; i < tragectory_steps; i++){
        tragectory_prediction[i] = (SDL_Point){pos.x,pos.y};
        
        tmp_force = vec_add(tmp_force,vec_multiplyN(gravity(pos,shipb.mass),(potato)?144:4));

        pos = vec_add(pos,tmp_force);
    }
}

void render_tragectory(){
    SDL_SetRenderTarget(renderer,tragectory);
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,0,255,0,255);
    SDL_RenderDrawLines(renderer,tragectory_prediction,(potato)?99:999);

    SDL_SetRenderTarget(renderer,NULL);
}

void game_start(){

    srand(time(0));

    for(size_t i = 0; i < asteroids_max; i++){
        asteroids[i] = (Asteroid){{zero,zero,zero,0,0},0,false};
    }

    ship = (Ship){ { initial_pos,{sun_pos.x,shipb.pos.y+8*scale},{1.5,0},1,90},false,false,false};

    tragectory_request = true;
    //double distance = vec_distance(shipb.center_pos,sun_pos);
    //double velocity = SDL_sqrt(G*sun_mass*((2/distance)-(1/vec_distance(shipb.center_pos,sun_pos))));
    //shipb.velocity = (Vector2){velocity,0};
    //SDL_Log("%f",velocity);
    safe_starting = 60*5;
}//1.638802

#pragma region vector math
Vector2 vec_add(Vector2 v1,Vector2 v2){
    return(Vector2){v1.x+v2.x,v1.y+v2.y};
}
Vector2 vec_compare(Vector2 v1,Vector2 v2){
    return(Vector2){v1.x-v2.x,v1.y-v2.y};
}
double get_distance(double x1,double y1,double x2,double y2){
    return SDL_sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
}
double vec_distance(Vector2 v1,Vector2 v2){
    return get_distance(v1.x,v1.y,v2.x,v2.y);
}
Vector2 vec_normalized(Vector2 v){
    double mag = vec_distance(v,(Vector2){0,0});
    if(mag>0.0){
        v.x/=mag;
        v.y/=mag;
        return v;
    }else{
        return (Vector2){0,0};
    }
}
Vector2 vec_multiply(Vector2 v1,Vector2 v2){
    return (Vector2){v1.x*v2.x,v1.y*v2.y};
}
Vector2 vec_multiplyN(Vector2 v,double N){
    return vec_multiply(v,(Vector2){N,N});
}
Vector2 vec_divide(Vector2 v1,Vector2 v2){
    return (Vector2){v1.x/v2.x,v1.y/v2.y};
}
Vector2 vec_divideN(Vector2 v,double N){
    return vec_divide(v,(Vector2){N,N});
}
double get_angle(Vector2 v){
    return SDL_atan2(v.y,v.x)*(180.0/3.1415);
}
#pragma endregion
