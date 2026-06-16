#include <iostream>
#include "raylib.h"
#include "../include/diff_eq_numerical_integrator.h"
#include "../include/physics_objects.h"
#include "rlgl.h"
#include "raymath.h"
#include <cmath>
#include <vector>
#include <matplot/matplot.h>


#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080)
#define WINDOW_TITLE "Falling Mass"
#define FPS 60


//SampleAtTime(double t, const std::vector<double>& times, const std::vector<double>& values)
//Takes in a time to be sampled at t, times, and values, and grabs the time t and returns the value
double SampleAtTime(double t, const std::vector<double>& times, const std::vector<double>& values){
    //safeguard for if times or values happens to be empty
    if(times.size() < 2) return values.empty() ? 0.0 : values.front();
    if(t <= times.front()) return values.front();
    if(t >= times.back()) return values.back();


    const double h = times[1] - times[0];                           //equivalent to step size
    const double fidx = (t - times.front()) / h;                    //the amount of samples between time t and the initial time 
    const size_t i = static_cast<size_t>(fidx);                     
    const double frac = fidx - static_cast<double>(i);

    return values[i] + frac * (values[i+1] - values[i]);

    
}

//Important to note, 1 meter == 1 pixel
int main(void)
{
    double pi = 3.1415926535;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);

    //Rectangle properties
    double rect_width = 10.0;
    double rect_height = 10.0;
    double rect_xpos = (SCREEN_WIDTH / 2.0) - (rect_width / 2.0);
    double rect_ypos = (SCREEN_HEIGHT / 2.0) - (rect_height / 2.0);
    double orange_rect_mass = 2.0;
    double gravity = -9.81;                      //used for Algebraic
    const double G = 6.67430e-11;               //Newton's Gravitational Constant
    const double mass_of_planet = 5.9722e24;   //Earth
    const double radius_of_planet = 6378137;  //meters
    const double mass_of_blue_rect = 1.0;    //kg
    const double mass_of_orange_rect = 2.0; //kg
   

    //Camera properties, set to center of the screen
    Vector2 offset = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    Vector2 target = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    float rotation = 0.0f;

    //zooming in, because 1 pixel = 1 meter, so we want a cube about 10 meters tall for a more realistic looking fall
    float zoom = 2.0f;
    Camera2D camera = {offset, target, rotation, zoom};
    


    //Differential equation y'' = -GMm/(R^2*m), (the y-axis in raylib is flipped, we will take care of this later)
    std::function<double(double,double,double)> blue_eqn_of_motion = [G, mass_of_planet, radius_of_planet, mass_of_blue_rect](double t, double y, double y_prime){
        return (-G*mass_of_planet*mass_of_blue_rect/(radius_of_planet*radius_of_planet*mass_of_blue_rect));
    };

    std::function<double(double,double,double)> orange_eqn_of_motion = [G, mass_of_planet, radius_of_planet, mass_of_orange_rect](double t, double y, double y_prime){
        return (-G*mass_of_planet*mass_of_orange_rect/(radius_of_planet*radius_of_planet*mass_of_orange_rect));
    };


    //Initial conditions and values, generic t_initial and t_final for all rectangles
    double t_initial = 0.0;
    double t_final = 10.0;
    double y_initial = 535.0;
    double step_size = 0.00001;

    //y(0) = 535, and y'(0) = 0 for both blue and orange rectangles
    InitialCondition initial_condition_gravity_pos(t_initial, y_initial);
    InitialCondition initial_condition_gravity_vel(t_initial, 0);

    //Blue Rectangle
    double blue_rect_ypos = 535;
    double blue_time = 0;
    SecondOrderODE blue_falling_body(blue_eqn_of_motion, initial_condition_gravity_pos, initial_condition_gravity_vel);
    blue_falling_body.SecondOrderRK4Solve(step_size, t_final);
    std::vector<double> time_vals = blue_falling_body.predicted_rk4_independent_vals;
    std::vector<double> y_pos_vals = blue_falling_body.predicted_rk4_dependent_vals;
    std::vector<double> vel_time_vals = blue_falling_body.predicted_prime_rk4_independent_vals;
    std::vector<double> vel_vals = blue_falling_body.predicted_prime_rk4_dependent_vals;


    //Orange Rectangle whose mass is 2*mass of Blue rectangle, they start at the same height and velocity, so using the same init cond as blue
    double orange_rect_ypos = 535;
    double orange_time = 0;
    SecondOrderODE orange_falling_body(orange_eqn_of_motion, initial_condition_gravity_pos, initial_condition_gravity_vel);
    orange_falling_body.SecondOrderRK4Solve(step_size, t_final);
    std::vector<double> orange_time_vals = orange_falling_body.predicted_rk4_independent_vals;
    std::vector<double> orange_y_pos_vals = orange_falling_body.predicted_rk4_dependent_vals;
    std::vector<double> orange_vel_time_vals = orange_falling_body.predicted_prime_rk4_independent_vals;
    std::vector<double> orange_vel_vals = orange_falling_body.predicted_prime_rk4_dependent_vals;
    


    double elapsed = 0.0;
    //const double rect_start_y = initial_condition_gravity_pos.independent;
    
    //used to plot algebraic solution
    std::vector<double> red_cube_time_vals;
    std::vector<double> red_cube_pos_vals;

    const int x_offset = 10;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        
        red_cube_time_vals.push_back(elapsed);
        red_cube_pos_vals.push_back(-rect_ypos + 2*y_initial);

        //Algebraic
        rect_ypos = y_initial + 0.5* -(gravity) * elapsed * elapsed;
        DrawRectangle(rect_xpos - x_offset, rect_ypos, rect_width, rect_height, RED);
    


        //Diff Eqn Blue Rect, flipping y because intrinsically, +y is down, so I'm flipping it such that -y is down
        const double blue_start_y = (SCREEN_HEIGHT / 2.0) - (rect_height / 2.0);
        blue_rect_ypos = SampleAtTime(elapsed, time_vals, y_pos_vals);
        double world_y = SampleAtTime(elapsed, time_vals, y_pos_vals);
        double screen_y = blue_start_y + (y_initial - world_y);
        DrawRectangle(rect_xpos + x_offset, screen_y, rect_width, rect_height, BLUE);



        //Diff Eqn Orange Rect
        const double orange_start_y = (SCREEN_HEIGHT / 2.0) - (rect_height / 2.0);
        orange_rect_ypos = SampleAtTime(elapsed, orange_time_vals, orange_y_pos_vals);
        double orange_world_y = SampleAtTime(elapsed, orange_time_vals, orange_y_pos_vals);
        double orange_screen_y = orange_start_y + (orange_start_y - orange_world_y);
        DrawRectangle(rect_xpos + 3*x_offset, orange_screen_y, rect_width, rect_height, ORANGE);

        //Blue and Orange Rectangle representing the numerical way of falling
        elapsed += GetFrameTime();



        EndMode2D();
        EndDrawing();
        
    }


    CloseWindow();


    //Second Order Testing
    // Actual Solution
    std::vector<double> t_real = matplot::linspace(t_initial, t_final);
    std::vector<double> y_real;
    std::vector<double> y_prime_real;
    double y_val;
    double y_prime_val;
    double t_val;
    
    for (double val : t_real){
        y_val = -0.5*(G*mass_of_planet/(radius_of_planet*radius_of_planet))*val*val + y_initial;
        y_prime_val = -9.8*val;
        y_real.push_back(y_val);
        y_prime_real.push_back(y_prime_val);
    }

    // Plotting RK4 predictions
    auto fig = matplot::figure(true);
    matplot::plot(blue_falling_body.predicted_rk4_independent_vals, blue_falling_body.predicted_rk4_dependent_vals, "--b",
        orange_falling_body.predicted_rk4_independent_vals, orange_falling_body.predicted_rk4_dependent_vals, "m",
        t_real, y_real, "r--o");
        

    std::vector<std::string> labels = {"RK4-Blue", "RK4-Orange", "Actual" };
    
    
    matplot::xlabel("Time (s)");
    matplot::ylabel("Height (m)");
    matplot::title("Free Falling Body, Δ = 0.00001");
    matplot::legend(labels);

    fig->width(800);
    fig->height(600);

    //matplot::save("plots/delta.png");

    matplot::show(fig);
    return 0;
}


