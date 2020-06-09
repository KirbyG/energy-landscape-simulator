#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>

void draw_circle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

int main() {
	SDL_Window* window;                    // Declare a pointer

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    int width = 800;
    int height = 800;
    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        100,           // initial x position
        300,           // initial y position
        width,                               // width, in pixels
        height,                               // height, in pixels
        SDL_WINDOW_OPENGL | SDL_WINDOW_MAXIMIZED                 // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // The window is open: could enter program loop here (see SDL_PollEvent())

    SDL_Renderer* renderman = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    int radius = 40;
    int x = 100;
    int y = 400;
    int vx = 9;
    int vy = 3;
    int ax = 0;
    int ay = 1;

    SDL_Event e;
	bool quit = false;
	struct timespec t_curr;
	struct timespec t_prev;
	int64_t t_curr_f;
	int64_t t_prev_f;
	clock_gettime(CLOCK_REALTIME, &t_prev);
	t_prev_f = t_prev.tv_sec * INT64_C(1000) + t_prev.tv_nsec / 1000000;
	while (!quit){
    	while (SDL_PollEvent(&e)){
        	if (e.type == SDL_QUIT){
            	quit = true;
        	}
        	if (e.type == SDL_KEYDOWN){
            	quit = true;
        	}
        	if (e.type == SDL_MOUSEBUTTONDOWN){
            	quit = true;
        	}
    	}

    	//logic
    	bool bounce = false;
    	if (x + radius >= width || x - radius <= 0) {
    		//printf("bounce x\n");
    		bounce = true;
    		vx = -vx;
    	}
    	if (y + radius >= height || y - radius <= 0) {
    		//printf("bounce y\n");
    		bounce = true;
    		vy = -vy;
    	}
    	

    	x += vx;
    	y += vy;
    	if (!bounce) {
    		vx += ax;
    		vy += ay;
    	}
    	//printf("vx = %d,vy = %d, x = %d, y = %d\n", vx, vy, x, y);




    	SDL_SetRenderDrawColor(renderman, 0xff, 0xff, 0xff, 0xff);
    	SDL_RenderClear(renderman);

    	//SDL_Rect rect = {x, 100, 200, 200};
    	SDL_SetRenderDrawColor( renderman, 0x64, 0x00, 0x00, 0xff );
    	//SDL_RenderFillRect(renderman, &rect);
    	//printf("loop\n");
    	draw_circle(renderman, x, y, radius);
    	//printf("checkpoint2\n");
    	SDL_RenderPresent(renderman);
    	int fps = 20;
    	int delta = 30;
    	while (t_curr_f - t_prev_f < delta) {
    		clock_gettime(CLOCK_REALTIME, &t_curr);
    		t_curr_f = t_curr.tv_sec * INT64_C(1000) + t_curr.tv_nsec / 1000000;
    		//printf("inner: tcurr=%lld, tprev=%lld\n", t_curr_f, t_prev_f);
    	}
    	if (t_curr_f - t_prev_f > delta) {
    		//printf("fail\n");
    	}
      //clock_gettime(CLOCK_REALTIME, &t_curr);
      //t_curr_f = t_curr.tv_sec * INT64_C(1000) + t_curr.tv_nsec / 1000000;
    	printf("%lld\n", t_curr_f - t_prev_f);
    	t_prev_f = t_curr_f;
	}

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();

    
    

    return 0;
}