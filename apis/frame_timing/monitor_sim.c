#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "api.c"
#define API_IMPLEMENTATION_ONLY
#include "api.c"

double monitor_refresh_rate = 60;
uint8_t vsync = 1;

//these are functions so you can add randomness to them if you want to simulate "a slow frame every now and then" and stuff
double game_update_time()
{
    return .00001;
}
double game_render_time()
{
    return .005;
}
double game_display_time()
{
    return .000001;
}
double busy_time()
{
    return .000001;
}

#define d_rand (double)rand()/(double)RAND_MAX
//system time at 0 helps find rounding issues when the system timer is "in sync" with the game timer
//randomizing it is "more accurate"
double system_time = 0;//d_rand * 10000;

double timing_fuzziness = 1.0 / 60.0 * .005;
double fuzzy()
{
    return (2.0*d_rand - 1.0)*timing_fuzziness;
}

//measurements
int frame_updates = 0;
double total_updates = 0;
int last_vsync = 0;
int missed_updates = 0;
int double_updates = 0;

double max(double a, double b)
{
    double max = a;
    if(b > max) max = b;
    return max;
}

void simulate_update()
{
    system_time += max(0.0, game_update_time() + fuzzy() * .01);
    total_updates++;
    frame_updates++;
}
void simulate_render()
{
    system_time += max(0.0, game_render_time() + fuzzy() * .01);
}
void simulate_display()
{
    if(vsync)
        system_time += max(0.0, (ceil(system_time * monitor_refresh_rate) / monitor_refresh_rate) - system_time + fuzzy()); 
    else
        system_time += max(0.0, game_display_time() + fuzzy());

    int current_vsync = round(system_time * monitor_refresh_rate);
    if(last_vsync != current_vsync)
    {
        for(int i = last_vsync; i<current_vsync-1; i++)
        {
            printf("\n");
            missed_updates++;
        }
        printf("%d",frame_updates);
        if(frame_updates > 1) double_updates++;
        last_vsync = current_vsync;

        frame_updates = 0;
    }
    
}
void simulate_busy()
{
    system_time += max(0.0, busy_time() + fuzzy() * .00001);
}

//this is where you test your game loop
int main()
{
    frame_timing_setup(60.0);

    double prev_frame_time = system_time;
    last_vsync = round(system_time * monitor_refresh_rate);
    int first_vsync = last_vsync;

    double frame_accumulator = 0;

    while(total_updates < 10000)
    {
        // update_accumulator(system_time);
        double current_frame_time = system_time;
        double delta_frame_time = current_frame_time - prev_frame_time;
        frame_accumulator += delta_frame_time;
        #error frame_accumulator would have to be float or double
        prev_frame_time = current_frame_time;

        while(frame_accumulator >= 1.0 / 60.0)
        {
            simulate_update();
            frame_accumulator -= 1.0 / 60.0;
        }

        simulate_render();
        simulate_display();
        simulate_busy();
    }

    printf("\n\n");
    printf("TOTAL UPDATES: %u\n",total_updates);
    printf("TOTAL VSYNCS: %u\n", last_vsync-first_vsync);
    printf("TOTAL DOUBLE UPDATES: %u\n", double_updates);
    printf("TOTAL SKIPPED RENDERS: %u\n", missed_updates);

    printf("GAME TIME: %f\n", total_updates*(1.0/60.0));
    printf("SYSTEM TIME: %f\n", (last_vsync-first_vsync)/monitor_refresh_rate);
}