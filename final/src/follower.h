#ifndef __FOLLOWER_AGENT__H
#define __FOLLOWER_AGENT__H

#include "enviro.h"
#include <cmath>

using namespace enviro;

class FollowerController : public Process, public AgentInterface {

    public:

    // Respectful distance to maintain behind the leader
    double target_distance = 50;  // Closer distance to stay near leader
    double min_distance = 35;     // Minimum safe distance
    double max_distance = 70;     // Maximum distance before catching up
    int leader_id = 0;  // Leader is the first agent (id 0)
    
    // Stuck detection and 90-degree turn tracking
    int stuck_counter = 0;
    double last_position_x = 0;
    double last_position_y = 0;
    bool turning_90 = false;
    double turn_start_angle = 0;

    FollowerController() : Process(), AgentInterface() {}

    void init() {}

    void start() {
        last_position_x = x();
        last_position_y = y();
    }

    void update() {

        double current_x = x();
        double current_y = y();
        double current_angle = angle();
        
        // Check if stuck (not moving much) - same as leader
        double movement = sqrt((current_x-last_position_x)*(current_x-last_position_x) +
                               (current_y-last_position_y)*(current_y-last_position_y));
        
        if ( movement < 1.0 ) {  // Lower threshold for faster stuck detection
            stuck_counter++;
        } else {
            stuck_counter = 0;
        }
        
        last_position_x = current_x;
        last_position_y = current_y;

        // Use range finder to detect what's in front
        double range_distance = sensor_value(0);
        std::string detected_object = sensor_reflection_type(0);
        
        // Check for wall in front
        bool wall_detected = (range_distance < 40 && detected_object != "Leader");
        
        // Try to find leader position for navigation
        bool leader_found = false;
        double leader_x = 0, leader_y = 0;
        
        try {
            if ( agent_exists(leader_id) ) {
                Agent& leader = find_agent(leader_id);
                leader_x = leader.x();
                leader_y = leader.y();
                leader_found = true;
            }
        } catch (...) {
            leader_found = false;
        }
        
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
                // Continue to normal movement below
            } else {
                // Continue turning 90 degrees clockwise in place (3x faster)
                track_velocity(0, 30.0);  // Turn clockwise (positive), 3x faster: 10.0 * 3 = 30.0
                return;
            }
        }
        
        // If stuck, turn 90 degrees clockwise (same as leader)
        if ( stuck_counter > 5 && !turning_90 ) {
            // Start 90-degree clockwise turn
            turning_90 = true;
            turn_start_angle = current_angle;
            stuck_counter = 0;
            // Turn clockwise in place (3x faster)
            track_velocity(0, 30.0);  // Turn clockwise (positive), 3x faster: 10.0 * 3 = 30.0
            return;
        }

        // If leader found, always point toward leader
        if ( leader_found ) {
            double my_x = x();
            double my_y = y();
            double dx = leader_x - my_x;
            double dy = leader_y - my_y;
            double distance_to_leader = sqrt(dx*dx + dy*dy);
            
            // Calculate angle to leader
            double angle_to_leader = atan2(dy, dx);
            
            // Calculate angle difference (normalize to [-pi, pi])
            double angle_diff = angle_to_leader - current_angle;
            while ( angle_diff > M_PI ) angle_diff -= 2 * M_PI;
            while ( angle_diff < -M_PI ) angle_diff += 2 * M_PI;
            
            // If wall detected, avoid it while still trying to point to leader
            if ( wall_detected ) {
                // Turn away from wall but still try to point toward leader
                double turn_speed = 18.0;  // Fast turning
                if ( angle_diff > 0 ) {
                    // Need to turn right, but also avoid wall
                    track_velocity(45, turn_speed);  // Half speed: 90 * 0.5 = 45
                } else {
                    // Need to turn left, but also avoid wall
                    track_velocity(45, -turn_speed);  // Half speed: 90 * 0.5 = 45
                }
            }
            // If range finder detects the Leader
            else if ( detected_object == "Leader" ) {
                // Use range finder reading to maintain respectful distance
                // But always point toward leader
                double forward_speed;
                double turn_speed = angle_diff * 90.0;  // Proportional turning (3x faster: 30 * 3)
                
                if ( range_distance > max_distance ) {
                    // Too far from leader, catch up quickly
                    forward_speed = 105;  // Half speed: 210 * 0.5 = 105
                } else if ( range_distance < min_distance ) {
                    // Too close to leader, slow down
                    forward_speed = 37.5;  // Half speed: 75 * 0.5 = 37.5
                } else {
                    // Good distance - maintain it
                    double speed = (45 + (range_distance - target_distance) * 0.6) * 3 * 0.5;  // Half speed
                    if ( speed < 45 ) speed = 45;  // Half speed: 90 * 0.5 = 45
                    if ( speed > 90 ) speed = 90;  // Half speed: 180 * 0.5 = 90
                    forward_speed = speed;
                }
                
                track_velocity(forward_speed, turn_speed);
            }
            // Range finder doesn't see leader (leader is not directly ahead)
            else {
                // Always point toward leader and move toward it
                double forward_speed;
                double turn_speed = angle_diff * 90.0;  // Proportional turning (3x faster)
                
                if ( distance_to_leader > max_distance ) {
                    // Too far, catch up quickly
                    forward_speed = 112.5;  // Half speed: 225 * 0.5 = 112.5
                } else if ( distance_to_leader < min_distance ) {
                    // Too close, slow down
                    forward_speed = 37.5;  // Half speed: 75 * 0.5 = 37.5
                } else {
                    // Good distance, maintain following
                    forward_speed = 82.5;  // Half speed: 165 * 0.5 = 82.5
                }
                
                track_velocity(forward_speed, turn_speed);
            }
        }
        // No leader found - move forward to search
        else if ( wall_detected ) {
            // Just turn away from wall
            track_velocity(37.5, 21.0);  // Half speed: 75 * 0.5 = 37.5
        } else {
            track_velocity(75, 0);  // Half speed: 150 * 0.5 = 75
        }

    }

    void stop() {}

};

class Follower : public Agent {
    public:
    Follower(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    FollowerController c;
};

DECLARE_INTERFACE(Follower)

#endif