#include <Arduino.h>
#include <defines.h>

class Motor{

    private:
    int pin_a;
    int pin_b;
    int pin_pwm;

    public:
    Motor(int a, int b, int pwm){
        pin_a = a;
        pin_b = b;
        pin_pwm = pwm;
    }

    void begin(){
        pinMode(pin_a, OUTPUT);
        pinMode(pin_b, OUTPUT);
        pinMode(pin_pwm, OUTPUT);      
    }

    void setSpeed(int speed){
        if(speed < 0){
            digitalWrite(pin_a, LOW);
            digitalWrite(pin_b, HIGH);
        } else {
            digitalWrite(pin_a, HIGH);
            digitalWrite(pin_b, LOW);
        }
        analogWrite(pin_pwm, constrain(abs(speed),0,255));
    }
};