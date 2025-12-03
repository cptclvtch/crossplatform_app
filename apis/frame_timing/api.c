//Dependencies
#include <stdint.h>

#ifndef _FRAME_TIMING_H
    #define _FRAME_TIMING_H

uint8_t min_fps = 15, target_fps = 60, max_fps = 255;
uint32_t frame_count = 0;

typedef uint32_t nano_s;
typedef uint32_t milli_s;

//TODO consider switching to nano-seconds
milli_s oldtime, desired_ft, dt = 0;
#define FT ((float)desired_ft/1000)
#define DT ((float)dt/1000)

uint8_t timing_reset_request; //bool
milli_s frame_accumulator; //TODO research type for this variable

nano_s vsync_maxerror = 200;
#define MAX_VSYNC_MULTIPLES 5
nano_s vsync_frametime_snaps[MAX_VSYNC_MULTIPLES];
// uint32_t ticks_per_second;

uint8_t update_multiplicity = 1;

void measure_dt(milli_s newtime);
void massage_dt();
void frame_timing_setup(float display_refresh_rate);
#endif //_FRAME_TIMING_H

//----------------------------------

#if defined(INCLUDE_IMPLEMENTATION) && !defined(_FRAME_TIMING_C)
    #define _FRAME_TIMING_C

void frame_timing_setup(float display_refresh_rate)
{
    desired_ft = 1000 / target_fps; //milliseconds per frame

    //vsync snapping
    float hz = 60.0;
    if(display_refresh_rate > 0) hz = display_refresh_rate;

    while((uint8_t)(hz/2) >= min_fps) hz /= 2;

    uint8_t i = 0;
    while(i < MAX_VSYNC_MULTIPLES)
    {
        vsync_frametime_snaps[i] = (milli_s)(1000000 / hz);
        
        hz *= 2;
        i++;
    }

    timing_reset_request = 1;
}

//TODO see if this is only useful for fixed time steps
void vsync_snap()
{
    uint8_t i = 0;
    while(i < MAX_VSYNC_MULTIPLES)
    {
        nano_s snap_ft = vsync_frametime_snaps[i];
        nano_s snap_diff = dt*1000 - snap_ft;
        if(snap_diff >= -vsync_maxerror || snap_diff <= vsync_maxerror)
        {
            dt = snap_ft;
            break;
        }

        i++;
    }
}

void measure_dt(milli_s newtime)
{
    if(newtime > oldtime)
        dt = newtime - oldtime;
    else
        dt = desired_ft; //default value

    //dismiss slow frames
    if(dt > desired_ft*8) dt = desired_ft;

    oldtime = newtime;
}

void massage_dt()
{
    vsync_snap();

    //dt averaging would go here
}

void update_accumulator(milli_s newtime)
{
    measure_dt(newtime);
    massage_dt();

    //add to the accumulator
    frame_accumulator += dt;

    //TODO consider pulling this into it's own function
    //reset if requested by user or if update is taking too long(spiral of death)
    if(timing_reset_request || frame_accumulator > desired_ft*8)
    {
        frame_accumulator = 0;
        dt = desired_ft;

        timing_reset_request = 0;
    }
}

void fixed_update(void (*update_func)())
{
    while(frame_accumulator >= desired_ft*update_multiplicity)
    {
        uint8_t i = 0;
        while(i < update_multiplicity)
        {
            update_func();

            frame_accumulator -= desired_ft;
            i++;
            frame_count++;
        }
    }
}

void game_loop_function(milli_s newtime)
{
    update_accumulator(newtime);

    //process system events

    //update game
    // fixed_update(update_func);
    
    //game.render(1.0);
}

//old delay code:
//SDL_Delay((1000/target_fps - dt)*(dt < 1000/target_fps));

//TODO research SDL_timer.h implementation
#endif

