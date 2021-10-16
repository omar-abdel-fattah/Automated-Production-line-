//IR_sensor
int irPin =A2 ; // This is our input pin (IR LED at pin
7)
int sensorOut;
//limit switch
int limitswitchfront=3;
int limitswitchback=4;
//DC_motor
#define enA 9
#define in1 7
#define in2 6
//Serial communication TX and RX between 2 arduinos
char stop_key[2]="f";
void setup()
{
//limit switches
pinMode(limitswitchfront, INPUT);
pinMode(limitswitchback, INPUT);
//dcmotor
pinMode(enA, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
digitalWrite(enA,HIGH);
//sensor
pinMode(irPin, INPUT);
homing();
Serial.begin(9600);
}
void push() //feeding mechanism to push
{
//move motor in forward direction
digitalWrite(in1,HIGH);
digitalWrite(in2,LOW);
if(digitalRead(limitswitchfront)==LOW) //if the rack is in the
front position
{
//stop motor infront to check the cap dont flip
digitalWrite(in1, LOW);
digitalWrite(in2, LOW);
delay (1000);

//move motor in backward direction
digitalWrite(in1,LOW);
digitalWrite(in2,HIGH);
if(digitalRead(limitswitchback)==LOW) // if the rack is in the
back position
{
// stop motor
digitalWrite(in1, LOW);
digitalWrite(in2, LOW);
}
}
}
void homing()
{
while (digitalRead(limitswitchback==HIGH))
{
//move motor in backward direction
digitalWrite(in1,LOW);
digitalWrite(in2,HIGH);
}
}
void wait_signal() //wait for a signal from IR
to stop conveyer and push
{
int counter=0; //a counter for sensor
readings to reject errors
int times=0; // a counter for number of
parts reade by sensor
int pushing=0; // a flag to push where 0=
dont push
while(1)
{
stop_key[0]='f'; //flag send by TX to RX of
disassembly station where o=stop conveyer (stepper motor) and f=dont stop
conveyer
sensorOut=analogRead(irPin); //store sensor readings in
SensorOut
delay(100);
if(sensorOut<370) //calibration (if there is
a product)
{
counter++;
if(counter==5) //to be certain it is a
product not error
{
delay(500);
times++;
counter=0;
}
if(times==2) // if both the product and
the cap are read by the IR sensor (2 readings)
{
counter=0;
times=0;
pushing=1; // set flag to push

stop_key[0]='o'; // flag send by TX to RX of
disassembly station where o=stop conveyer (stepper motor)
break; // break out of the while
loop
}
}
}
if(pushing==1) //if it is time to push
{
Serial.write(stop_key,2); // send a serial flag to RX
of the disassembly station by TX of feeding station to stop the conveyer
delay(1000); // wait one second
push(); // push the products to
magazine (by rack and pinion)
pushing=0; // reset
}
}

void loop()
{
wait_signal(); // get readings from
sensors and wait till the conditions occur
}
