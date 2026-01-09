#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "api.h"
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include <cstdio>

// controller


int wingState=0;
int tongueState=0;


pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::MotorGroup leftMotors({13, -12, 11}); // left motors on ports 1, 2, 3
pros::MotorGroup rightMotors({-20, 19, -18}); // right motors on ports 4, 5, 6

pros::Motor UpperIntake(10);
pros::Motor MiddleIntake(-1);
pros::Motor LowerIntake(14);
pros::adi::DigitalOut wingPiston('B');
pros::adi::DigitalOut tonguePiston('A');


// create an imu on port 10
pros::Imu imu(15);

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor, port 20, not reversed
pros::Rotation horizontalEnc(9);
// vertical tracking wheel encoder. Rotation sensor, port 11, reversed
pros::Rotation leftEnc(5);
pros::Rotation rightEnc(-4);
// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_2, -4.5);
// vertical tracking wheel. 2.75" diameter, 2.5" offset, left of the robot (negative)
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
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors

    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
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

void autonomous() {
    // chassis.turnToHeading(90, 980990909090);
    pros::Task adsjfdsjf([](){while(1){std::printf("x %.2f   y %.2f   t %.2f\n",chassis.getPose().x,chassis.getPose().y,chassis.getPose().theta); pros::delay(100);}});
    
    /*
x -1.035869   y -30.486217   t 4.014997
x -9.735420   y -34.673637   t 78.631584
x 30.589226   y -29.712183   t 214.523605
x 35.901657   y -22.420971   t 213.004639
x 21.707937   y -45.638882   t 213.005951

    */
    chassis.setPose(0, 0, 0);

    std::printf("gurt\n");

    // RIGHT SIDE
    
    // LowerIntake.move_velocity(600);
    // MiddleIntake.move_velocity(300);
    // UpperIntake.move_velocity(25);
    // chassis.moveToPose(18.463, 25.490, 34.850, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=20});
    // pros::delay(200);
    // LowerIntake.move_velocity(100);
    // MiddleIntake.move_velocity(0);
    // UpperIntake.move_velocity(0);

    // chassis.moveToPose(-6.000, 35.750, -52.507, 2000, {.forwards=true,.maxSpeed=127,.minSpeed=20});
    // LowerIntake.move_velocity(-60);
    // MiddleIntake.move_velocity(-60);
    // UpperIntake.move_velocity(-50);
    // pros::delay(1500);
    // LowerIntake.move_velocity(0);
    // MiddleIntake.move_velocity(0);
    // UpperIntake.move_velocity(0);

    // chassis.moveToPose(32.247, 0.731, -51.862, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    // chassis.turnToHeading(182.000, 1500);
    // tonguePiston.set_value(true);
    // pros::delay(500);
    // LowerIntake.move_velocity(600);
    // MiddleIntake.move_velocity(250);
    // UpperIntake.move_velocity(25);
    // chassis.moveToPose(35.229, -23.255, 183.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // pros::delay(1000);
    // LowerIntake.move_velocity(0);
    // MiddleIntake.move_velocity(0);
    // UpperIntake.move_velocity(0);

    // chassis.moveToPose(35.000, 9.655, 183.504, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=45});
    // tonguePiston.set_value(false);
    // LowerIntake.move_velocity(600);
    // MiddleIntake.move_velocity(600);
    // UpperIntake.move_velocity(600);

    // LEFT SIDE

    LowerIntake.move_velocity(600);
    MiddleIntake.move_velocity(250);
    UpperIntake.move_velocity(25);
    chassis.moveToPose(3.500, 31.500, 5.000, 3000, {.forwards=true,.lead=0.7,.maxSpeed=127,.minSpeed=45});
    pros::delay(200);
    LowerIntake.move_velocity(300);
    MiddleIntake.move_velocity(50);
    UpperIntake.move_velocity(5);
    chassis.moveToPose(-39.500, 11.500, -110.377, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=45});
    LowerIntake.move_velocity(0);
    MiddleIntake.move_velocity(0);
    UpperIntake.move_velocity(0);

    chassis.turnToHeading(-150.000, 2000);
    chassis.moveToPose(-26.009, 25.000, -148.000, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=45});
    pros::delay(500);
    LowerIntake.move_velocity(600);
    MiddleIntake.move_velocity(600);
    UpperIntake.move_velocity(600);
    pros::delay(1250);
    LowerIntake.move_velocity(0);
    MiddleIntake.move_velocity(0);
    UpperIntake.move_velocity(0);

    tonguePiston.set_value(true);
    LowerIntake.move_velocity(400);
    MiddleIntake.move_velocity(150);
    UpperIntake.move_velocity(25);
    chassis.moveToPose(-41.500, -2.200, -150.000, 1850, {.forwards=true,.lead=0,.maxSpeed=40,.minSpeed=10});
    pros::delay(300);
    LowerIntake.move_velocity(0);
    MiddleIntake.move_velocity(0);
    UpperIntake.move_velocity(0);

    chassis.moveToPose(-26.150, 25.000, -148.000, 2000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=45});
    tonguePiston.set_value(false);
    LowerIntake.move_velocity(600);
    MiddleIntake.move_velocity(600);
    UpperIntake.move_velocity(600);
    pros::delay(850);
    MiddleIntake.move_velocity(-600);
    pros::delay(300);
    MiddleIntake.move_velocity(600);

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

}

/**
 * Runs in driver control
 */
void opcontrol() {
    // controller
    // loop to continuously update motors
pros::Task adsjfdsjf([](){while(1){std::printf("%.3f, %.3f, %.3f\n",chassis.getPose().x,chassis.getPose().y,chassis.getPose().theta); pros::delay(100);}});
    
    chassis.cancelAllMotions();
    chassis.setPose(0,0,0);
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
        
        if(controller.get_digital_new_press(DIGITAL_Y)){wingPiston.set_value(wingState++%2);}
		if(controller.get_digital_new_press(DIGITAL_B)){tonguePiston.set_value(tongueState++%2);}
				//storage
		if(controller.get_digital(DIGITAL_R1)){
			LowerIntake.move(94*0.5);
			MiddleIntake.move(63*0.5);
			UpperIntake.move(-63);
		}
	
		if(controller.get_digital_new_release(DIGITAL_R1)){
			MiddleIntake.brake();
			UpperIntake.brake();
		}

		//move blocks up
		if(controller.get_digital(DIGITAL_L1)){
			LowerIntake.move(94*0.5);
			MiddleIntake.move(127*0.5);
			UpperIntake.move(127);
		}

		//move blocks down
		if(controller.get_digital(DIGITAL_R2)){
			LowerIntake.move(-31*0.5);
			MiddleIntake.move(-63*0.5);
			UpperIntake.move(-63);
		}

		//middle goal
		if(controller.get_digital(DIGITAL_L2)){
			LowerIntake.move(63*0.5);
			MiddleIntake.move(94*0.5);
			UpperIntake.move(-106);
		}

		//un-middle goal
		if(controller.get_digital(DIGITAL_X)){
			LowerIntake.move(-63*0.5);
			MiddleIntake.move(-94*0.5);
			UpperIntake.move(106);
		}

		//unstucky
		if(controller.get_digital(DIGITAL_DOWN)){
			LowerIntake.move(-127*0.5);
			MiddleIntake.move(127*0.5);
		}

		if(controller.get_digital(DIGITAL_RIGHT)){
			MiddleIntake.brake();
			LowerIntake.brake();
			UpperIntake.brake();
		}

        pros::delay(50);
    }
}
