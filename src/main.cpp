#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "api.h"
#include "pros/colors.hpp"
#include "pros/llemu.hpp"
#include "pros/motor_group.hpp"
#include "pros/rtos.hpp"
#include "pros/screen.h"
#include "pros/screen.hpp"
#include <thread>
#include <vector>
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
lemlib::ControllerSettings linearcontroller(6, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            7, // derivative gain (kD)
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
                                             1.5, // derivative gain (kD)
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


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize();
    chassis.calibrate(); // calibrate sensors
    // touchscreen::screenListInit();

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
void disabled() {}

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
enum autonRoute{
    skills,
    leftSide,
    rightSide
};
autonRoute selectedAuton=skills;
void autonomous() {
    // chassis.turnToHeading(90, 980990909090);
    pros::Task adsjfdsjf([](){while(1){std::printf("x %.2f   y %.2f   t %.2f\n",chassis.getPose().x,chassis.getPose().y,chassis.getPose().theta); pros::delay(100);}});
    


    chassis.setPose(0, 0, 0);

    std::printf("gurt\n");

    // WIN POINT

    // Moves to collect the cluster of three blocks
    retainerPiston.set_value(true);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    chassis.moveToPose(18.000, 25.490, 42.850, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=40});
    pros::delay(500);

    // Move to align with long goal and score cluster
    chassis.moveToPose(43.500, -2.500, 110.377, 2500, {.forwards=true,.maxSpeed=127,.minSpeed=45});
    chassis.turnToHeading(180.138, 1500);
    chassis.moveToPose(37.500, 18.000, 180.000, 1000, {.forwards=false,.lead=0.2,.maxSpeed=127,.minSpeed=45});
    retainerPiston.set_value(false);
    pros::delay(2500);
    LeftIntake.move_velocity(0);
    RightIntake.move_velocity(0);
    retainerPiston.set_value(true);

    // Move forward and turn to face next block cluster
    chassis.moveToPose(39.500, 2.490, 180.000, 2000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=20});
    chassis.moveToPose(12.463, 20.490, -45.000, 1000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=20});
    LeftIntake.move_velocity(100);
    RightIntake.move_velocity(100);

    // Collect other cluster and score in High Middle Goal
    chassis.moveToPose(-46.878, 23.685, -90.452, 3000, {.forwards=true,.maxSpeed=127,.minSpeed=20});
    chassis.turnToHeading(-131.138, 1500);
    chassis.moveToPose(-26.500, 32.000, -131.138, 1300, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    LeftIntake.move_velocity(-300);
    RightIntake.move_velocity(-300);
    pros::delay(250);
    middlePiston.set_value(true);
    LeftIntake.move_velocity(400);
    RightIntake.move_velocity(400);
    pros::delay(2500);

    // Move towards loader and collect alliance's colored blocks
    LeftIntake.move_velocity(0);
    RightIntake.move_velocity(0);
    middlePiston.set_value(false);
    chassis.moveToPose(-65.285, 1.045, -131.138, 2000, {.forwards=true,.lead=0,.maxSpeed=50,.minSpeed=20});
    chassis.turnToHeading(-172.032, 2000);
    tonguePiston.set_value(true);
    LeftIntake.move_velocity(400);
    RightIntake.move_velocity(400);
    chassis.moveToPose(-64.071, -28.750, -171.281, 2000, {.forwards=true,.maxSpeed=127,.minSpeed=20});

    // Score blocks from loader into long goal
    pros::delay(1500);
    LeftIntake.move_velocity(0);
    RightIntake.move_velocity(0);
    chassis.moveToPose(-60.750, 15.904, -173.217, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=20});
    retainerPiston.set_value(false);
    tonguePiston.set_value(false);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);

    // RIGHT SIDE
    
    // // Moves to collect the cluster of three blocks
    // LowerIntake.move_velocity(600);
    // .move_velocity(300);
    // .move_velocity(25);
    // chassis.moveToPose(18.463, 25.490, 34.850, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=20});
    // pros::delay(500);
    // LowerIntake.move_velocity(175);
    // .move_velocity(25);
    // .move_velocity(0);

    // // Move to score cluster of three and preload in low goal
    // chassis.moveToPose(-5.200, 35.650, -45.000, 2000, {.forwards=true,.maxSpeed=127,.minSpeed=20});
    // LowerIntake.move_velocity(-60);
    // .move_velocity(-60);
    // .move_velocity(-50);
    // pros::delay(2000);
    // LowerIntake.move_velocity(0);
    // .move_velocity(0);
    // .move_velocity(0);

    // // Move to point in between the loader and long goal, then turn to line up with the loader and extend the tongue
    // chassis.moveToPose(32.247, 0.731, -51.862, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    // chassis.turnToHeading(182.000, 1500);
    // tonguePiston.set_value(true);
    // pros::delay(500);

    // // Moves into the loader to unload and store the alliance's three colored blocks
    // LowerIntake.move_velocity(600);
    // .move_velocity(250);
    // .move_velocity(25);
    // chassis.moveToPose(35.229, -22.500, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // pros::delay(500);
    // LowerIntake.move_velocity(0);
    // .move_velocity(0);
    // .move_velocity(0);

    // // Moves to long goal to score the three blocks from the loader
    // chassis.moveToPose(35.500, 9.655, 183.504, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=45});
    // tonguePiston.set_value(false);
    // LowerIntake.move_velocity(600);
    // .move_velocity(600);
    // .move_velocity(600);

    // LEFT SIDE

    // // Moves to collect the cluster of three blocks
    // LowerIntake.move_velocity(600);
    // .move_velocity(250);
    // .move_velocity(25);
    // chassis.moveToPose(3.500, 31.500, 5.000, 3000, {.forwards=true,.lead=0.7,.maxSpeed=127,.minSpeed=45});
    // pros::delay(200);
    // LowerIntake.move_velocity(300);
    // .move_velocity(50);
    // .move_velocity(5);

    // // Moves to a point in between the loader and the long goal, then turns to line up with the loader
    // chassis.moveToPose(-39.500, 11.500, -110.377, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=45});
    // LowerIntake.move_velocity(0);
    // .move_velocity(0);
    // .move_velocity(0);
    // chassis.turnToHeading(-150.000, 2000);

    // // Moves to the long goal and scores the cluster of three
    // chassis.moveToPose(-25.500, 25.000, -148.000, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=45});
    // pros::delay(500);
    // LowerIntake.move_velocity(600);
    // .move_velocity(600);
    // .move_velocity(600);
    // pros::delay(1250);
    // LowerIntake.move_velocity(0);
    // .move_velocity(0);
    // .move_velocity(0);

    // // Extends the tongue and moves into the loader to store the alliance's three colored blocks
    // tonguePiston.set_value(true);
    // LowerIntake.move_velocity(400);
    // .move_velocity(150);
    // .move_velocity(25);
    // chassis.moveToPose(-39.000, -2.750, -150.000, 1850, {.forwards=true,.lead=0,.maxSpeed=40,.minSpeed=10});
    // pros::delay(300);
    // LowerIntake.move_velocity(0);
    // .move_velocity(0);
    // .move_velocity(0);

    // // Moves back to the long goal and scores the three blocks
    // chassis.moveToPose(-25.500, 25.000, -148.000, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=45});
    // tonguePiston.set_value(false);
    // LowerIntake.move_velocity(600);
    // .move_velocity(600);
    // .move_velocity(600);
    // pros::delay(850);
    // .move_velocity(-600);
    // pros::delay(300);
    // .move_velocity(600);

    // SKILLS

    // chassis.moveToPose(0, 5, 0, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(39.048, -2.000, 141.497, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(37.000, -27.016, 180.997, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(37.000, 9.334, 180.896, 5000, {.forwards=false,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(37.000, 0.334, 180.000, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(46.056, 41.714, -2.279, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(41.746, 72.456, -21.396, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(30.423, 102.161, -2.026, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(31.225, 74.883, 0.161, 5000, {.forwards=false,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(-16.950, 85.486, -89.635, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(-54.651, 109.783, 0.598, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(-55.550, 82.431, -0.437, 5000, {.forwards=false,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(-68.747, 74.860, -176.785, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(-69.050, 15.349, -179.905, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(-55.872, -15.930, -178.615, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(-55.471, 11.445, -175.001, 5000, {.forwards=false,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(-22.871, -14.863, -265.214, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    // chassis.moveToPose(20.000, -15.000, -265.000, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=25});
    */
    
}

/**
* Runs in driver control
*/


std::vector<touchscreen::button*> buttons={};


touchscreen::screen* activeScreen;


void opcontrol() {

    activeScreen=touchscreen::screenList.at(0);

    // controller
    // loop to continuously update motors
    // pros::Task adsjfdsjf([](){while(1){std::printf("%.3f, %.3f, %.3f\n",chassis.getPose().x,chassis.getPose().y,chassis.getPose().theta); pros::delay(100);}});

    chassis.cancelAllMotions();
    chassis.setPose(0,0,0);
    int i=0;


    pros::screen::touch_callback([](){
        activeScreen->onPress();
    },pros::last_touch_e_t::E_TOUCH_PRESSED);


    while (true) {

        pros::screen::erase();
        activeScreen->draw();
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
			LeftIntake.move(-94);
            RightIntake.move(-94);
		}

        //stop intake
		if(controller.get_digital(DIGITAL_RIGHT)){
			LeftIntake.brake();
            RightIntake.brake();
		}

        pros::delay(50);
    }
}
