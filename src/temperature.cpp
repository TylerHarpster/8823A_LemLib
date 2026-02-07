#include "main.h"
#include "pros/screen.h"
#include "pros/screen.hpp"
#include <cstddef>
#include <string>
#include <bits/stdc++.h>

namespace touchscreen{
    screen* testScreen=new screen({
        new button(0,0,480,240,[](button* self){
            
            if(self->getState()==0){
                self->setState(pros::screen::touch_status().x*100000+pros::screen::touch_status().y);
            }
            else{
                int x=self->getState()/100000;
                int y=self->getState()%1000;
                testScreen->addObject(new button(x,y,pros::screen::touch_status().x-x,pros::screen::touch_status().y-y,[](button* self){
                    std::printf("x: %i | y: %i | w: %i | h: %i",self->getX(),self->getY(),self->getXscl(),self->getYscl());
                },{},{}));
            }
        },{},{.fillColor=0x00000000})
    });
    // screen* motorScreen=new screen({
    //     new button()
    // });


    screen* autonScreen=new screen ({
                    new button(
                        10,
                        10,
                        200,
                        50,
                        [](button* self){
                            self->setStrokeWeight(5);
                            self->setState(1);
                        },
                        [](button* self){
                            self->setStrokeWeight(0);
                            self->setState(0);
                        },
                        {
                            .strokeWeight=0,
                            .text="Left Side",
                            .fillColor=0x00001ff0
                        }
        
                    ),
                    new button(
                        10,
                        70,
                        200,
                        50,
                        [](button* self){
                            self->setStrokeWeight(5);
                            self->setState(1);
                        },
                        [](button* self){
                            self->setStrokeWeight(0);
                            self->setState(0);
                        },
                        {
                            .strokeWeight=0,
                            .text="Right Side",
                            .fillColor=0x00A00001,
        
                        }
        
                    ),
                    new button(
                        10,
                        130,
                        200,
                        50,
                        [](button* self){
                            self->setStrokeWeight(5);
                            self->setState(1);
                        },
                        [](button* self){
                            self->setStrokeWeight(0);
                            self->setState(0);
                        },
                        {
                            .strokeWeight=0,
                            .text="Win point",
                            .fillColor=0x00ddddaa
                            
                        }
        
                    ),
                    new button(
                        10,
                        190,
                        200,
                        50,
                        [](button* self){
                            self->setStrokeWeight(5);
                            self->setState(1);
                        },
                        [](button* self){
                            self->setStrokeWeight(0);
                            self->setState(0);
                        },
                        {
                            .strokeWeight=0,
                            .text="Move backwards",
                            .fillColor=0x00001067
                            
                        }
        
                    ),
                });

        




    std::vector<screen*> screenList={
        
                
    };
}





