// The timer can be set to three intervals: 5, 10 or 15 minutes.
// The interval is indicated by the LEDs: if the interval is 5 minutes,
// only one LED is turned on. If the interval is 10 minutes, two LEDs
// are turned on.
#define NUM_OF_INTERVALS 3
// Because the button pins are in INPUT_PULLUP mode,
// their default state is HIGH (not pressed). They go to LOW
// if they are pressed.
#define ACTIVATED LOW

const int buttonPin = 6;        // timer setting button; the other end must be connected to ground
const int confirmButtonPin = 5; // confirm button; the other end must connect to ground
const int buzzerPin = 8;
const int ledPins[NUM_OF_INTERVALS] = {10, 11, 12}; // LEDs indicating the number of button presses

int buttonPushCounter = 0;   // counts the number of button presses
int buttonState = HIGH;      // current state of the button
int lastButtonState = HIGH;  // previous state of the button

// resetCounter is called when the user presses the button
// more than NUM_OF_INTERVALS times. In this case, do not turn off
// the first LED, because it will be turned on anyway, and this
// way there is no LED blinking, which may be visible to the user.
void resetCounter()
{
  buttonPushCounter = 0;
  for (int i = 1; i < NUM_OF_INTERVALS; ++i)
    digitalWrite(ledPins[i], LOW);
}

void setup()
{
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(confirmButtonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  // Make sure the LEDs are turned off.
  for (int i = 0; i < NUM_OF_INTERVALS; ++i)
  {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
    
  noTone(buzzerPin);
  
  // initialize serial communication:
  Serial.begin(9600);
  Serial.println("start");
}

void loop()
{
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState)
  {
    if (buttonState == ACTIVATED)
    {
      if (buttonPushCounter == NUM_OF_INTERVALS)
        resetCounter();
  
      digitalWrite(ledPins[buttonPushCounter], HIGH);
      buttonPushCounter++;
    }
  }
  // Delay a little bit to avoid bouncing.
  delay(50);

  lastButtonState = buttonState;

  // Check if the user confirmed his choice.
  if ((digitalRead(confirmButtonPin)) == ACTIVATED)
  {
    Serial.println("Confirm button activated. Number of presses: ");
    Serial.println(buttonPushCounter);
    // Blink the LEDs to acknowledge timer confirmation.
    for (int j = 0; j < 3; ++j)
    {
      for (int i = 0; i < NUM_OF_INTERVALS; ++i)
        digitalWrite(ledPins[i], HIGH);
      delay(300);
      for (int i = 0; i < NUM_OF_INTERVALS; ++i)
        digitalWrite(ledPins[i], LOW);
      delay(300);
    }    
      
    // Reset buttonPushCounter and start the timer.
    int interval = buttonPushCounter * 5000;
    buttonPushCounter = 0;
    delay(interval);

    // If the time elapsed, activate the buzzer.
    tone(buzzerPin, 1319);
    delay(5000);
    noTone(buzzerPin);
  }
}
