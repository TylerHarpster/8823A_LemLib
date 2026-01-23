#include "main.h"
#include <string>
#include <bits/stdc++.h>

namespace touchscreen{

    std::vector<screen*> screenList={
        
                new screen ({
                    new button(
                        10,
                        10,
                        200,
                        50,
                        [](button* self){
                            self->setStrokeWeight(5);
                        },
                        [](button* self){
                            self->setStrokeWeight(0);
                        },
                        {
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
                        },
                        [](button* self){
                            self->setStrokeWeight(0);
                        },
                        {
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
                        },
                        [](button* self){
                            self->setStrokeWeight(0);
                        },
                        {
                            .text="Win point",
                            .fillColor=0x00ddddaa
                            
                        }
        
                    )
                }),
        
                new screen ({
                    new button(
                        10,
                        10,
                        200,
                        50,
                        [](button* self){
                            int state=self->getState();
                            self->setFillColor(0x00FF0F00);
                            if(state%2==1) self->setFillColor(0x00000FFF);
                            self->setState(state+1);
                            printf("ran my ts \n");
                        },
                        [](button* self){
                            
                        },
                        {
                            .text="I'm a toggle",
                            .fillColor=0x00FF0F00
                        }
        
                    ),
                    new button(
                        10,
                        70,
                        200,
                        50,
                        [](button* self){
                            self->setFillColor(0x00F0F0F0);
                            self->setState(self->getState()+1);
                            self->setText(std::string("I'm a counter ")+std::to_string(self->getState()));
                        },
                        [](button* self){
                            
                        },
                        {
                            .text="I'm a counter",
                            .fillColor=0x00F0F0F0,
        
                        }
        
                    ),
                    new button(
                        10,
                        130,
                        200,
                        50,
                        [](button* self){
                            self->setFillColor(0x00FFFF00);
                            self->setState(1);
                        },
                        [](button* self){
                            self->setFillColor(0x00FF0000);
                            self->setState(0);
                        },
                        {
                            .text="I'm deselectable",
                            .fillColor=0x00FF0000
                            
                        }
        
                    )
                })








    };
}





