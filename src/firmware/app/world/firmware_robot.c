#include "firmware/app/world/firmware_robot.h"

#include <math.h>
#include <stdlib.h>

// #include "firmware/app/control/control.h"

typedef void (*TrajectoryFollower_t)(FirmwareRobot_t*, PositionTrajectory_t);
typedef void (*ApplyDirectPerWheelPower_t)(FirmwareRobot_t*, 
    TbotsProto_DirectControlPrimitive_DirectPerWheelControl);
typedef void (*SetLocalVelocity_t)(
    FirmwareRobot_t*, TbotsProto_DirectControlPrimitive_DirectVelocityControl);
typedef void (*StopRobot_t)(FirmwareRobot_t*, TbotsProto_StopPrimitive_StopType);

struct FirmwareRobot
{
    // NOTE: Everything here is in the global field reference frame (ie. 0,0 is the center
    // of the field, 0 degrees is towards the enemy goal) unless otherwise specified.
    Charger_t* charger;
    Chicker_t* chicker;
    Dribbler_t* dribbler;
    float (*get_robot_position_x)(void);
    float (*get_robot_position_y)(void);
    float (*get_robot_orientation)(void);
    float (*get_robot_velocity_x)(void);
    float (*get_robot_velocity_y)(void);
    float (*get_robot_velocity_angular)(void);
    float (*get_battery_voltage)(void);
    TrajectoryFollower_t follow_pos_trajectory;
    ApplyDirectPerWheelPower_t apply_direct_per_wheel_power;
    SetLocalVelocity_t set_local_velocity;
    StopRobot_t stop_robot;
    ControllerState_t* controller_state;
    VelocityWheel_t* front_right_velocity_wheel;
    VelocityWheel_t* front_left_velocity_wheel;
    VelocityWheel_t* back_right_velocity_wheel;
    VelocityWheel_t* back_left_velocity_wheel;
    ForceWheel_t* front_right_force_wheel;
    ForceWheel_t* front_left_force_wheel;
    ForceWheel_t* back_right_force_wheel;
    ForceWheel_t* back_left_force_wheel;
    RobotConstants_t robot_constants;
};

FirmwareRobot_t* app_firmware_robot_velocity_wheels_create(
    Charger_t* charger, Chicker_t* chicker, Dribbler_t* dribbler,
    float (*get_robot_position_x)(void), float (*get_robot_position_y)(void),
    float (*get_robot_orientation)(void), float (*get_robot_velocity_x)(void),
    float (*get_robot_velocity_y)(void), float (*get_robot_velocity_angular)(void),
    float (*get_battery_voltage)(void), VelocityWheel_t* front_right_wheel,
    VelocityWheel_t* front_left_wheel, VelocityWheel_t* back_right_wheel,
    VelocityWheel_t* back_left_wheel, ControllerState_t* controller_state,
    RobotConstants_t robot_constants)
{
    FirmwareRobot_t* new_robot = malloc(sizeof(FirmwareRobot_t));

    new_robot->charger                    = charger;
    new_robot->chicker                    = chicker;
    new_robot->dribbler                   = dribbler;
    new_robot->get_robot_position_x       = get_robot_position_x;
    new_robot->get_robot_position_y       = get_robot_position_y;
    new_robot->get_robot_orientation      = get_robot_orientation;
    new_robot->get_robot_velocity_x       = get_robot_velocity_x;
    new_robot->get_robot_velocity_y       = get_robot_velocity_y;
    new_robot->get_robot_velocity_angular = get_robot_velocity_angular;
    new_robot->get_battery_voltage        = get_battery_voltage;
    new_robot->follow_pos_trajectory        = follow_pos_trajectory_velocity;
    new_robot->apply_direct_per_wheel_power = apply_direct_per_velocity_wheel_velocity;
    new_robot->set_local_velocity           = set_local_velocity_wheels_velocity;
    new_robot->stop_robot                   = stop_velocity_wheels;
    new_robot->front_right_velocity_wheel            = front_right_wheel;
    new_robot->front_left_velocity_wheel             = front_left_wheel;
    new_robot->back_right_velocity_wheel             = back_right_wheel;
    new_robot->back_left_velocity_wheel              = back_left_wheel;
    new_robot->front_right_force_wheel            = NULL;
    new_robot->front_left_force_wheel             = NULL;
    new_robot->back_right_force_wheel             = NULL;
    new_robot->back_left_force_wheel              = NULL;
    new_robot->robot_constants              = robot_constants;
    new_robot->controller_state             = controller_state;

    return new_robot;
}

