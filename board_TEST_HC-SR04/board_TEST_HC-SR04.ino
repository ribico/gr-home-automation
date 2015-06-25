int vcc = 49; //attach pin 2 to vcc
int trig = 51; // attach pin 3 to Trig
int echo = 53; //attach pin 4 to Echo
//int gnd = 5; //attach pin 5 to GND

void setup() {

pinMode (vcc,OUTPUT);
//pinMode (gnd,OUTPUT);
// initialize serial communication:
Serial.begin(9600);
Serial.println("START");
}

void loop()
{
digitalWrite(vcc, HIGH);
// establish variables for duration of the ping,
// and the distance result in inches and centimeters:
long duration;
float cm;

// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
pinMode(trig, OUTPUT);
digitalWrite(trig, LOW);
delayMicroseconds(2);
digitalWrite(trig, HIGH);
delayMicroseconds(5);
digitalWrite(trig, LOW);

// The same pin is used to read the signal from the PING))): a HIGH
// pulse whose duration is the time (in microseconds) from the sending
// of the ping to the reception of its echo off of an object.
pinMode(echo,INPUT);
duration = pulseIn(echo, HIGH);

// convert the time into a distance
cm = microsecondsToCentimeters(duration);

Serial.print(cm);
Serial.print("cm");
Serial.println();

delay(10);
}

float microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29.1 / 2;
}