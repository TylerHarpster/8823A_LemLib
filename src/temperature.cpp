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
                }),
                                new screen ({
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
                }),

        



    };
}





