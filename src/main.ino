#include "SoftwareSerial.h"


#define led_pin 13
#define pin_power 2
#define pin_pump 3

#define pin_valve_hot 4
#define pin_valve_cold 5

#define pin_motor_l 6
#define pin_motor_r 7

#define pin_pulse 8
#define pin_open 9

//#define pin_tone X
#define pin_rx 10
#define pin_tx 11

//turn = 0
//CLOCKWISE;
unsigned long duration;

//Water Level
#define EMPTY     0
#define MININUM   1
#define HALF      2
#define FULL      3

//Machine States
#define PAUSED          0
#define LOADING         1
#define WASHING         2
#define RINSING         3
#define DRAINIG         4


//Machine function
#define WASHED          1
#define RINSED          1
#define DRAINED         1

//Door State
#define OPEN            1
#define CLOSE           0

//machie power
#define ON              1
#define OFF             0

SoftwareSerial sSerial (pin_rx, pin_tx);
uint8_t buf[4];
uint8_t i;
bool clean = 0;


typedef struct {
  uint8_t   water;
  uint8_t   state;
  uint8_t   function;
  uint8_t   door; 
  uint8_t   power;
} machine_t;

volatile machine_t machine;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  sSerial.begin(9600);
  // initialize digital pin 13 as an output.
  pinMode(led_pin, OUTPUT);
  pinMode(pin_power, OUTPUT);
  pinMode(pin_pump, OUTPUT);

  pinMode(pin_valve_hot, OUTPUT);
  pinMode(pin_valve_cold, OUTPUT);

  pinMode(pin_motor_r, OUTPUT);
  pinMode(pin_motor_l, OUTPUT);

  pinMode(pin_pulse, INPUT);

//  tone(pin_tone, 1);
//  delay(1000);
//  noTone(pin_tone);

  digitalWrite(pin_power, HIGH);

  Serial.println(F("[SmartWash]"));

}

void init(){
  machine.state = WASHED;
}

void loaded(){
  water_level = get_water_level();
  if (water_level <= machine.water) {
    digitalWrite(pin_valve_cold, HIGH);
    machine.state = LOADING;
  }
  else {
    digitalWrite(pin_valve_cold, LOW);
    machine.state = WASHING;
  }
}

void washed(){
  do{
      digitalWrite(pin_motor_r, HIGH);
      delay(1000); 
      digitalWrite(pin_motor_r, LOW);
      delay(1000); 
      digitalWrite(pin_motor_l, HIGH);
      delay(1000); 
      digitalWrite(pin_motor_l, LOW);
      delay(1000); 

  } while(time_washed / 4);
}

void soaked(){

}

void rinsed() {

}

void drained(){
  digitalWrite(pin_pump, HIGH);
}

void automata(){
  switch (machine.state) {
      case WASHED:
        if (water_level > 10)
        // do something
        break;
      case SOAKED:
        // do something
        break;
      case RINSED:
        break;
      case DRAINED:
        break;
      default:
         
        // do something
  }

}

uint8_t get_water_level(){
  for(uint8_t i=0; i<5; i++){
    water_level += pulseIn(pin_pulse, RISING);
  }
  return (water_level / 5)
}

// the loop function runs over and over again forever
void loop() {
  if (clean == 0){
    delay(5000);
    while(sSerial.available()) {
      sSerial.read();
    }
    clean = 1;
    Serial.println("buffer flush!");
  }
  if (sSerial.available()){
    Serial.print("\nBuf: ");
    i = 0;
    while(sSerial.available()) {
      //sb = sSerial.read()
      buf[i] = sSerial.read();
      Serial.print(buf[i]);
      i++;
    }
    machine.power_state = buf[0];
    machine.water_level = buf[1];
    machine.function = buf[2];
    machine.door_state = 0;


  }

  get_water_level();



  if (machine.power_state == OFF){
    //clean vars y power off machine
    break;
  }
  if (machine.door_state == OPEN)
    pause();
    break;
  if ()
  


  switch (machine.function) {
      case WASHED:{
        fill_water();
        to_wash(1);
        to_drain();
        break;
      }
      case RINSED:{
        fill_water();
        to_wash(10);
        to_drain();
        break;
      }
      case DRAINED:{

        break;
      }
      default:


  //c & (1 << 0x5) //get bit
  //c | (1 << 0x5) //set bit
  //
  }
}
