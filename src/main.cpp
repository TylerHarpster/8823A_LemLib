#include "main.h"
#include "fmt/format.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "api.h"
// #include "pros/colors.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/rtos.hpp"
#include "pros/screen.h"
#include "pros/screen.hpp"
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <variant>
// #include "pros/screen.h"
// #include "pros/screen.hpp"
// controller
#include <iostream>












int wingState=0; // variable for wing
int tongueState=0; // variable for tongue
int middleState=0; // variable for middle score
int retainerState=0; // variable for retainer








pros::Controller controller(pros::E_CONTROLLER_MASTER); // defines the controller




pros::MotorGroup leftMotors({1, -6, 4}); // left motors on ports 13, 12, 11
pros::MotorGroup rightMotors({-16, 10, -21}); // right motors on ports 20, 19, 18




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
                                            10 // maximum acceleration (slew)
);




// angular motion controller
lemlib::ControllerSettings angularcontroller(0.95, // proportional gain (kP)
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
            pros::delay(100);
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
    // std::ifstream inFile;
    // inFile.open("auto-runs.txt");
    // int runNumber=inFile.get();
    // printf("run #%i\n",runNumber);
    // inFile.close();
    // pros::delay(1000);
    // std::ofstream outFile2;
    // outFile2.open("auto-runs.txt");
    // outFile2 << runNumber+1;
    // outFile2.close();
    bool isDone=false;
    // pros::Task adsjfdsjf([&runNumber, &isDone](){
    //     std::ofstream outFile;
    //     lemlib::Pose oldPose(0,0,0);
    //     std::string fileName="route"+fmt::to_string(runNumber)+".txt";
    //     std:: cout <<fileName;
    //     outFile.open(fileName);
    //     outFile << std::fixed << std::setprecision(2);
    //     while(!isDone){
    //         outFile << chassis.getPose().x << ", " << chassis.getPose().y << ", " << chassis.getPose().theta << ", " << chassis.getPose().distance(oldPose) << "\n";
           
    //         oldPose=chassis.getPose();
    //         pros::delay(50);
    //     }
    //     outFile.close();
    //     });




    chassis.setPose(0, 0, 0);




    std::printf("gurt\n");
    // pros::Task autoStop([](){
    //     pros::delay(15000);
    //     if(pros::competition::is_competition_switch()){
        //         while(1){
    //             chassis.cancelAllMotions();
    //             pros::delay(100);
    //         }
    //     }
    // });
    // pros::Task autoVibrate([](){
       
    // });
   
    // SIGNATURE    WIN POINT
    if(std::get<touchscreen::button*>(touchscreen::autonScreen->getObjects().at(2))->getState()){
    // if(1){




    //NEW WP




    //1st loader








    //go to loader
    //     float loader_y=31.8;
    //     chassis.setBrakeMode(pros::E_MOTOR_BRAKE_BRAKE);
    // chassis.setPose(0, 0, 0);
    //     retainerPiston.set_value(true);
    //     LeftIntake.move_velocity(500);
    //     RightIntake.move_velocity(500);
    //     chassis.moveToPoint(0.000 , loader_y, 5000,{.minSpeed=60});
    //     chassis.turnToHeading(90, 750);
    //     tonguePiston.set_value(true);
    //     chassis.moveToPoint(17.5 , loader_y, 1300,{.minSpeed=60});
    //     chassis.moveToPoint(18 , loader_y, 1000,{.minSpeed=60});
    //     pros::delay(200);
    //     //long goal
    //     chassis.moveToPoint(-10 , loader_y, 5000,{.forwards=false,.minSpeed=65});
    //     pros::delay(750);
    //     retainerPiston.set_value(false);
    //     LeftIntake.move_velocity(600);
    //     RightIntake.move_velocity(600);




    //     pros::delay(1300);
    //     tonguePiston.set_value(false);
    //     retainerPiston.set_value(true);
    //     chassis.moveToPoint(-2, loader_y, 5000, {.minSpeed=65});
    //     //right cluster
    //     // chassis.turnToHeading(154, 1000);
    //     // chassis.moveToPoint(-2 , 24.31, 5000,{.minSpeed=50});
    //     chassis.turnToHeading(220, 750);
    //     chassis.moveToPoint(-25.2 , 3, 5000,{.minSpeed=65});
    //     //left cluster
    //     chassis.turnToHeading(180, 750);
    //     // chassis.moveToPoint(-24.7 , -17.4, 5000,{.minSpeed=60});
    //     chassis.moveToPoint(-24.6 , -41.4, 5000,{.minSpeed=65});
    //     // chassis.moveToPose(-28.0 ,-33.1, 149.8,5000,{.minSpeed=30});
    //     //high middle goal
    //     pros::delay(1);
    //     chassis.turnToHeading(135, 750);
    //     tonguePiston.set_value(true);
    //     chassis.moveToPoint(-32 , -33, 2000,{.forwards=false,.minSpeed=65});
    //     pros::delay(400);
    //     LeftIntake.move_velocity(-380);
    //     RightIntake.move_velocity(-380);
    //     middlePiston.set_value(true);
    //     pros::delay(420);
    //     LeftIntake.move_velocity(600);
    //     RightIntake.move_velocity(600);
    //     pros::delay(1050);
    //     //2nd loader
    //     middlePiston.set_value(false);
    //     LeftIntake.move_velocity(0);
    //     RightIntake.move_velocity(0);
    //     chassis.moveToPoint(0 , -63.1, 5000,{.minSpeed=65});
    //     chassis.turnToHeading(90, 800,{.minSpeed=50});
    //     // LeftIntake.move_velocity(600);
    //     // RightIntake.move_velocity(600);
    //     // chassis.moveToPoint(17, -63.4, 1500,{.minSpeed=60});
    //     // pros::delay(600);
    //     //2nd long goal
    //     chassis.moveToPoint(-11 , -61, 5000,{.forwards=false,.minSpeed=70});
    //     pros::delay(500);
    //     retainerPiston.set_value(false);
    //     LeftIntake.move_velocity(600);
    //     RightIntake.move_velocity(600);












        // chassis.moveToPose(-24.7 ,-41.2, 171.3,5000,{.minSpeed=30});
        // chassis.moveToPose(-22.1 ,-40.2, 144.6,5000,{.minSpeed=30});
        // chassis.moveToPose(5.365 ,-85.7, 146.5,5000,{.minSpeed=30});
        // chassis.moveToPose(-8.67 ,-75.3, 103.8,5000,{.minSpeed=30});
        // chassis.moveToPose(-17.4 ,-73.1, 104.9,5000,{.minSpeed=30});








    //OLD  SIG WP




        //go to loader
        LeftIntake.move_velocity(600);
        RightIntake.move_velocity(600);
        chassis.moveToPoint(0, 33.5, 2000,{.maxSpeed=127,.minSpeed=52});
        tonguePiston.set_value(true);
        chassis.turnToHeading(-90, 750);
        chassis.moveToPoint(-21.25, 28.5, 4000,{.maxSpeed=127,.minSpeed=52});
        pros::delay(1085);


        //go to long goal
        chassis.moveToPoint(8, 28, 1750,{.forwards=false,.maxSpeed=127,.minSpeed=80});
        // LeftIntake.move_velocity(600);
        // RightIntake.move_velocity(600);
        // pros::delay(1000);
        pros::delay(850);
        LeftIntake.move_velocity(-300);
        RightIntake.move_velocity(-300);
        pros::delay(300);
        retainerPiston.set_value(false);
        LeftIntake.move_velocity(600);
        RightIntake.move_velocity(600);
        pros::delay(1050);


        //collect cluster
        wingPiston.set_value(true);
        tonguePiston.set_value(false);
        retainerPiston.set_value(true);
        chassis.moveToPoint(-2, 29, 1000,{.forwards=true,.maxSpeed=127,.minSpeed=65});
        chassis.turnToPoint(29, -4,1000,{.maxSpeed=127});
        chassis.moveToPoint(29, -4, 2500,{.maxSpeed=127,.minSpeed=53});
        pros::delay(950);
        tonguePiston.set_value(true);
        pros::delay(250);


        //go to middle
        chassis.turnToHeading(-55, 750);
        chassis.moveToPoint(29, -0.9,2000,{.forwards=false,.maxSpeed=127,.minSpeed=53});
        pros::delay(300);
        // tonguePiston.set_value(true);
        LeftIntake.move_velocity(-300);
        RightIntake.move_velocity(-300);
        pros::delay(500);
        middlePiston.set_value(true);
        // retainerPiston.set_value(false);
        LeftIntake.move_velocity(600);
        RightIntake.move_velocity(600);
        pros::delay(1350);
        middlePiston.set_value(false);
        tonguePiston.set_value(false);
        // retainerPiston.set_value(true);
       
        //collect second cluster
        chassis.moveToPoint(24.25, 2,1500,{.maxSpeed=127,.minSpeed=65});
        chassis.turnToHeading(180, 750);
        chassis.moveToPoint(24.75, -52, 2000,{.maxSpeed=127,.minSpeed=65});
        pros::delay(800);
        tonguePiston.set_value(true);


        //low goal
        // pros::delay(250);
        chassis.turnToHeading(45, 850);
        tonguePiston.set_value(false);
        chassis.moveToPoint(45.5, -30, 2000, {.maxSpeed=127,.minSpeed=55});
        pros::delay(500);
        tonguePiston.set_value(true);
        LeftIntake.move_velocity(-55);
        RightIntake.move_velocity(-55);
       
        // chassis.moveToPoint(27, 5,2000,{.maxSpeed=127,.minSpeed=67});
        // chassis.turnToHeading(-180, 1000,{.minSpeed=50});
        // chassis.moveToPoint(25, 5-48,2000,{.maxSpeed=127,.minSpeed=67});
        // //go to long goal
       
        // chassis.turnToHeading(45, 1000,{});
        // chassis.moveToPoint(25+6, 5-48+6,2000,{.maxSpeed=127,.minSpeed=67});
        // while(1){
        //     LeftIntake.move_velocity(-600);
        // RightIntake.move_velocity(-600);
        // pros::delay(400);
        // LeftIntake.move_velocity(600);
        // RightIntake.move_velocity(600);
        // pros::delay(400);
        // }
        // chassis.turnToPoint(25-sqrt(pow(24,2)*2)+4, 5-48-sqrt(pow(24,2)*2)+8+2,1000,{.maxSpeed=127,.minSpeed=80});
        // chassis.moveToPoint(25-sqrt(pow(24,2)*2)+4, 5-48-sqrt(pow(24,2)*2)+8+2,2000,{.maxSpeed=127,.minSpeed=80});
        // chassis.turnToHeading(-90, 1000,{.minSpeed=60});
        // chassis.moveToPoint(25-sqrt(pow(24,2)*2)+4+24, 5-48-sqrt(pow(24,2)*2)+8+2,1000,{.forwards=false,.maxSpeed=127,.minSpeed=67});
        // retainerPiston.set_value(false);
        // chassis.moveToPose(33.092670, 8.0418580, -229.84500,2000,{.maxSpeed=127,.minSpeed=50});
        // chassis.moveToPose(38.939789, 3.7359330, -46.993523,2000,{.maxSpeed=127,.minSpeed=50});
        // chassis.moveToPose(36.759243, -11.66561, -187.67282,2000,{.maxSpeed=127,.minSpeed=50});
        // chassis.moveToPose(44.177277, -51.09176, -188.34074,2000,{.maxSpeed=127,.minSpeed=50});
        // chassis.moveToPose(34.785660, -70.40437, -104.48307,2000,{.maxSpeed=127,.minSpeed=50});
        // chassis.moveToPose(25.481079, -26.63905, -18.461731,2000,{.maxSpeed=127,.minSpeed=50});


    }








    // NEW LEFT SIDE
    else if(std::get<touchscreen::button*>(touchscreen::autonScreen->getObjects().at(0))->getState()){




    //go to loader
        LeftIntake.move_velocity(600);
        RightIntake.move_velocity(600);
        chassis.moveToPoint(0, 33.5, 4000,{.maxSpeed=127,.minSpeed=50});
        tonguePiston.set_value(true);
        chassis.turnToHeading(-90, 1000);
        chassis.moveToPoint(-20.75, 28.5, 4000,{.maxSpeed=127,.minSpeed=50});
        pros::delay(1100);


        //go to long goal
        chassis.moveToPoint(9, 27.67, 1750,{.forwards=false,.maxSpeed=127,.minSpeed=50});
        // LeftIntake.move_velocity(600);
        // RightIntake.move_velocity(600);
        // pros::delay(1000);
        pros::delay(850);
        LeftIntake.move_velocity(-300);
        RightIntake.move_velocity(-300);
        pros::delay(350);
        retainerPiston.set_value(false);
        LeftIntake.move_velocity(600);
        RightIntake.move_velocity(600);
        pros::delay(1350);


        //collect cluster
        wingPiston.set_value(true);
        tonguePiston.set_value(false);
        retainerPiston.set_value(true);
        chassis.moveToPoint(-2, 29, 4000,{.forwards=true,.maxSpeed=127,.minSpeed=50});
        chassis.turnToPoint(29, -1,1000,{.maxSpeed=127});
        chassis.moveToPoint(29, -1,2000,{.maxSpeed=127,.minSpeed=50});
        pros::delay(850);
        tonguePiston.set_value(true);
        pros::delay(350);


        //go to middle
        chassis.turnToHeading(-55, 1000);
        chassis.moveToPoint(29, -0.75,2000,{.forwards=false,.maxSpeed=127,.minSpeed=50});
        pros::delay(300);
        // tonguePiston.set_value(true);
        LeftIntake.move_velocity(-300);
        RightIntake.move_velocity(-300);
        pros::delay(450);
        middlePiston.set_value(true);
        // retainerPiston.set_value(false);
        LeftIntake.move_velocity(600);
        RightIntake.move_velocity(600);
        pros::delay(1350);
        middlePiston.set_value(false);
        tonguePiston.set_value(false);
        // retainerPiston.set_value(true);
   
    }








    // NEW RIGHT SIDE
    else if((std::get<touchscreen::button*>(touchscreen::autonScreen->getObjects().at(1))->getState())){
    // Moves to collect the cluster of three blocks
    wingPiston.set_value(true);
    retainerPiston.set_value(true);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    chassis.moveToPose(22.463, 28.490, 54.850, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=45});
    // tonguePiston.set_value(true);
    pros::delay(750);




    // Move to point in between the loader and long goal, then turn to line up with the loader and extend the tongue
    // tonguePiston.set_value(false);
    chassis.turnToPoint(37.547, -8.75, 1000);
    chassis.moveToPoint(37.547, -8.750, 2000, {.forwards=true,.maxSpeed=127,.minSpeed=45});
    tonguePiston.set_value(true);
    chassis.turnToHeading(180.000, 1500);
    pros::delay(500);




    // Moves into the loader to unload and store the alliance's three colored blocks
    chassis.moveToPoint(35.5, -23.500, 1250, {.forwards=true,.maxSpeed=127,.minSpeed=50});
    // chassis.moveToPose(35.229, -23.000, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // chassis.moveToPose(35.229, -24.000, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    pros::delay(1200);




    // Moves to long goal to score the three blocks from the loader
    // LeftIntake.move_velocity(-400);
    // RightIntake.move_velocity(-400);
    // pros::delay(100);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    chassis.moveToPoint(37, 10.000, 2000, {.forwards=false,.maxSpeed=127,.minSpeed=50});
    // tonguePiston.set_value(false);
    pros::delay(500);
    pros::delay(500);
    retainerPiston.set_value(false);
    pros::delay(1000);
    LeftIntake.move_velocity(-400);
    RightIntake.move_velocity(-400);
    pros::delay(250);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    pros::delay(1500);
    // LeftIntake.move_velocity(-400);
    // RightIntake.move_velocity(-400);
    // pros::delay(250);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // pros::delay(1500);
    // // Lines up the wing and gets control zone in long goal




   
    chassis.moveToPoint(37, -16, 1000,{.minSpeed=50});
    wingPiston.set_value(false);
    chassis.turnToHeading(135, 1000);
    chassis.moveToPoint(34, 0,4000,{.forwards=false,.minSpeed=50});
    chassis.turnToHeading(183, 750);
    chassis.moveToPoint(34.5, 27,4000,{.forwards=false,.minSpeed=50});
    // chassis.moveToPose(20.777630, 13.49215, 177.465729,10000,{.forwards=false,.minSpeed=30});
    // chassis.moveToPose(20.315542, 24.59947, 176.855530,10000,{.forwards=false,.minSpeed=30});
    // chassis.moveToPose(20.183004, 26.82297, 177.199905,10000,{.forwards=false,.minSpeed=30});
   
   
   
    // chassis.moveToPose(35.229, -13.900, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=50});
    // chassis.moveToPose(24.325, 14.377, 168.279, 9000, {.forwards=false,.maxSpeed=127,.minSpeed=20});
    // chassis.moveToPose(22.000, 18.377, 168.279, 9000, {.forwards=false,.maxSpeed=127,.minSpeed=20});
    // wingPiston.set_value(false);
    // chassis.moveToPose(22.325, 23.377, 159.279, 9000, {.forwards=false,.maxSpeed=127,.minSpeed=20});
    }

    //Stupid freaky smeg right side
    else if((std::get<touchscreen::button*>(touchscreen::autonScreen->getObjects().at(5))->getState())){
    wingPiston.set_value(true);
    retainerPiston.set_value(true);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    chassis.moveToPoint(0, 12, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=45});
    // tonguePiston.set_value(true);
    pros::delay(750);




    // Move to point in between the loader and long goal, then turn to line up with the loader and extend the tongue
    // tonguePiston.set_value(false);
    chassis.turnToPoint(37.547, -8.75, 1000);
    chassis.moveToPoint(37.547, -8.750, 2000, {.forwards=true,.maxSpeed=127,.minSpeed=45});
    tonguePiston.set_value(true);
    chassis.turnToHeading(180.000, 1500);
    pros::delay(500);




    // Moves into the loader to unload and store the alliance's three colored blocks
    chassis.moveToPoint(35.5, -23.500, 1250, {.forwards=true,.maxSpeed=127,.minSpeed=50});
    // chassis.moveToPose(35.229, -23.000, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    // chassis.moveToPose(35.229, -24.000, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=45});
    pros::delay(1200);




    // Moves to long goal to score the three blocks from the loader
    // LeftIntake.move_velocity(-400);
    // RightIntake.move_velocity(-400);
    // pros::delay(100);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    chassis.moveToPoint(37, 10.000, 2000, {.forwards=false,.maxSpeed=127,.minSpeed=50});
    // tonguePiston.set_value(false);
    pros::delay(500);
    pros::delay(500);
    retainerPiston.set_value(false);
    pros::delay(1000);
    LeftIntake.move_velocity(-400);
    RightIntake.move_velocity(-400);
    pros::delay(250);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    pros::delay(1500);
    // LeftIntake.move_velocity(-400);
    // RightIntake.move_velocity(-400);
    // pros::delay(250);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // pros::delay(1500);
    // // Lines up the wing and gets control zone in long goal




   
    chassis.moveToPoint(37, -16, 1000,{.minSpeed=50});
    wingPiston.set_value(false);
    chassis.turnToHeading(135, 1000);
    chassis.moveToPoint(34, 0,4000,{.forwards=false,.minSpeed=50});
    chassis.turnToHeading(183, 750);
    chassis.moveToPoint(34.5, 27,4000,{.forwards=false,.minSpeed=50});
    // chassis.moveToPose(20.777630, 13.49215, 177.465729,10000,{.forwards=false,.minSpeed=30});
    // chassis.moveToPose(20.315542, 24.59947, 176.855530,10000,{.forwards=false,.minSpeed=30});
    // chassis.moveToPose(20.183004, 26.82297, 177.199905,10000,{.forwards=false,.minSpeed=30});
   
   
   
    // chassis.moveToPose(35.229, -13.900, 179.504, 1500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=50});
    // chassis.moveToPose(24.325, 14.377, 168.279, 9000, {.forwards=false,.maxSpeed=127,.minSpeed=20});
    // chassis.moveToPose(22.000, 18.377, 168.279, 9000, {.forwards=false,.maxSpeed=127,.minSpeed=20});
    // wingPiston.set_value(false);
    // chassis.moveToPose(22.325, 23.377, 159.279, 9000, {.forwards=false,.maxSpeed=127,.minSpeed=20});
    }









    // Move Backwards
    else if(std::get<touchscreen::button*>(touchscreen::autonScreen->getObjects().at(3))->getState()){
        chassis.moveToPoint(0, -8, 100000, {.forwards=false,.maxSpeed=127,.minSpeed=55});
    }








    // NEW SKILLS
    else if(std::get<touchscreen::button*>(touchscreen::autonScreen->getObjects().at(4))->getState()){

    chassis.setPose(0,1.15,0);


    // chassis.moveToPose(0, 24, 0, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=35});
    // chassis.moveToPoint(0, 24, 9000, {.forwards=true,.maxSpeed=127,.minSpeed=35});
    // chassis.turnToHeading(90, 9000);








    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    retainerPiston.set_value(true);
    chassis.moveToPoint(0, 6, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    // chassis.turnToHeading(90, 1500);
    chassis.moveToPose(35.600, -8.000, 180.000, 5000, {.forwards=true,.maxSpeed=127,.minSpeed=40});
    // chassis.turnToHeading(180, 1000);




    tonguePiston.set_value(true);
    chassis.moveToPose(35.750, -24.000, 180.000, 3500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    // chassis.moveToPose(34.900, -29.000, 180.000, 750, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=40});
    // chassis.moveToPose(34.900, -30.500, 180.000, 1000, {.forwards=true,.lead=0.2,.maxSpeed=127,.minSpeed=40});
    pros::delay(2000);
    LeftIntake.move_velocity(0);
    RightIntake.move_velocity(0);
    chassis.moveToPose(35.300, -10.000, 180.000, 2500, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    tonguePiston.set_value(false);




    chassis.turnToHeading(-90, 1000);
    chassis.moveToPose(24.600, -10.000, -90.000, 1000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(24.000, 30.731, 2.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(24.000, 74.731, 2.000, 3000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=40});
    chassis.turnToHeading(45, 1000);




    chassis.moveToPose(44.000, 90.731, 0.000, 2000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(38.000, 72.000, 0.000, 1500, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
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




    chassis.moveToPose(38.600, 112.150, 2.000, 2000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(38.600, 111.150, 2.000, 2000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(38.600, 112.150, 2.000, 2000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    pros::delay(2000);




    chassis.moveToPose(37.850, 71.800, 0.000, 2500, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    retainerPiston.set_value(false);
    pros::delay(1000);
    LeftIntake.move_velocity(-300);
    RightIntake.move_velocity(-300);
    pros::delay(250);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    pros::delay(2500);
    LeftIntake.move_velocity(-300);
    RightIntake.move_velocity(-300);
    pros::delay(250);
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    pros::delay(2750);
    // retainerPiston.set_value(true);
    tonguePiston.set_value(false);




    wingPiston.set_value(true);
    LeftIntake.move_velocity(0);
    RightIntake.move_velocity(0);
    // chassis.moveToPose(42.047, 77.731, 1.000, 2500, {.forwards=false,.lead=0.2,.maxSpeed=127,.minSpeed=35});
    // chassis.moveToPose(-13.247, 81.731, -90.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    // chassis.moveToPose(-58.247, 82.731, -90.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    // tonguePiston.set_value(true);




    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // chassis.moveToPose(-53.247, 115.731, 0.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=35});
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




    // LeftIntake.move_velocity(0);
    // RightIntake.move_velocity(0);
    // chassis.moveToPose(-53.047, 77.731, 1.000, 2500, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=35});
    // chassis.turnToHeading(90, 1000);
    // chassis.moveToPose(-45.247, 77.731, 90.000, 1000, {.forwards=true,.maxSpeed=127,.minSpeed=30});




    // chassis.moveToPose(-45.247, 55.731, 165.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    // chassis.moveToPose(-45.247, 4.731, 169.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});




    // chassis.moveToPose(-55.247, -3.731, 163.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    // chassis.moveToPose(-55.247, 9.731, 140.000, 4000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=30});
    // tonguePiston.set_value(true);
    // retainerPiston.set_value(false);
    // pros::delay(1000);
    // LeftIntake.move_velocity(-300);
    // RightIntake.move_velocity(-300);
    // pros::delay(250);
    // LeftIntake.move_velocity(600);
    // RightIntake.move_velocity(600);
    // pros::delay(3500);
    // retainerPiston.set_value(true);




    // chassis.moveToPose(-55.247, -22.731, 163.000, 4000, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=30});
    // chassis.moveToPose(-55.247, 10.731, 140.000, 4000, {.forwards=false,.lead=0,.maxSpeed=127,.minSpeed=30});
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




    // chassis.moveToPose(-30.247, -28.731, 139.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=30});
    // chassis.moveToPose(-48.247, -7.731, 140.000, 4000, {.forwards=false,.maxSpeed=127,.minSpeed=30});




    chassis.moveToPose(35.500, 83.500, 3.000, 2500, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(22.500, 74.731, 180.000, 3000, {.forwards=true,.lead=0.5,.maxSpeed=127,.minSpeed=35});
    chassis.moveToPose(22.500, 10.731, 180.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=40});
   
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    chassis.moveToPose(-15.000, 6.000, -90.000, 4000, {.forwards=true,.maxSpeed=127,.minSpeed=50});
    chassis.turnToHeading(180, 1000);
    chassis.moveToPose(-15.000, -200.000, 180.000, 1900, {.forwards=true,.lead=0,.maxSpeed=127,.minSpeed=127});
   
    }




    else{
    LeftIntake.move_velocity(600);
    RightIntake.move_velocity(600);
    pros::delay(1500);
    }
    isDone=true;
}




/**
 * Runs in driver control
 */
int tankState=0;
void opcontrol() {




    chassis.cancelAllMotions();
    // chassis.setPose(0,0,0);
    // chassis.setPose(0,0,0);
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