FirmwareRobot_t* app_firmware_robot_force_wheels_create(
    Charger_t* charger, Chicker_t* chicker, Dribbler_t* dribbler,
    float (*get_robot_position_x)(void), float (*get_robot_position_y)(void),
    float (*get_robot_orientation)(void), float (*get_robot_velocity_x)(void),
    float (*get_robot_velocity_y)(void), float (*get_robot_velocity_angular)(void),
    float (*get_battery_voltage)(void), ForceWheel_t* front_right_wheel,
    ForceWheel_t* front_left_wheel, ForceWheel_t* back_right_wheel,
    ForceWheel_t* back_left_wheel, ControllerState_t* controller_state,
    RobotConstants_t robot_constants)
{
    FirmwareRobot_t* new_robot = malloc(sizeof(FirmwareRobot_t));

    new_robot->charger                    = charger;
    new_robot->chicker                    = chicker;
    new_robot->dribbler                   = dribbler;
    new_robot->get_robot_position_x       = get_robot_position_x;
    new_robot->get_robot_position_y       = get_robot_position_y;
    new_robot->get_robot_orientation      = get_robot_orientation;
    new_robot->get_robot_velocity_x       = get_robot_velocity_x;
    new_robot->get_robot_velocity_y       = get_robot_velocity_y;
    new_robot->get_robot_velocity_angular = get_robot_velocity_angular;
    new_robot->get_battery_voltage        = get_battery_voltage;
    new_robot->follow_pos_trajectory        = follow_pos_trajectory_force;
    new_robot->apply_direct_per_wheel_power = apply_direct_per_force_wheel_velocity;
    new_robot->set_local_velocity           = set_local_force_wheels_velocity;
    new_robot->stop_robot                   = stop_force_wheels;
    new_robot->front_right_velocity_wheel            = NULL;
    new_robot->front_left_velocity_wheel             = NULL;
    new_robot->back_right_velocity_wheel             = NULL;
    new_robot->back_left_velocity_wheel              = NULL;
    new_robot->front_right_force_wheel            = front_right_wheel;
    new_robot->front_left_force_wheel             = front_left_wheel;
    new_robot->back_right_force_wheel             = back_right_wheel;
    new_robot->back_left_force_wheel              = back_left_wheel;
    new_robot->robot_constants              = robot_constants;
    new_robot->controller_state             = controller_state;

    return new_robot;
}

void app_firmware_robot_destroy(FirmwareRobot_t* robot)
{
    free(robot);
}

Charger_t* app_firmware_robot_getCharger(const FirmwareRobot_t* robot)
{
    return robot->charger;
}

Chicker_t* app_firmware_robot_getChicker(const FirmwareRobot_t* robot)
{
    return robot->chicker;
}

Dribbler_t* app_firmware_robot_getDribbler(const FirmwareRobot_t* robot)
{
    return robot->dribbler;
}

float app_firmware_robot_getPositionX(const FirmwareRobot_t* robot)
{
    return robot->get_robot_position_x();
}

float app_firmware_robot_getPositionY(const FirmwareRobot_t* robot)
{
    return robot->get_robot_position_y();
}

float app_firmware_robot_getOrientation(const FirmwareRobot_t* robot)
{
    return robot->get_robot_orientation();
}

float app_firmware_robot_getVelocityX(const FirmwareRobot_t* robot)
{
    return robot->get_robot_velocity_x();
}

float app_firmware_robot_getVelocityY(const FirmwareRobot_t* robot)
{
    return robot->get_robot_velocity_y();
}

float app_firmware_robot_getVelocityAngular(const FirmwareRobot_t* robot)
{
    return robot->get_robot_velocity_angular();
}

float app_firmware_robot_getSpeedLinear(const FirmwareRobot_t* robot)
{
    const float vx = app_firmware_robot_getVelocityX(robot);
    const float vy = app_firmware_robot_getVelocityY(robot);

    return sqrtf(powf(vx, 2.0f) + powf(vy, 2.0f));
}

float app_firmware_robot_getBatteryVoltage(const FirmwareRobot_t* robot)
{
    return robot->get_battery_voltage();
}

RobotConstants_t app_firmware_robot_getRobotConstants(const FirmwareRobot_t* robot)
{
    return robot->robot_constants;
}

ControllerState_t* app_firmware_robot_getControllerState(const FirmwareRobot_t* robot)
{
    return robot->controller_state;
}

