#include "pros/colors.hpp"
#include "pros/screen.h"
#include "pros/screen.hpp"
#include <bits/stdc++.h>
#include <cstdio>
#include <new>
#include <sys/types.h>
namespace touchscreen{
    enum buttonShapes{ //all shapes a button can be drawn in
        circle,
        rectangle,
        round_rectangle, //warning: looks goofy with no fill
        x                //warning: cannot be filled and must have stroke
    };
    struct buttonParams {
        mutable bool fill=true;                  //fill button?     < if fill and stroke are false button will not draw
        mutable bool stroke=false;               //outline?         <
        mutable const char * text="";
        mutable buttonShapes shape=rectangle;     //shape to draw button in (see 'enum buttonShapes')
        mutable u_int32_t fillColor=0x00FFFFFF;   //color of fill
        mutable u_int32_t strokeColor=0x00FFFFFF; //color of stroke
        mutable u_int32_t textColor=0x00FFFFFF; //color of stroke
    };  
    class button{
        private:
            std::function<void(button)> pressFunction; //pointer to function to call on press
            int x,y,xscl,yscl;   //size and position
            int state;           //state of button (by default, 0 is unpressed and 1 is pressed) 
            // std::function<void(button)> onOtherButton;   //runs when another button is pressed, by default just unselects this button
            // std::function<void(button)> onDraw;         //runs when this button is drawn
            buttonParams params; //params (see 'struct buttonParams')
        
        public:
            button();
            button(int x,int y,int xscl,int yscl, std::function<void(button)> f,buttonParams params){
                this->pressFunction=f;
                this->x=x; this->xscl=xscl;
                this->y=y; this->yscl=yscl;
                this->params=params;
                // this->onOtherButton=[this](button){this->state=0;};
            }
            void draw(){
                // onDraw(this);
                pros::screen::set_pen(params.fillColor);
                
                switch(params.shape){

                    case touchscreen::buttonShapes::rectangle:
                        if(params.fill) pros::screen::fill_rect(x, y,x+xscl,y+yscl);
                        pros::screen::set_pen(params.strokeColor);
                        if(params.stroke) pros::screen::draw_rect(x,y,x+xscl,y+yscl);
                    break;
                    case touchscreen::buttonShapes::round_rectangle:
                        pros::screen::draw_rect(x,y,x+xscl,y+yscl);
                    break;
                    case touchscreen::buttonShapes::circle:
                        pros::screen::draw_circle(x,y,x+xscl);
                    break;
                    case touchscreen::buttonShapes::x: 
                        pros::screen::draw_rect(x,y,x+xscl,y+yscl);
                    break;

                }
                pros::screen::set_pen(params.textColor);
                pros::screen::print(pros::E_TEXT_MEDIUM_CENTER,x+10,y+10,params.text);
            }

            void runPress(){
                printf("pressed \n");
                pressFunction(*this);
            }
            void runOtherPress(){
                // onOtherButton(*this);
            }




            const int getX(){return x;};
            const int getY(){return y;};
            int getXscl(){return xscl;};
            int getYscl(){return yscl;};
            int getState(){return state;};
            buttonParams getParams(){return params;};
            void setY(int i){y=i;};
            void setX(int i){x=i;};
            void setYscl(int i){yscl=i;};
            void setXscl(int i){xscl=i;};
            void setState(int i){state=i;};
            void setParams(buttonParams p){params=p;};

            void setFill(bool fill){params={fill,params.stroke,params.text,params.shape,params.fillColor,params.strokeColor,params.textColor};};
            void setStroke(bool stroke){params={params.fill,stroke,params.text,params.shape,params.fillColor,params.strokeColor,params.textColor};};
            void setText(const char* text){params={params.fill,params.stroke,text,params.shape,params.fillColor,params.strokeColor,params.textColor};};
            void setShape(buttonShapes shape){params={params.fill,params.stroke,params.text,shape,params.fillColor,params.strokeColor,params.textColor};};
            void setFillColor(u_int32_t fillColor){params={params.fill,params.stroke,params.text,params.shape,fillColor,params.strokeColor,params.textColor};};
            void setStrokeColor(u_int32_t strokeColor){params={params.fill,params.stroke,params.text,params.shape,params.fillColor,strokeColor,params.textColor};};
            void setTextColor(u_int32_t textColor){params={params.fill,params.stroke,params.text,params.shape,params.fillColor,params.strokeColor,textColor};};
            
            // void setShape(buttonParams p, buttonShapes shape){params={params.fill,params.stroke,params.text,params.shape,params.fillColor,params.strokeColor,params.textColor};};
            

            void setOnPress(std::function<void(button)> f){this->pressFunction=f;};
            // void setOnDraw(std::function<void(button)> f){this->onDraw=f;};
            // void setOnOther(std::function<void(button)> f){this->onOtherButton=f;};
    };
    button lastPressed();
}