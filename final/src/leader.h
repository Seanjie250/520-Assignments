#ifndef __LEADER_AGENT__H
#define __LEADER_AGENT__H

#include "enviro.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace enviro;

class LeaderController : public Process, public AgentInterface {

    public:
    
    LeaderController() : Process(), AgentInterface(), 
                        random_turn_counter(0), random_turn_duration(0),
                        random_direction(0), stuck_counter(0),
                        last_position_x(0), last_position_y(0),
                        turning_90(false), turn_start_angle(0), turn_target_angle(0) {
        // Initialize random seed
        srand(time(NULL));
    }

    void init() {}

    void start() {
        last_position_x = x();
        last_position_y = y();
        // Start with random movement
        random_direction = (rand() % 2 == 0) ? 1.0 : -1.0;
        random_turn_duration = 30 + (rand() % 60);  // Random duration 30-90 updates
    }

    void update() {

        double distance = sensor_value(0);
        double current_x = x();
        double current_y = y();
        double current_angle = angle();
    
        // Improved stuck detection - more sensitive
        double movement = sqrt((current_x-last_position_x)*(current_x-last_position_x) +
                               (current_y-last_position_y)*(current_y-last_position_y));
    
        if ( movement < 1.0 ) {  // Lower threshold for faster stuck detection
            stuck_counter++;
        } else {
            stuck_counter = 0;
        }
    
        last_position_x = current_x;
        last_position_y = current_y;
    
        // If currently turning 90 degrees clockwise, continue until complete
        if ( turning_90 ) {
            // Calculate how much we've turned
            double start_norm = turn_start_angle;
            while ( start_norm > M_PI ) start_norm -= 2 * M_PI;
            while ( start_norm < -M_PI ) start_norm += 2 * M_PI;
            
            double current_norm = current_angle;
            while ( current_norm > M_PI ) current_norm -= 2 * M_PI;
            while ( current_norm < -M_PI ) current_norm += 2 * M_PI;
            
            double angle_diff = current_norm - start_norm;
            while ( angle_diff > M_PI ) angle_diff -= 2 * M_PI;
            while ( angle_diff < -M_PI ) angle_diff += 2 * M_PI;
            
            // Check if we've turned 90 degrees (pi/2 radians) clockwise
            if ( angle_diff >= M_PI/2 - 0.2 ) {
                // Finished turning 90 degrees clockwise
                turning_90 = false;
                random_turn_counter = 0;
                random_turn_duration = 30 + (rand() % 50);
                // Continue to normal movement below
            } else {
                // Continue turning 90 degrees clockwise in place (3x faster)
                track_velocity(0, 30.0);  // Turn clockwise (positive), 3x faster: 10.0 * 3 = 30.0
                return;
            }
        }
    
        // -------- CRITICAL ESCAPE MODE (Very close to wall or stuck) --------
        if ( distance < 30 || stuck_counter > 5 ) {
            
            // If stuck, start 90-degree clockwise turn
            if ( stuck_counter > 5 && !turning_90 ) {
                // Start 90-degree clockwise turn
                turning_90 = true;
                turn_start_angle = current_angle;
                stuck_counter = 0;
                // Turn clockwise in place (3x faster)
                track_velocity(0, 30.0);  // Turn clockwise (positive), 3x faster: 10.0 * 3 = 30.0
                return;
            } else if ( !turning_90 ) {
                // Very close to wall (but not stuck): reverse and turn aggressively (3x faster)
                double turn_dir = (rand()%2==0) ? 1.0 : -1.0;
                track_velocity(-50, turn_dir * 24.0);  // 3x faster turn: 8.0 * 3 = 24.0
                return;
            }
        }
    
        // -------- IMPROVED WALL AVOIDANCE (Detect earlier, turn more aggressively) --------
        if ( distance < 70 ) {  // Detect wall earlier (was 60)
            double turn_dir = (rand()%2==0) ? 1.0 : -1.0;
            
            // More aggressive turning: faster rotation (3x faster)
            track_velocity(10, turn_dir * 21.0);  // 3x faster turn: 7.0 * 3 = 21.0
    
            return;
        }
    
        // -------- RANDOM EXPLORATION --------
        random_turn_counter++;
    
        if ( random_turn_counter > random_turn_duration ) {
            random_direction = (rand()%2==0) ? 1.0 : -1.0;
            random_turn_duration = 40 + rand()%80;
            random_turn_counter = 0;
        }
    
        double forward_speed = 70 + rand()%30;
        double turn_speed = random_direction * 3 * (1.0 + (rand()%3));  // 3x faster: 3 * (1-3) = 3-9
    
        track_velocity(forward_speed, turn_speed);
    }
    void stop() {}

    private:
    int random_turn_counter;
    int random_turn_duration;
    double random_direction;  // Positive = right, negative = left
    int stuck_counter;
    double last_position_x;
    double last_position_y;
    bool turning_90;  // Flag to track if we're turning 90 degrees
    double turn_start_angle;  // Angle when turn started
    double turn_target_angle;  // Target angle after 90-degree turn

};

class Leader : public Agent {
    public:
    Leader(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    LeaderController c;
};

DECLARE_INTERFACE(Leader)

#endif