void follow_pos_trajectory_force(FirmwareRobot_t* robot, PositionTrajectory_t pos_trajectory)
{
    // TODO: Implement with planner
}

void follow_pos_trajectory_velocity(FirmwareRobot_t* robot, PositionTrajectory_t pos_trajectory, size_t trajectory_index)
{
    VelocityWheel_t* front_right_wheel = robot->front_right_velocity_wheel;
    VelocityWheel_t* front_left_wheel = robot->front_left_velocity_wheel;
    VelocityWheel_t* back_right_wheel = robot->back_right_velocity_wheel;
    VelocityWheel_t* back_left_wheel = robot->back_left_velocity_wheel;

    // Convert position trajectory to velocity trajectory
    VelocityTrajectory_t velocity_trajectory;    
    // TODO: Extract # of elements properly    
    unsigned int num_elements = 5;    
    app_trajectory_planner_generateVelocityTrajectory(pos_trajectory,     
        num_elements, velocity_trajectory);    
    
    // Get target velocity in next timestep     
    // TODO: Original implementation compares vs current_time    
    while (trajectory_index < num_elements - 1)    
    {        
        trajectory_index++;    
    }    
    x_velocity = velocity_trajectory.x_velocity[trajectory_index];    
    y_velocity = velocity_trajectory.y_velocity[trajectory_index];    
    angular_velocity = velocity_trajectory.angular_velocity[trajectory_index];    
    
    // TODO: Convert to per wheel velocity

    // TODO: Probably need to convert from RPM to Force
    app_velocity_wheel_setTargetVelocity(robot, front_left_wheel, front_left_wheel_velocity);
}

void apply_direct_per_force_wheel_velocity(FirmwareRobot_t* robot, TbotsProto_DirectControlPrimitive_DirectPerWheelControl control_msg)
{
    ForceWheel_t* front_right_wheel = robot->front_right_force_wheel;
    ForceWheel_t* front_left_wheel = robot->front_left_force_wheel;
    ForceWheel_t* back_right_wheel = robot->back_right_force_wheel;
    ForceWheel_t* back_left_wheel = robot->back_left_force_wheel;

    // TODO: Convert RPM to force
    app_force_wheel_applyForce(front_right_wheel, control_msg.front_right_wheel_rpm);
    app_force_wheel_applyForce(front_left_wheel, control_msg.front_left_wheel_rpm);
    app_force_wheel_applyForce(back_right_wheel, control_msg.back_right_wheel_rpm);
    app_force_wheel_applyForce(back_left_wheel, control_msg.back_left_wheel_rpm);
}

void apply_direct_per_velocity_wheel_velocity(FirmwareRobot_t* robot, TbotsProto_DirectControlPrimitive_DirectPerWheelControl control_msg)
{
    VelocityWheel_t* front_right_wheel = robot->front_right_velocity_wheel;
    VelocityWheel_t* front_left_wheel = robot->front_left_velocity_wheel;
    VelocityWheel_t* back_right_wheel = robot->back_right_velocity_wheel;
    VelocityWheel_t* back_left_wheel = robot->back_left_velocity_wheel;

    app_velocity_wheel_setTargetVelocity(front_right_wheel, control_msg.front_right_wheel_rpm);
    app_velocity_wheel_setTargetVelocity(front_left_wheel, control_msg.front_left_wheel_rpm);
    app_velocity_wheel_setTargetVelocity(back_right_wheel, control_msg.back_right_wheel_rpm);
    app_velocity_wheel_setTargetVelocity(back_left_wheel, control_msg.back_left_wheel_rpm);
}

void set_local_force_wheels_velocity(FirmwareRobot_t* robot, TbotsProto_DirectControlPrimitive_DirectVelocityControl control_msg)
{
    ForceWheel_t* front_right_wheel = robot->front_right_force_wheel;
    ForceWheel_t* front_left_wheel = robot->front_left_force_wheel;
    ForceWheel_t* back_right_wheel = robot->back_right_force_wheel;
    ForceWheel_t* back_left_wheel = robot->back_left_force_wheel;

    float linear_velocity_x = control_msg.velocity.x_component_meters;
    float linear_velocity_y = control_msg.velocity.y_component_meters;
    float angular_velocity = control_msg.angular_velocity.radians_per_second;

    app_control_trackVelocityInRobotFrame(robot, front_left_wheel, front_right_wheel,
        back_left_wheel, back_right_wheel, linear_velocity_x, 
        linear_velocity_y, angular_velocity);
}

