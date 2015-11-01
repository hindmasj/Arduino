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
const int STATE_TIME=100;
const int BORED_TIME=8000;

// Patterns
const int LEFT_PATN=B10000000;
const int RGHT_PATN=B00000001;
const int OUTW_PATN=B00011000;
const int INWD_PATN=B10000001;
const int NONE=0;
const int LEFT=1;
const int RGHT=2;
const int OUTW=3;
const int INWD=4;

// State
byte current_state;
int pattern;
unsigned long next_state_time;
unsigned long next_bored_time;

void setup(){
  pinMode(SHFT_CLK,OUTPUT);
  pinMode(LTCH_CLK,OUTPUT);
  pinMode(DATA_IN,OUTPUT);
  pinMode(LEFT_IN,INPUT);
  pinMode(RGHT_IN,INPUT);
  current_state=0;
  pattern=NONE;
  next_state_time=millis();
  next_bored_time=millis()+BORED_TIME;
  flash();
  flash();
  vibrate();
  vibrate();
  flash();
  flash();
  flash();
}

void loop(){
  if(millis()>next_state_time){
    current_state=get_next_state(pattern,current_state);
    set_leds(current_state);
    next_state_time=millis()+STATE_TIME;
  }

  if(millis()>next_bored_time){
    flash();
    vibrate();
    vibrate();
    pattern=change_pattern(pattern);
    current_state=bored_state(pattern);
    next_bored_time=millis()+BORED_TIME;
    set_leds(current_state);
  }

  if(digitalRead(LEFT_IN) && pattern!=LEFT){
    next_bored_time=millis()+BORED_TIME;
    pattern=LEFT;
    current_state=LEFT_PATN;
    //pattern=OUTW;
    //current_state=OUTW_PATN;
    set_leds(current_state);
  }
  if(digitalRead(RGHT_IN) && pattern!=RGHT){
    next_bored_time=millis()+BORED_TIME;
    pattern=RGHT;
    current_state=RGHT_PATN;
    //pattern=INWD;
    //current_state=INWD_PATN;
    set_leds(current_state);
  }
}

int change_pattern(int current_pattern){
  switch(current_pattern){
    case LEFT: return OUTW;
    case RGHT: return INWD;
    case OUTW: return RGHT;
    case INWD: return LEFT;
    case NONE:
    default:
      return LEFT;
  }
}

byte bored_state(int current_pattern){
  switch(current_pattern){
    case LEFT: return B10001000;
    case RGHT: return B00010001;
    case OUTW: return B00011000;
    case INWD: return B10000001;
    case NONE:
    default:
      return LEFT_PATN;
  }
}

byte get_next_state(int direction,byte current_state){
  switch(direction){
    case LEFT: return go_left(current_state);
    case RGHT: return go_right(current_state);
    case OUTW: return go_outw(current_state);
    case INWD: return go_inwd(current_state);
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

byte go_outw(byte value){
  byte left=(value & B00001111) >> 1;
  byte rght=(value & B11110000) << 1;
  byte new_value=left+rght;
  if(value & B00000001)
    new_value+=B00001000;
  if(value & B10000000)
    new_value+=B00010000;
  return new_value;
}

byte go_inwd(byte value){
  byte left=(value & B00000111) << 1;
  byte rght=(value & B11100000) >> 1;
  byte new_value=left+rght;
  if(value & B00001000)
    new_value+=B00000001;
  if(value & B00010000)
    new_value+=B10000000;
  return new_value;
}

void flash(){
  set_leds(255);
  delay(100);
  set_leds(0);
  delay(100);
}

void vibrate(){
  set_leds(B10101010);
  delay(100);
  set_leds(B01010101);
  delay(100);
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
