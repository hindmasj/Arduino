// JigAlong - driving wearables
//
// copyright SJ Hindmarch 2015
// Distributed under the GNU General Public License

// Pins
const int SHFT_CLK=10;
const int LTCH_CLK=9;
const int DATA_IN=8;
const int LEFT_IN=12;
const int RGHT_IN=2;

// Settings
const int BITS=8;
const int STATE_TIME=200;

// Pattern states
const int LEFT_PATN=17;
const int RGHT_PATN=132;
const int NONE=0;
const int LEFT=1;
const int RGHT=2;
const int OUTW=3;
const int INWD=4;

// State
byte current_state;
int pattern;
unsigned long next_time;

void setup(){
  pinMode(SHFT_CLK,OUTPUT);
  pinMode(LTCH_CLK,OUTPUT);
  pinMode(DATA_IN,OUTPUT);
  pinMode(LEFT_IN,INPUT);
  pinMode(RGHT_IN,INPUT);
  current_state=0;
  pattern=NONE;
  next_time=millis();
}

void loop(){
  if(millis()>next_time){
    current_state=get_next_state(pattern,current_state);
    set_leds(current_state);
    next_time=millis()+STATE_TIME;
  }

  if(digitalRead(LEFT_IN) && pattern!=LEFT){
    pattern=LEFT;
    current_state=LEFT_PATN;
  }
  if(digitalRead(RGHT_IN) && pattern!=RGHT){
    pattern=RGHT;
    current_state=RGHT_PATN;
  }
}

byte get_next_state(int direction,byte current_state){
  switch(direction){
    case LEFT: return go_left(current_state);
    case RGHT: return go_right(current_state);
    case NONE: return 0;
    default:return current_state;
  }
}

byte go_left(byte value){
  byte new_value=value >> 1;
  if(bitRead(value,0)==1)
    bitSet(new_value,7);
  return new_value;
}

byte go_right(byte value){
  byte new_value=value << 1;
  if(bitRead(value,7)==1)
    bitSet(new_value,0);
  return new_value;
}

void set_leds(int value){
  byte op=1;
  for(int i=0;i<BITS;i++){
    if((value & op)==op){
	    digitalWrite(DATA_IN,HIGH);
    }else{
	    digitalWrite(DATA_IN,LOW);
    }
	  digitalWrite(SHFT_CLK,LOW);
	  digitalWrite(SHFT_CLK,HIGH);
    op = op << 1;
  }
  digitalWrite(LTCH_CLK,LOW);
  digitalWrite(LTCH_CLK,HIGH);
}