void set_local_velocity_wheels_velocity(FirmwareRobot_t* robot, TbotsProto_DirectControlPrimitive_DirectVelocityControl control_msg)
{
    VelocityWheel_t* front_right_wheel = robot->front_right_velocity_wheel;
    VelocityWheel_t* front_left_wheel = robot->front_left_velocity_wheel;
    VelocityWheel_t* back_right_wheel = robot->back_right_velocity_wheel;
    VelocityWheel_t* back_left_wheel = robot->back_left_velocity_wheel;

    float linear_velocity_x = control_msg.velocity.x_component_meters;
    float linear_velocity_y = control_msg.velocity.y_component_meters;
    float angular_velocity = control_msg.angular_velocity.radians_per_second;

    // TODO: Call `app_velocity_wheel_setTargetVelocity`
}

void stop_force_wheels(FirmwareRobot_t* robot, TbotsProto_StopPrimitive_StopType stop_type)
{
    ForceWheel_t* front_right_wheel = robot->front_right_force_wheel;
    ForceWheel_t* front_left_wheel = robot->front_left_force_wheel;
    ForceWheel_t* back_right_wheel = robot->back_right_force_wheel;
    ForceWheel_t* back_left_wheel = robot->back_left_force_wheel;

    if (stop_type == TbotsProto_StopPrimitive_StopType_COAST)
    {
        app_force_wheel_coast(front_right_wheel);
        app_force_wheel_coast(front_left_wheel);
        app_force_wheel_coast(back_right_wheel);
        app_force_wheel_coast(back_left_wheel);
    }
    else
    {
        app_force_wheel_brake(front_right_wheel);
        app_force_wheel_brake(front_left_wheel);
        app_force_wheel_brake(back_right_wheel);
        app_force_wheel_brake(back_left_wheel);
    }
}

void stop_velocity_wheels(FirmwareRobot_t* robot, TbotsProto_StopPrimitive_StopType stop_type)
{
    VelocityWheel_t* front_right_wheel = robot->front_right_velocity_wheel;
    VelocityWheel_t* front_left_wheel = robot->front_left_velocity_wheel;
    VelocityWheel_t* back_right_wheel = robot->back_right_velocity_wheel;
    VelocityWheel_t* back_left_wheel = robot->back_left_velocity_wheel;

    if (stop_type == TbotsProto_StopPrimitive_StopType_COAST)
    {
        app_velocity_wheel_coast(front_right_wheel);
        app_velocity_wheel_coast(front_left_wheel);
        app_velocity_wheel_coast(back_right_wheel);
        app_velocity_wheel_coast(back_left_wheel);
    }
    else
    {
        app_velocity_wheel_brake(front_right_wheel);
        app_velocity_wheel_brake(front_left_wheel);
        app_velocity_wheel_brake(back_right_wheel);
        app_velocity_wheel_brake(back_left_wheel);
    }
}

void app_firmware_robot_follow_pos_trajectory(FirmwareRobot_t* robot, PositionTrajectory_t pos_trajectory)
{
    if (wheel_type == FORCE_WHEEL) {
        robot->follow_pos_trajectory_force(robot, pos_trajectory)
    }
    else {
        robot->follow_pos_trajectory_velocity(robot, pos_trajectory)
    }
}

void app_firmware_robot_apply_direct_per_wheel_power(FirmwareRobot_t* robot, TbotsProto_DirectControlPrimitive_DirectPerWheelControl control_msg)
{
    if (wheel_type == FORCE_WHEEL) {
        robot->apply_direct_per_wheel_force(robot, control_msg)
    }
    else {
        robot->apply_direct_per_wheel_velocity(robot, control_msg)
    }
}

void app_firmware_robot_set_local_velocity(FirmwareRobot_t* robot, TbotsProto_DirectControlPrimitive_DirectVelocityControl control_msg)
{
    if (wheel_type == FORCE_WHEEL) {
        robot->set_local_force_wheels_velocity(robot, control_msg)
    }
    else {
        robot->set_local_velocity_wheels_velocity(robot, control_msg)
    }
}

void app_firmware_robot_stop(FirmwareRobot_t* robot, TbotsProto_StopPrimitive_StopType stop_type, Robot_Wheel_Type wheel_type)
{
    if (wheel_type == FORCE_WHEEL) {
        robot->stop_force_wheels(robot, stop_type)
    }
    else {
        robot->stop_velocity_wheels(robot, stop_type)
    }
}
