#define MAX_ANALOGUE            1023
#define MIN_ANALOGUE            0
#define ANALOGUE_TRESHOLD       10
#define UP_TRESHOLD      MAX_ANALOGUE-ANALOGUE_TRESHOLD
#define DOWN_TRESHOLD    MIN_ANALOGUE+ANALOGUE_TRESHOLD
#define LEFT_TRESHOLD    MAX_ANALOGUE-ANALOGUE_TRESHOLD
#define RIGHT_TRESHOLD   MIN_ANALOGUE+ANALOGUE_TRESHOLD

// Pin configuration
#define x_analogue  A0
#define y_analogue  A1
#define ESC_BUTTON  2
#define RETURN_BUTTON 3
#define WAKE_BUTTON 4

#define CURSOR_DELAY 20
#define SCROLL_DELAY 2000
#define DEBOUNCING_DELAY  50
#define BOUNDARY  500

signed char x_status = 0;
signed char y_status = 0;
bool esc_status = HIGH;
bool return_status = HIGH;
bool wake_status = HIGH;

int y_position = 0;
int x_position = 0;

void setup() {
  pinMode(ESC_BUTTON, INPUT);
  pinMode(RETURN_BUTTON, INPUT);
  pinMode(WAKE_BUTTON, INPUT);
 // initialize mouse control:
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  int x_pos = analogRead(x_analogue);
  int y_pos = analogRead(y_analogue);
  bool esc_pos  = digitalRead(ESC_BUTTON);
  bool return_pos = digitalRead(RETURN_BUTTON);
  bool wake_pos = digitalRead(WAKE_BUTTON);

  if (y_pos > UP_TRESHOLD){
    if (y_position > BOUNDARY){
      Mouse.move(0,0,-1);
      delay(SCROLL_DELAY);
    }
    else{
      Mouse.move(0,1,0);
      y_position++;
      delay(CURSOR_DELAY);
    }
  } else if (y_pos < DOWN_TRESHOLD){
    if (y_position < -(BOUNDARY-100)){
      Mouse.move(0,0,1);
      delay(SCROLL_DELAY);
    }
    else{
      Mouse.move(0,-1,0);
      y_position--;
      delay(CURSOR_DELAY);
    }
  }

  Serial.println(y_position);

  if (x_pos < RIGHT_TRESHOLD){
    if (x_position < BOUNDARY*2){
      Mouse.move(1,0,0);
      x_position++;
      delay(CURSOR_DELAY);
    }
  } else if (x_pos > LEFT_TRESHOLD){
    if (x_position > -(BOUNDARY*2)){
      Mouse.move(-1,0,0);
      x_position--;
      delay(CURSOR_DELAY);
    }
  }

  if (esc_status ==  LOW && esc_pos == HIGH){
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_ARROW);
    delay(100); // delay 100 ms
    Keyboard.releaseAll();
    esc_status = HIGH;
  } else if (esc_status == HIGH && esc_pos == LOW){
    delay(DEBOUNCING_DELAY);
    esc_status = LOW; 
  }
  
  if (return_status ==  LOW && return_pos == HIGH){
    Mouse.click();
    delay(DEBOUNCING_DELAY);
    return_status = HIGH;
  } else if (return_status == HIGH && return_pos == LOW){
    delay(DEBOUNCING_DELAY);
    return_status = LOW; 
  }

  if (wake_status ==  LOW && wake_pos == HIGH){
    Keyboard.write(' ');
    delay(1000);  // delay 1 s
    Keyboard.write(' ');

    delay(DEBOUNCING_DELAY);
    wake_status = HIGH;
  } else if (wake_status == HIGH && wake_pos == LOW){
    delay(DEBOUNCING_DELAY);
    wake_status = LOW;     
  }
}
 