//TODO reread article and implement new frame timing api
//https://medium.com/@tglaiel/how-to-make-your-game-run-at-60fps-24c61210fe75
/*
//these are loaded from Settings in production code
double update_rate = 60;
int update_multiplicity = 1;
#ifdef FRAME_INTERPOLATION
bool unlock_framerate = true;
#endif

//compute how many ticks one update should be
int64_t clocks_per_second = SDL_GetPerformanceFrequency();
double fixed_deltatime = 1.0 / update_rate;
int64_t desired_frametime = clocks_per_second / update_rate;

//these are to snap deltaTime to vsync values if it's close enough
int64_t vsync_maxerror = clocks_per_second / 5000;

//get the refresh rate of the display (you should detect which display the window is on in production)
int display_framerate = 60;
SDL_DisplayMode current_display_mode;
if(SDL_GetCurrentDisplayMode(0, &current_display_mode)==0) {
    display_framerate = current_display_mode.refresh_rate;
}
int64_t snap_hz = display_framerate;
if(snap_hz <= 0) snap_hz = 60;

//these are to snap deltaTime to vsync values if it's close enough
#define NO_OF_SNAP_FREQUENCIES 8
int64_t snap_frequencies[NO_OF_SNAP_FREQUENCIES] = {};
for(int i = 0; i < NO_OF_SNAP_FREQUENCIES; i++)
{
    snap_frequencies[i] = (clocks_per_second / snap_hz) * (i+1);
}

//this is for delta time averaging
//I know you can and should use a ring buffer for this, but I didn't want to include dependencies in this sample code
const int time_history_count = 4;
int64_t time_averager[time_history_count] = {desired_frametime, desired_frametime, desired_frametime, desired_frametime};
int64_t averager_residual = 0;

//these are stored in my Application class and are not local variables in production code
bool timing_reset_request = true;
int64_t prev_frame_time = get_counter_fn();
int64_t frame_accumulator = 0;

while (running){
  //frame timer
    int64_t current_frame_time = SDL_GetPerformanceCounter();
    int64_t delta_time = current_frame_time - prev_frame_time;
    prev_frame_time = current_frame_time;

  //handle unexpected timer anomalies (overflow, extra slow frames, etc)
    if(delta_time > desired_frametime*8){ //ignore extra-slow frames
        delta_time = desired_frametime;
    }
    if(delta_time < 0){
        delta_time = 0;
    }


  //vsync time snapping
    for(int64_t snap : snap_frequencies){
        if(std::abs(delta_time - snap) < vsync_maxerror){
            delta_time = snap;
            break;
        }
    }

  //delta time averaging
    for(int i = 0; i<time_history_count-1; i++){
        time_averager[i] = time_averager[i+1];
    }
    time_averager[time_history_count-1] = delta_time;
    int64_t averager_sum = 0;
    for(int i = 0; i<time_history_count; i++){
        averager_sum += time_averager[i];
    }
    delta_time = averager_sum / time_history_count;

    averager_residual += averager_sum % time_history_count;
    delta_time += averager_residual / time_history_count;
    averager_residual %= time_history_count;

  //add to the accumulator
    frame_accumulator += delta_time;

  //spiral of death protection
    if(frame_accumulator > desired_frametime*8){ 
        timing_reset_request = true;
    }

  //timer timing_reset_request if requested
    if(timing_reset_request) {
        frame_accumulator = 0;
        delta_time = desired_frametime;
        timing_reset_request = false;
    }

  // process system events
    ProcessEvents();

    #ifdef FRAME_INTERPOLATION
    if(unlock_framerate)
    { //UNLOCKED FRAMERATE, INTERPOLATION ENABLED
        int64_t consumedDeltaTime = delta_time;

        while(frame_accumulator >= desired_frametime)
        {
            game.fixed_update(fixed_deltatime);
            if(consumedDeltaTime > desired_frametime)
            { //cap variable update's dt to not be larger than fixed update, and interleave it (so game state can always get animation frames it needs)
                game.variable_update(fixed_deltatime);
                consumedDeltaTime -= desired_frametime;
            }
            frame_accumulator -= desired_frametime;
        }

        game.variable_update((double)consumedDeltaTime / clocks_per_second);
        game.render((double)frame_accumulator / desired_frametime);        
    }
    else
    #endif
    { //LOCKED FRAMERATE, NO INTERPOLATION
        while(frame_accumulator >= desired_frametime*update_multiplicity)
        {
            for(int i = 0; i<update_multiplicity; i++)
            {
                game.fixed_update(fixed_deltatime);
                game.variable_update(fixed_deltatime);
                frame_accumulator -= desired_frametime;
            }
        }
        
        game.render(1.0);
    }
    display(); //swap buffers
}*/