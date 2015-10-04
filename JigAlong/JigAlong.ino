// JigAlong - driving wearables
//
// copyright SJ Hindmarch 2015
// Distributed under the GNU General Public License

const int LED_PIN[]={3,4,5,6,7,8,9,10};
const int SW_PIN[]={2,12};
const int NUM_LEDS=8;
const int LIGHT_TIME=100;
const int FLASHES=2;
const int LEFT=-1;
const int RIGHT=1;

int all_high[]={HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH,HIGH};
int all_low[]={LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW};
int zero_high[]={HIGH,LOW,LOW,LOW,LOW,LOW,LOW,LOW};
int* current_state;
int current_pin=0;
int direction=0;

void setup() {
  for(int i=0;i<NUM_LEDS;i++){
    pinMode(LED_PIN[i],OUTPUT);  
  }
  pinMode(SW_PIN[0],INPUT);
  pinMode(SW_PIN[1],INPUT);
  for(int i=0;i<FLASHES;i++)
    all_flash();
  current_state=zero_high;
}

void loop() {
  if(digitalRead(SW_PIN[0]))direction=RIGHT;
  if(digitalRead(SW_PIN[1]))direction=LEFT;
  set_leds(current_state,LED_PIN);
  delay(LIGHT_TIME);
  shift_state();
}

void all_flash(){
  set_leds(all_high,LED_PIN);
  delay(250);
  set_leds(all_low,LED_PIN);
  delay(250);    
}

void shift_state(){
  current_state[current_pin]=LOW;
  current_pin+=direction;
  if(current_pin>=NUM_LEDS)current_pin=0;
  if(current_pin<0)current_pin=NUM_LEDS-1;
  current_state[current_pin]=HIGH;
}

void set_leds(int state[],const int pin[]){
  for(int i=0;i<NUM_LEDS;i++){
    digitalWrite(pin[i],state[i]);
  }
}

