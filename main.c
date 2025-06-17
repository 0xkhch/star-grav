#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>

#define NUM_STARS 1000
#define GRAV_CONST (6.67 / 1e11)

#define STAR_MASS 50000
#define STAR_RADIUS 5
#define PLANET_MASS 1000
#define PLANET_RADIUS 1

#define DELTA_TIME 10000
#define FRICTION 0.5

#define WIDTH 1200
#define HEIGHT 1200


typedef struct {
	double mass;
	double velocity_x;
	double velocity_y;
	Vector2 pos;
} Star_t;

Star_t star_array[NUM_STARS];
Vector2 star_pos = {
	.x = WIDTH/2,
	.y = HEIGHT/2
};

int random_range(int min, int max);
void init_array();
void draw_pixals();
void update_positions();
double euclidean_distance(Vector2 pos1, Vector2 pos2);

int random_range(int min, int max) {
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
} 

void init_array() {
	for (int i = 0; i < NUM_STARS; ++i) {
   		Vector2 pos;
		// Brute force find a random point not within the 100 radi of the star
		for (;;) {
			pos.x = random_range(0, WIDTH);
			pos.y = random_range(0, HEIGHT);

			if (euclidean_distance(pos, star_pos) > 100) {
				break;
			}
		}

		// calculating the initial velocity of the planet based on how faraway it is
		double dx = pos.x - WIDTH/2;
		double dy = pos.y - HEIGHT/2;
		double angle = atan2(dy, dx);
		
		double init_vel = sqrt(GRAV_CONST * STAR_MASS / euclidean_distance(pos, star_pos)); 

		double init_x = init_vel * cos(angle + PI/2);
		double init_y = init_vel * sin(angle + PI/2);
		//printf("Init vel of %d is %f in x and %f in y\n", i, init_x, init_y);

		Star_t star = {
			.mass = PLANET_MASS,
			.velocity_x = init_x,
			.velocity_y = init_y,
			.pos = pos
		};
    	star_array[i] = star;
	}
}

void draw_pixels() {
	for (int i = 0; i < NUM_STARS; ++i) {
		DrawCircleV(star_array[i].pos, PLANET_RADIUS, WHITE);
	}   
}

void update_position() {
	for(int i = 0; i < NUM_STARS; ++i) {
		// clamping to 5 because devision with inf
		double distance = euclidean_distance(star_array[i].pos, star_pos) < 5 ? 5 : euclidean_distance(star_array[i].pos, star_pos);
		double gravitional_force = (GRAV_CONST * star_array[i].mass * STAR_MASS) / (distance * distance);
		double force_x = gravitional_force * (star_pos.x - star_array[i].pos.x)/distance;
    	double force_y = gravitional_force * (star_pos.y - star_array[i].pos.y)/distance;
			
		star_array[i].velocity_x = star_array[i].velocity_x + force_x * DELTA_TIME / star_array[i].mass;
		star_array[i].velocity_y = star_array[i].velocity_y + force_y * DELTA_TIME / star_array[i].mass;
		star_array[i].pos.x = star_array[i].pos.x + star_array[i].velocity_x * DELTA_TIME;
		star_array[i].pos.y = star_array[i].pos.y + star_array[i].velocity_y * DELTA_TIME;		
	}
}
double euclidean_distance(Vector2 pos1, Vector2 pos2) {
	double dx = pos2.x - pos1.x;
	double dy = pos2.y - pos1.y;
	return sqrt(dx * dx + dy * dy);
}

int main() {
	InitWindow(WIDTH, HEIGHT, "Render");
	SetTargetFPS(60);
	init_array();
	while (!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(BLACK);
			//DrawFPS(0, 0);
			DrawCircleV(star_pos, STAR_RADIUS, WHITE);
			draw_pixels();
			update_position();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
