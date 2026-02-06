#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "api.h"
// #include "pros/colors.hpp"
#include "pros/llemu.hpp"
#include "pros/rtos.hpp"
#include "pros/screen.h"
#include "pros/screen.hpp"
#include <cstdio>
// #include "pros/screen.h"
// #include "pros/screen.hpp"
// controller



int wingState=0; // variable for wing
int tongueState=0; // variable for tongue
int middleState=0; // variable for middle score
int retainerState=0; // variable for retainer


pros::Controller controller(pros::E_CONTROLLER_MASTER); // defines the controller

pros::MotorGroup leftMotors({1, -6, 4}); // left motors on ports 13, 12, 11
pros::MotorGroup rightMotors({-9, 10, -21}); // right motors on ports 20, 19, 18

pros::Motor LeftIntake(5);
pros::Motor RightIntake(-20);
pros::adi::DigitalOut wingPiston('C');
pros::adi::DigitalOut tonguePiston('B');
pros::adi::DigitalOut middlePiston('A');
pros::adi::DigitalOut retainerPiston('D');

// create an imu (inertial) on port 15
pros::Imu imu(3);

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor, port 9, not reversed
pros::Rotation horizontalEnc(18);
// vertical tracking wheel encoder. Rotation sensor, port 5, not reversed
pros::Rotation leftEnc(11);
// vertical tracking wheel encoder. Rotation sensor, port 4, reversed
pros::Rotation rightEnc(-19);
// horizontal tracking wheel. 2.75" diameter, 4.5" offset, back of the robot (negative)
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_2, -4.5);
// vertical tracking wheel. 2.75" diameter, 5.25" offset, left of the robot (negative)
lemlib::TrackingWheel right(&rightEnc, lemlib::Omniwheel::NEW_275, 5.25);
lemlib::TrackingWheel left(&leftEnc, lemlib::Omniwheel::NEW_275, -5.25);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              10.5, // 10 inch track width
                              lemlib::Omniwheel::NEW_275, // using new 4" omnis
                              600, // drivetrain rpm is 360
                              2 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearcontroller(0.2, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            0, // anti windup
                                            0.25, // small error range, in inches,
                                            100, // small error range timeout, in milliseconds
                                            2, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            0 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularcontroller(0.88, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             1.54, // derivative gain (kD)
                                             0, // anti windup
                                             0.5, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             2, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(&right, // vertical tracking wheel
                            &left, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

//input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearcontroller, angularcontroller, sensors, &throttleCurve, &steerCurve);


touchscreen::screen* activeScreen;


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

 void initialize() {
     pros::lcd::initialize();
     
     chassis.calibrate(); // calibrate sensors
     retainerPiston.set_value(true); // start with retainer down

     activeScreen=touchscreen::autonScreen;
     
     
    pros::screen::touch_callback([](){activeScreen->onPress();}, pros::last_touch_e_t::E_TOUCH_RELEASED);

    std::printf("smeggy \n");
    pros::Task screenTask([](){
        while(67==tuff_asf_boi){
            pros::screen::erase();
            activeScreen->draw();
            pros::delay(10000);
        }
    });
        
    // std::printf("rizzy \n");
    
        
    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
}

/**
 * Runs while the robot is disabled
 */
void disabled() {
}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

// get a path used for pure pursuit 

// this needs to be put outside a function
ASSET(susdiddytuahohioblud_txt); // '.' replaced with "_" to make c++ happy
ASSET(tuah_txt); // '.' replaced with "_" to make c++ happy

void ram(float speed){
    do{
        rightMotors.move(speed);
        leftMotors.move(speed);
    }while(abs(leftEnc.get_velocity())>1000&&abs(rightEnc.get_velocity())>1000);
    rightMotors.brake();
    
    leftMotors.brake();
}

/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */
// enum autonRoute{
//     skills,
//     leftSide,
//     rightSide
// };
// autonRoute selectedAuton=skills;
void autonomous() {

    pros::Task adsjfdsjf([](){while(1){std::printf("x %.2f   y %.2f   t %.2f\n",chassis.getPose().x,chassis.getPose().y,chassis.getPose().theta); pros::delay(100);}});

    chassis.setPose(0, 0, 0);

    // chassis.turnToHeading(90, 9000);
    // chassis.moveToPose(0, 24, 0, 9000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=20});

    std::printf("gurt\n");

    // SIGNATURE WIN POINT

    // // Moves to collect the cluster of three blocks
    // retainerPiston.set_value(true);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // chassis.moveToPose(18.000, 25.490, 42.850, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=40});
    // pros::delay(500);

    // // Move to align with long goal and score cluster
    // chassis.moveToPose(43.500, -2.500, 110.377, 2500, {.forwards=true,.maxSpeed=127,.minSpeed=45});
    // chassis.turnToHeading(180.138, 1500);
    // chassis.moveToPose(37.500, 18.000, 180.000, 1000, {.forwards=false,.lead=0.2,.maxSpeed=127,.minSpeed=45});
    // retainerPiston.set_value(false);
    // pros::delay(2500);
    // LeftIntake.move_velocity(0);
    // RightIntake.move_velocity(0);
    // retainerPiston.set_value(true);

    // // Move forward and turn to face next block cluster
    // chassis.moveToPose(39.500, 2.490, 180.000, 2000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=20});
    // chassis.moveToPose(12.463, 20.490, -45.000, 1000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=20});
    // LeftIntake.move_velocity(100);
    // RightIntake.move_velocity(100);

    // // Collect other cluster and score in High Middle Goal
    // chassis.moveToPose(-46.878, 23.685, -90.452, 3000, {.forwards=true,.maxSpeed=127,.minSpeed=20});
    // chassis.turnToHeading(-131.138, 1500);
    // chassis.moveToPose(-26.500, 32.000, -131.138, 1300, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    // LeftIntake.move_velocity(-300);
    // RightIntake.move_velocity(-300);
    // pros::delay(250);
    // middlePiston.set_value(true);
    // LeftIntake.move_velocity(400);
    // RightIntake.move_velocity(400);
    // pros::delay(2500);

    // // Move towards loader and collect alliance's colored blocks
    // LeftIntake.move_velocity(0);
    // RightIntake.move_velocity(0);
    // middlePiston.set_value(false);
    // chassis.moveToPose(-65.285, 1.045, -131.138, 2000, {.forwards=true,.lead=0,.maxSpeed=50,.minSpeed=20});
    // chassis.turnToHeading(-172.032, 2000);
    // tonguePiston.set_value(true);
    // LeftIntake.move_velocity(400);
    // RightIntake.move_velocity(400);
    // chassis.moveToPose(-64.071, -28.750, -171.281, 2000, {.forwards=true,.maxSpeed=127,.minSpeed=20});

    // // Score blocks from loader into long goal
    // pros::delay(1500);
    // LeftIntake.move_velocity(0);
    // RightIntake.move_velocity(0);
    // chassis.moveToPose(-60.750, 15.904, -173.217, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=20});
    // retainerPiston.set_value(false);
    // tonguePiston.set_value(false);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);


    // NEW LEFT SIDE
    // // Collect cluster
    // retainerPiston.set_value(true);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // chassis.moveToPose(-21.500, 29.465, -49.452, 1850, {.forwards=true,.maxSpeed=127,.minSpeed=50});
    // pros::delay(750);
    // chassis.moveToPose(-18.500, 25.565, -49.452, 950, {.forwards=false,.maxSpeed=127,.minSpeed=50});


    // // Score in High Middle Goal
    // chassis.turnToHeading(-136.138, 750);
    // chassis.moveToPose(-2.300, 30.000, -136.138, 1500, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=47});
    // LeftIntake.move_velocity(-380);
    // RightIntake.move_velocity(-380);
    // pros::delay(285);
    // // retainerPiston.set_value(false);
    // middlePiston.set_value(true);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // pros::delay(1850);

    // // Move towards loader and collect alliance's colored blocks
    // tonguePiston.set_value(true);
    // // retainerPiston.set_value(true);
    // middlePiston.set_value(false);
    // chassis.moveToPose(-39.000, -2.950, -141.138, 2450, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=50});
    // chassis.turnToHeading(-185.032, 925);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // chassis.moveToPose(-37.400, -28.800, -178.781, 1050, {.forwards=true,.maxSpeed=127,.minSpeed=45});
    // // chassis.moveToPose(-37.400, -27.500, -178.781, 450, {.forwards=true,.maxSpeed=127,.minSpeed=43});
    // // chassis.moveToPose(-37.400, -28.800, -178.781, 500, {.forwards=true,.maxSpeed=127,.minSpeed=43});

    // // Score blocks from loader into long goal
    // pros::delay(275);
    // chassis.moveToPose(-39.500, 10.904, -180.500, 1500, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=50});
    // LeftIntake.move_velocity(-380);
    // RightIntake.move_velocity(-380);
    // pros::delay(150);
    // retainerPiston.set_value(false);
    // tonguePiston.set_value(false);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // pros::delay(2000);
    // LeftIntake.move_velocity(-380);
    // RightIntake.move_velocity(-380);
    // pros::delay(150);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);


    // NEW RIGHT SIDE

    // // Moves to collect the cluster of three blocks
    // wingPiston.set_value(true);
    // retainerPiston.set_value(true);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // chassis.moveToPose(22.463, 28.490, 54.850, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=45});

    // // Move to point in between the loader and long goal, then turn to line up with the loader and extend the tongue
    // chassis.moveToPose(37.247, -10.731, 131.000, 2000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // chassis.turnToHeading(182.000, 1500);
    // tonguePiston.set_value(true);
    // pros::delay(500);

    // // Moves into the loader to unload and store the alliance's three colored blocks
    // chassis.moveToPose(35.229, -23.900, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=50});
    // // chassis.moveToPose(35.229, -23.000, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // // chassis.moveToPose(35.229, -24.000, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // pros::delay(500);

    // // Moves to long goal to score the three blocks from the loader
    // chassis.moveToPose(36.750, 9.655, 181.504, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=50});
    // tonguePiston.set_value(false);
    // retainerPiston.set_value(false);
    // pros::delay(1000);
    // LeftIntake.move_velocity(-400);
    // RightIntake.move_velocity(-400);
    // pros::delay(250);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // pros::delay(2500);

    // Lines up the wing and gets control zone in long goal
    // chassis.moveToPose(35.229, -13.900, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=50});
    // chassis.moveToPose(24.325, 14.377, 168.279, 9000, {.forwards=false,.maxSpeed=127,.minSpeed=20});
    // chassis.moveToPose(22.000, 18.377, 168.279, 9000, {.forwards=false,.maxSpeed=127,.minSpeed=20});
    // wingPiston.set_value(false);
    // chassis.moveToPose(22.325, 23.377, 159.279, 9000, {.forwards=false,.maxSpeed=127,.minSpeed=20});


    // OLD RIGHT SIDE
    
    // // Moves to collect the cluster of three blocks
    // retainerPiston.set_value(true);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // chassis.moveToPose(18.463, 25.490, 34.850, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=20});
    // pros::delay(500);

    // // Move to score cluster of three and preload in low goal
    // chassis.moveToPose(-5.200, 35.650, -45.000, 2000, {.forwards=true,.maxSpeed=127,.minSpeed=20});
    // LeftIntake.move_velocity(-60);
    // RightIntake.move_velocity(-60);
    // pros::delay(2000);
    // LeftIntake.move_velocity(0);
    // RightIntake.move_velocity(0);

    // // Move to point in between the loader and long goal, then turn to line up with the loader and extend the tongue
    // chassis.moveToPose(32.247, 0.731, -51.862, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    // chassis.turnToHeading(182.000, 1500);
    // tonguePiston.set_value(true);
    // pros::delay(500);

    // // Moves into the loader to unload and store the alliance's three colored blocks
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // chassis.moveToPose(35.229, -22.500, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // pros::delay(500);

    // // Moves to long goal to score the three blocks from the loader
    // chassis.moveToPose(35.500, 9.655, 183.504, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=45});
    // tonguePiston.set_value(false);
    // retainerPiston.set_value(false);


    // OLD LEFT SIDE

    // // Moves to collect the cluster of three blocks
    // retainerPiston.set_value(true);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // chassis.moveToPose(3.500, 31.500, 5.000, 3000, {.forwards=true,.lead=0.7,.maxSpeed=127,.minSpeed=45});
    // pros::delay(200);

    // // Moves to a point in between the loader and the long goal, then turns to line up with the loader
    // chassis.moveToPose(-39.500, 11.500, -110.377, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=45});
    // chassis.turnToHeading(-150.000, 2000);

    // // Extends the tongue and moves into the loader to store the alliance's three colored blocks
    // tonguePiston.set_value(true);
    // chassis.moveToPose(-39.000, -2.750, -150.000, 1850, {.forwards=true,.lead=0,.maxSpeed=40,.minSpeed=10});
    // pros::delay(300);

    // // Moves back to the long goal and scores the three blocks
    // chassis.moveToPose(-25.500, 25.000, -148.000, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=45});
    // tonguePiston.set_value(false);
    // retainerPiston.set_value(false);


    // MOVE FORWARD (JUST IN CASE LOWK)

    // chassis.moveToPose(0, 6, 0, 9000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});


    // OLD SKILLS

    // // Backup for if it doesn't work
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);

    // // Moves to collect the cluster of three blocks
    // wingPiston.set_value(true);
    // retainerPiston.set_value(true);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // chassis.moveToPose(22.463, 28.490, 54.850, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=45});

    // // Move to point in between the loader and long goal, then turn to line up with the loader and extend the tongue
    // chassis.moveToPose(37.247, -10.731, 131.000, 2000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // chassis.turnToHeading(182.000, 1500);
    // tonguePiston.set_value(true);
    // pros::delay(500);

    // // Moves into the loader to unload and store the blocks
    // chassis.moveToPose(35.229, -23.900, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=50});
    // // chassis.moveToPose(35.229, -23.000, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // // chassis.moveToPose(35.229, -24.000, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // pros::delay(500);

    // // Moves to long goal to score the blocks from the loader
    // chassis.moveToPose(38.000, 9.655, 181.504, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=50});
    // tonguePiston.set_value(false);
    // retainerPiston.set_value(false);
    // pros::delay(1000);
    // LeftIntake.move_velocity(-400);
    // RightIntake.move_velocity(-400);
    // pros::delay(250);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // pros::delay(3000);

    // // Moves into the loader to unload and store the blocks
    // retainerPiston.set_value(true);
    // tonguePiston.set_value(true);
    // chassis.moveToPose(35.229, -23.900, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=50});
    // // chassis.moveToPose(35.229, -23.000, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // // chassis.moveToPose(35.229, -24.000, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // pros::delay(500);

    // // Moves to long goal to score the blocks from the loader
    // chassis.moveToPose(38.000, 9.655, 181.504, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=50});
    // tonguePiston.set_value(false);
    // retainerPiston.set_value(false);
    // pros::delay(1000);
    // LeftIntake.move_velocity(-400);
    // RightIntake.move_velocity(-400);
    // pros::delay(250);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // pros::delay(3000);

    // // Move forward and turn to face next block cluster
    // retainerPiston.set_value(true);
    // wingPiston.set_value(false);
    // chassis.moveToPose(39.500, 2.490, 180.000, 2000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    // chassis.moveToPose(-10, 0, 180, 9000,{.forwards=true,.maxSpeed=127,.minSpeed=35});
    // chassis.moveToPose(-10, -100, 180, 9000,{.forwards=true,.maxSpeed=127,.minSpeed=127});


    // chassis.moveToPose(12.463, 20.490, -45.000, 1000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=20});
    // LeftIntake.move_velocity(100);
    // RightIntake.move_velocity(100);

    // // Collect other cluster and score in High Middle Goal
    // chassis.moveToPose(-55.878, 23.685, -90.452, 3000, {.forwards=true,.maxSpeed=127,.minSpeed=20});
    // chassis.turnToHeading(-131.138, 1500);
    // chassis.moveToPose(-26.500, 32.000, -131.138, 1300, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    // LeftIntake.move_velocity(-300);
    // RightIntake.move_velocity(-300);
    // pros::delay(250);
    // middlePiston.set_value(true);
    // LeftIntake.move_velocity(400);
    // RightIntake.move_velocity(400);
    // pros::delay(2500);

    // // Move towards loader and collect alliance's colored blocks
    // LeftIntake.move_velocity(0);
    // RightIntake.move_velocity(0);
    // middlePiston.set_value(false);
    // chassis.moveToPose(-65.285, 1.045, -131.138, 2000, {.forwards=true,.lead=0,.maxSpeed=50,.minSpeed=20});
    // chassis.turnToHeading(-172.032, 2000);
    // tonguePiston.set_value(true);
    // LeftIntake.move_velocity(400);
    // RightIntake.move_velocity(400);
    // chassis.moveToPose(-64.071, -28.750, -171.281, 2000, {.forwards=true,.maxSpeed=127,.minSpeed=20});

    // // Score blocks from loader into long goal
    // pros::delay(1500);
    // LeftIntake.move_velocity(0);
    // RightIntake.move_velocity(0);
    // chassis.moveToPose(-60.750, 15.904, -173.217, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=20});
    // retainerPiston.set_value(false);
    // tonguePiston.set_value(false);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);


    // NEW SKILLS

    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    retainerPiston.set_value(true);
    chassis.moveToPose(0, 10, 0, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    chassis.moveToPose(38.047, -4.000, 170.000, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=35});
    // chassis.turnToHeading(180, 1000);

    tonguePiston.set_value(true);
    chassis.moveToPose(35.000, -30.200, 185.000, 4500, {.forwards=true,.lead=0.2,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(35.000, -28.000, 185.000, 750, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(35.000, -30.200, 185.000, 1000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    LeftIntake.move_velocity(0);
    RightIntake.move_velocity(0);
    chassis.moveToPose(35.500, -11.000, 185.000, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    tonguePiston.set_value(false);

    chassis.turnToHeading(-90, 1000);
    chassis.moveToPose(28.750, -10.250, -90.000, 750, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(28.000, 30.731, 4.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(28.500, 74.731, 4.000, 3000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    chassis.turnToHeading(75, 1000);

    chassis.moveToPose(43.547, 90.731, 0.000, 4000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(43.500, 72.000, 0.000, 1500, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    tonguePiston.set_value(true);
    retainerPiston.set_value(false);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    pros::delay(1000);
    LeftIntake.move_velocity(-300);
    RightIntake.move_velocity(-300);
    pros::delay(250);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    pros::delay(3500);
    retainerPiston.set_value(true);

    chassis.moveToPose(44.500, 112.500, 4.000, 4000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    pros::delay(2000);
    chassis.moveToPose(43.047, 70.500, 4.000, 2500, {.forwards=false,.lead=0.2,.maxSpeed=127,.minSpeed=35});
    retainerPiston.set_value(false);
    pros::delay(1000);
    LeftIntake.move_velocity(-300);
    RightIntake.move_velocity(-300);
    pros::delay(250);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    pros::delay(3500);
    retainerPiston.set_value(true);
    tonguePiston.set_value(false);

    LeftIntake.move_velocity(0);
    RightIntake.move_velocity(0);
    chassis.moveToPose(42.047, 77.731, 1.000, 2500, {.forwards=false,.lead=0.2,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(-13.247, 81.731, -90.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    chassis.moveToPose(-58.247, 82.731, -90.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    tonguePiston.set_value(true);

    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    chassis.moveToPose(-53.247, 115.731, 0.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=35});
    // chassis.moveToPose(-53.247, 66.731, 0.000, 4000, {.forwards=false,.maxSpeed=127,.minSpeed=30});
    // retainerPiston.set_value(false);
    // pros::delay(1000);
    // LeftIntake.move_velocity(-300);
    // RightIntake.move_velocity(-300);
    // pros::delay(250);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // pros::delay(3500);
    // retainerPiston.set_value(true);
    // tonguePiston.set_value(false);

    LeftIntake.move_velocity(0);
    RightIntake.move_velocity(0);
    chassis.moveToPose(-53.047, 77.731, 1.000, 2500, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    chassis.turnToHeading(90, 1000);
    chassis.moveToPose(-45.247, 77.731, 90.000, 1000, {.forwards=true,.maxSpeed=127,.minSpeed=30});

    chassis.moveToPose(-45.247, 55.731, 165.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    chassis.moveToPose(-45.247, 4.731, 169.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});

    chassis.moveToPose(-55.247, -3.731, 163.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    chassis.moveToPose(-55.247, 9.731, 140.000, 4000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=30});
    tonguePiston.set_value(true);
    retainerPiston.set_value(false);
    pros::delay(1000);
    LeftIntake.move_velocity(-300);
    RightIntake.move_velocity(-300);
    pros::delay(250);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    pros::delay(3500);
    retainerPiston.set_value(true);

    chassis.moveToPose(-55.247, -22.731, 163.000, 4000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=30});
    chassis.moveToPose(-55.247, 10.731, 140.000, 4000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=30});
    retainerPiston.set_value(false);
    pros::delay(1000);
    LeftIntake.move_velocity(-300);
    RightIntake.move_velocity(-300);
    pros::delay(250);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    pros::delay(3500);
    retainerPiston.set_value(true);
    tonguePiston.set_value(false);

    // chassis.moveToPose(-30.247, -28.731, 139.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    // chassis.moveToPose(-48.247, -7.731, 140.000, 4000, {.forwards=false,.maxSpeed=127,.minSpeed=30});

    chassis.moveToPose(-20.247, -10.731, 90.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    chassis.moveToPose(-80.247, -10.731, 90.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    
    
}

/**
 * Runs in driver control
 */



int tankState=0;
void opcontrol() {

    chassis.cancelAllMotions();
    chassis.setPose(0,0,0);
    int i=0;
    while (true) {
        // get joystick positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        float j1=0.5*controller.get_analog(ANALOG_RIGHT_X);
		float j3=controller.get_analog(ANALOG_LEFT_Y);

		leftMotors.move(j3+j1);
		rightMotors.move(j3-j1);

        if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_POWER)){
            std::printf("x %f   y %f   t %f\n",chassis.getPose().x,chassis.getPose().y,chassis.getPose().theta);
        }
        // delay to save resources
        
        if(controller.get_digital_new_press(DIGITAL_Y)){wingPiston.set_value(wingState++%2);} // Wing Button
        if(controller.get_digital_new_press(DIGITAL_DOWN)){tankState++;} // Tank Button
		if(controller.get_digital_new_press(DIGITAL_B)){tonguePiston.set_value(tongueState++%2);} // Tongue Button
        if(controller.get_digital_new_press(DIGITAL_L2)){middlePiston.set_value(middleState++%2);} // Middle Scoring Button
        if(controller.get_digital_new_press(DIGITAL_L1)){retainerPiston.set_value(retainerState++%2);} // Retainer Button
			

		//move blocks up
		if(controller.get_digital(DIGITAL_R1)){
			LeftIntake.move(94);
            RightIntake.move(94);
		}

		//move blocks down
		if(controller.get_digital(DIGITAL_R2)){
			LeftIntake.move(-47);
            RightIntake.move(-47);
		}

        //stop intake
		if(controller.get_digital(DIGITAL_RIGHT)){
			LeftIntake.brake();
            RightIntake.brake();
		}

        pros::delay(50);
    }
}
