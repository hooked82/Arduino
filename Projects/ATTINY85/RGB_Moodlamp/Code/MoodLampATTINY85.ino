const int potentiometerPin = 1;
const int buttonPin = 3;

const int RED_PIN = 0;
const int GREEN_PIN = 1;
const int BLUE_PIN = 4;

const int MODE_SPECTRUM = 0;
const int MODE_SOLID = 1;
const int MODE_FLICKER = 2;

int lastButtonState = HIGH;

int mode = 0;

int potentiometerValue;

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  int buttonState;
  potentiometerValue = analogRead(potentiometerPin);
  buttonState = digitalRead(buttonPin);
  
  while (true) {
    if (mode == MODE_SOLID)
    {
      //Single Color Mode
      while (true) {
        potentiometerValue = analogRead(potentiometerPin);
  
        //If mode button is pressed, change mode then reset
        if (checkMode()) {
          changeMode();
          break;
        }
        changeColors(potentiometerValue);
        delay(10);
      }
    }
    else if (mode == MODE_FLICKER)
    {
      //Candle Flicker Mode
      analogWrite(RED_PIN, 255);
      analogWrite(GREEN_PIN, random(15, 30));
      analogWrite(BLUE_PIN, 0);
      
      if (checkMode()) {
        changeMode();
        break;
      }
          
      delay(random(100));
    }
    else
    {
      //Spectrum Colors Mode
      int x;
    
      for (x = 0; x < 768; x++) {
        potentiometerValue = analogRead(potentiometerPin);
        
        //If mode button is pressed, change mode then reset
        if (checkMode()) {
          changeMode();
          break;
        }
          
        showRGB(x);
        int wait = potentiometerValue/10;
        
        if (wait <= 10)
          wait = 10;
        
        delay(wait);
      }
    }
  }
}

boolean checkMode() {
  int state = digitalRead(buttonPin);
  
  if (state == LOW && lastButtonState == HIGH) {
    lastButtonState = state;
  
    return true;
  }
  
  lastButtonState = state;
  return false;
}

void changeColors(int potVal) {
  int redVal = 0;
  int grnVal = 0;
  int bluVal = 0;
  
  if (potVal < 341)  // Lowest third of the potentiometer's range (0-340)
    {                  
      potVal = (potVal * 3) / 4; // Normalize to 0-255
      redVal = 256 - potVal;  // Red from full to off
      grnVal = potVal;        // Green from off to full
      bluVal = 1;             // Blue off
      
      if (potVal == 0) {
        redVal = 255;
      }
    }
    else if (potVal < 682) // Middle third of potentiometer's range (341-681)
    {
      potVal = ( (potVal-341) * 3) / 4; // Normalize to 0-255
  
      if (potVal == 0) {
        potVal = 1;
      }
      redVal = 1;            // Red off
      grnVal = 256 - potVal; // Green from full to off
      bluVal = potVal;       // Blue from off to full
    }
    else  // Upper third of potentiometer"s range (682-1023)
    {
      potVal = ( (potVal-683) * 3) / 4; // Normalize to 0-255
  
      if (potVal == 0) {
        potVal = 1;
      }
      redVal = potVal;       // Red from off to full
      grnVal = 1;            // Green off
      bluVal = 256 - potVal; // Blue from full to off
    }
    analogWrite(RED_PIN, redVal);   // Write values to LED pins
    analogWrite(GREEN_PIN, grnVal); 
    analogWrite(BLUE_PIN, bluVal);  
}

void changeMode() {
  if (mode == MODE_FLICKER) {
    mode = 0;
  } else {
    mode++;
  }
}

void changeLEDS(int level) {
  digitalWrite(RED_PIN, level);
  digitalWrite(GREEN_PIN, level);
  digitalWrite(BLUE_PIN, level);
}

void showRGB(int color) {
  int redIntensity;
  int greenIntensity;
  int blueIntensity;

  if (color <= 255) {
    redIntensity = 255 - color;
    greenIntensity = color;
    blueIntensity = 0;
  } else if (color <= 511) {
    redIntensity = 0;
    greenIntensity = 255 - (color - 256);
    blueIntensity = (color - 256);
  } else {
    redIntensity = (color - 512);
    greenIntensity = 0;
    blueIntensity = 255 - (color - 512);
  }

  analogWrite(RED_PIN, redIntensity);
  analogWrite(BLUE_PIN, blueIntensity);
  analogWrite(GREEN_PIN, greenIntensity);
}
