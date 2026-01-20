#include "pros/colors.hpp"
#include "pros/screen.h"
#include "pros/screen.hpp"
#include <bits/stdc++.h>
#include <cstdio>
#include <iterator>
#include <new>
#include <sys/types.h>
#include <variant>
#include <vector>
enum autonRoute{
    skills,
    leftSide,
    rightSide,
    smegSide
    
};
extern autonRoute selectedAuton;

namespace touchscreen{
    enum buttonShapes{ //all shapes a button can be drawn in
        circle,
        rectangle,
        round_rectangle, //warning: looks goofy with no fill
        x                //warning: cannot be filled and must have stroke
    };
    enum drawPositions{
        top_left,
        bottom_left,
        top_right,
        bottom_right,
        middle,
        middle_left,
        middle_right,
        middle_top,
        middle_bottom
    };
    struct buttonParams {
        bool fill=true;                  //fill button? (if fill and stroke are false button will not draw)
        int strokeWeight=1;              //stroke weight
        std::string text="";               //text on button
        buttonShapes shape=rectangle;     //shape to draw button in (see 'enum buttonShapes')
        u_int32_t fillColor=0x00FFFFFF;   //color of fill
        u_int32_t strokeColor=0x00FFFFFF; //color of stroke
        u_int32_t textColor=0x00FFFFFF; //color of stroke
        drawPositions drawPosition=top_left;

    };  
    class button{
        private:
            std::function<void(button*)> pressFunction; //pointer to function to call on press
            int x,y,xscl,yscl;   //size and position
            int radius=0;          //only used on round rectangle or circle
            int state=0;           //state of button (by default, 0 is unpressed and 1 is pressed)
            std::function<void(button*)> onOtherButton;   //runs when another button is pressed, by default just unselects this button
            // std::function<void(button)> onDraw;         //runs when this button is drawn
            buttonParams params; //params (see 'struct buttonParams')
        
        public:
            button();
            button(int x,int y,int xscl,int yscl, std::function<void(button*)> f,std::function<void(button*)> other,buttonParams params){
                this->pressFunction=f;
                this->x=x; this->xscl=xscl;
                this->y=y; this->yscl=yscl;
                this->params=params;
                this->onOtherButton=other;
            }
            button(int x,int y,int radius, std::function<void(button*)> f,std::function<void(button*)> other,buttonParams params){
                this->pressFunction=f;
                this->x=x;
                this->y=y;
                this->radius=radius;
                this->params=params;
                this->onOtherButton=other;
            }
            void draw(){
                // onDraw(this);
                pros::screen::set_pen(params.strokeColor);
                
                switch(params.shape){

                    case touchscreen::buttonShapes::rectangle:
                        pros::screen::fill_rect(x-params.strokeWeight,y-params.strokeWeight,x+xscl+params.strokeWeight,y+yscl+params.strokeWeight);
                        pros::screen::set_pen(params.fillColor);
                        if(params.fill) pros::screen::fill_rect(x, y,x+xscl,y+yscl);
                    break;
                    case touchscreen::buttonShapes::round_rectangle:
                        pros::screen::draw_rect(x,y,x+xscl,y+yscl);
                    break;
                    case touchscreen::buttonShapes::circle:
                        pros::screen::draw_circle(x,y,radius);
                    break;
                    case touchscreen::buttonShapes::x: 
                        pros::screen::draw_rect(x,y,x+xscl,y+yscl);
                    break;

                }
                pros::screen::set_pen(params.textColor);
                pros::screen::print(pros::E_TEXT_SMALL,x+10,y+10,params.text.data());
            }

            void runPress(){
                printf("pressed \n");
                pressFunction(this);
                
            }
            void runOtherPress(){
                onOtherButton(this);
            }
            bool isPressed(){
                switch(params.shape){
                    case circle:
                        if(sqrt(pow((pros::screen::touch_status().x-x),2.)+pow((pros::screen::touch_status().y-y),2.))<=radius){
                            return true;
                        }
                    break;
                    default:
                        if(x<pros::screen::touch_status().x && x+xscl>pros::screen::touch_status().x &&
                        y<pros::screen::touch_status().y && y+yscl>pros::screen::touch_status().y){
                            std::cout << pros::screen::touch_status().x << " ";
                            std::cout << pros::screen::touch_status().y << "\n";
                            return true;
                        }
                    break;
                }
                return false;
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

            void setFill(bool fill){params={fill,params.strokeWeight,params.text,params.shape,params.fillColor,params.strokeColor,params.textColor};};
            void setStrokeWeight(int strokeWeight){params={params.fill,strokeWeight,params.text,params.shape,params.fillColor,params.strokeColor,params.textColor};};
            void setText(std::string text){params={params.fill,params.strokeWeight,text,params.shape,params.fillColor,params.strokeColor,params.textColor};};
            void setShape(buttonShapes shape){params={params.fill,params.strokeWeight,params.text,shape,params.fillColor,params.strokeColor,params.textColor};};
            void setFillColor(u_int32_t fillColor){params={params.fill,params.strokeWeight,params.text,params.shape,fillColor,params.strokeColor,params.textColor};};
            void setStrokeColor(u_int32_t strokeColor){params={params.fill,params.strokeWeight,params.text,params.shape,params.fillColor,strokeColor,params.textColor};};
            void setTextColor(u_int32_t textColor){params={params.fill,params.strokeWeight,params.text,params.shape,params.fillColor,params.strokeColor,textColor};};
            
            // void setShape(buttonParams p, buttonShapes shape){params={params.fill,params.stroke,params.text,params.shape,params.fillColor,params.strokeColor,params.textColor};};
            

            void setOnPress(std::function<void(button*)> f){this->pressFunction=f;};
            // void setOnDraw(std::function<void(button)> f){this->onDraw=f;};
            void setOnOther(std::function<void(button*)> f){this->onOtherButton=f;};
        
    };

    class screen{
        private:
            std::vector<std::variant<button*>> objectList;
        public:
            screen(std::vector<std::variant<button*>> objectList){
                this->objectList=objectList;
            }
            void draw(){
                for(int i=0; i<objectList.size();i++){
                    if(std::holds_alternative<button*>(objectList.at(i))){
                        button* btn=std::get<button*>(objectList.at(i));
                        btn->draw();
                    }
                }
            }
            void onPress(){
                int pressedIndex=-1;
                for(int i=0; i<objectList.size();i++){
                    if(std::holds_alternative<button*>(objectList.at(i))){
                    button* btn=std::get<button*>(objectList.at(i));
                        
                        if(btn -> isPressed()){btn->runPress();
                        std::printf("press at %i\n",i);
                        pressedIndex=i;
                        break;
                        }
                    }
                }
                if(pressedIndex!=-1){
                    for(int i=0; i<objectList.size();i++){
                        if(std::holds_alternative<button*>(objectList.at(i))){
                            button* btn=std::get<button*>(objectList.at(i));
                            if(i!=pressedIndex) btn->runOtherPress();
                        }
                    }
                }
                std::printf("%i\n",pressedIndex);
            }
    };

    extern std::vector<screen*> screenList;
    void screenListInit();
    button lastPressed();
}