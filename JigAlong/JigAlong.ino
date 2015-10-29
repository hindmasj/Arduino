// JigAlong - driving wearables
//
// copyright SJ Hindmarch 2015
// Distributed under the GNU General Public License

const int SHFT_CLK=10;
const int LTCH_CLK=9;
const int DATA_IN=8;
const int BITS=8;

byte current_state;

void setup(){
     pinMode(SHFT_CLK,OUTPUT);
     pinMode(LTCH_CLK,OUTPUT);
     pinMode(DATA_IN,OUTPUT);
     current_state=17;
}

void loop(){
     set_leds(current_state);
     delay(200);
     current_state=go_right(current_state);
     //if(current_state==0)current_state=128;
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
