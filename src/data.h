#define G 0.005
#define scale 3
#define asteroids_max 5
typedef enum GameStateE{
    menu,
    play,
    pause,
    how,
    game_over
}GameState;

typedef struct Vector2S{
    double x,y;
}Vector2;

typedef struct BodyS{
    Vector2 pos,center_pos,velocity;
    double mass,angle;
}Body;

typedef struct AsteroidS{
    Body body;
    char type;
    bool show;
}Asteroid;

typedef struct ShipS{
    Body body;
    bool engines,left,right;
}Ship;

const Vector2   sun_pos = {1280/2,720/2},
                initial_pos = {1280/2-(8*scale),720/2-(78*scale)},
                zero = {0,0};
const double sun_mass = 100000.0;