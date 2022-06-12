
#include <Mouse.h>
int horzPin = A0;  // Analog output of horizontal joystick pin
int vertPin = A1;  // Analog output of vertical joystick pin
int leftClick = 9;  // select button pin of joystick
int rightClick = 8;
int scrollButton = 7;

int vertZero, horzZero;  // Stores the initial value of each axis, usually around 512
int vertValue, horzValue;  // Stores current analog output of each axis
const int sensitivity = 200;  // Higher sensitivity value = slower mouse, should be <= about 500
int mouseClickFlag = 0;

int invertMouse = -1;         //Noninverted joystick based on orientation
int lastButtonState;
int currentButtonState;

void setup()
{
  Serial.begin(9600);
  pinMode(horzPin, INPUT);  // Set both analog pins as inputs
  pinMode(vertPin, INPUT);
  pinMode(leftClick, INPUT_PULLUP);  // set button select pin as input
  digitalWrite(leftClick, HIGH);  // Pull button select pin high
  pinMode(rightClick, INPUT_PULLUP); 
  digitalWrite(rightClick, HIGH);  // Pull button select pin high
  pinMode(scrollButton, INPUT_PULLUP); 
  
  currentButtonState = digitalRead(scrollButton);

  delay(1000);  // short delay to let outputs settle
  vertZero = analogRead(vertPin);  // get the initial values
  horzZero = analogRead(horzPin);  // Joystick should be in neutral position when reading these

  Mouse.begin(); //Init mouse emulation

}

void loop()
{
  vertValue = analogRead(vertPin) - vertZero;  // read vertical offset
  horzValue = analogRead(horzPin) - horzZero;  // read horizontal offset

  if (vertValue != 0)
    Mouse.move(0, (invertMouse * (vertValue / sensitivity)), 0);
  if (horzValue != 0)
    Mouse.move((invertMouse * (horzValue / sensitivity)), 0, 0);

  if ((digitalRead(leftClick) == 0) && (!mouseClickFlag))
  {
    mouseClickFlag = 1;
    Mouse.press(MOUSE_LEFT);  // click the left button down
  }
  else if ((digitalRead(leftClick)) && (mouseClickFlag))
  {
    mouseClickFlag = 0;
    Mouse.release(MOUSE_LEFT);  // release the left button
  }

  if ((digitalRead(rightClick) == 0) && (!mouseClickFlag))
  {
    mouseClickFlag = 1;
    Mouse.press(MOUSE_RIGHT);
  }
  else if ((digitalRead(rightClick)) && (mouseClickFlag))
  {
    mouseClickFlag = 0;
    Mouse.release(MOUSE_RIGHT);
  }


  
  lastButtonState = currentButtonState;
  currentButtonState = digitalRead(scrollButton);

  if(lastButtonState == HIGH && currentButtonState == LOW) {
    Serial.println("Scroll button pressed");
    Mouse.move(0, 0, (invertMouse * (vertValue / sensitivity)));
  }
}
