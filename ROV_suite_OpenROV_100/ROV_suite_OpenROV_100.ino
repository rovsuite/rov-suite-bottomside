#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Servo.h>


float versionNumber = 1.00;  //keep the version number here
const int numberMotors = 3;
const int numberRelays = 3;
const int numberServos = 2;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
IPAddress broadcast(255,255,255,255);  //broadcast across all IP's

unsigned int localPort = 51000;      // local port to listen on
unsigned int topsidePort = 50000;  //remote port to send packet to

int relayVal[numberRelays];
int motorVal[numberMotors];
int servoVal[numberServos];

int relayPins[numberRelays];
int motorPins[numberMotors];
int servoPins[numberServos];

Servo motors[numberMotors];
Servo servos[numberServos];

char packetBuffer[100];  //100 bytes to hold the incoming packet
char replyBuffer[100];  //100 bytes to hold the packet to send
char tempBuffer[50];

//Create UDP objectmotorVal
EthernetUDP Udp;

void setup()
{
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  
  setupPins();  //call this function to setup the motors, relays and servos
  
}

void loop()
{
 int packetSize = Udp.parsePacket();  //see if there is data available
 if(packetSize)  //if there is a packet
 {
   memset(packetBuffer, 0, sizeof(packetBuffer));  //clear buffer
   memset(replyBuffer, 0, sizeof(packetBuffer));  //clear buffer
   //Read the packet into the buffer
   Udp.read(packetBuffer, packetSize);
   outputCommands();  //output the command values
   readSensors();  //read the sensors and send up a reply packet
 }
}

//Output the command values sent from the topside
void outputCommands()
{
  sscanf(packetBuffer, "%d %d %d %d %d %d %d %d", &motorVal[0], &motorVal[1], &motorVal[2], &relayVal[0], &relayVal[1], &relayVal[2],
  &servoVal[0], &servoVal[1]);
  
  //Write out motor values
  for(int i=0;i<numberMotors;i++)
  {
    motors[i].writeMicroseconds(motorVal[i]);
  }
  
  //Write out the servo values
  for(int i=0;i<numberServos;i++)
  {
    servos[i].write(servoVal[i]);
  }
  
  //Write out relay values
  for(int i=0;i<numberRelays;i++)
  {
    if(relayVal[i] == 1)  //if enabled
    {
      digitalWrite(relayPins[i], HIGH);
    }
    else
    {
      digitalWrite(relayPins[i], LOW);
    }
  }
  
}

//Read the sensors and send the values up to the tospide
void readSensors()
{
  float depth = 1.2;
  float heading = 60.5;
  float voltage = 11.7;
  float sensor0 = 9.0;
  float sensor1 = 6.3;
  
  dtostrf(versionNumber, 2,2, tempBuffer);
  strcat(replyBuffer, tempBuffer);
  strcat(replyBuffer, " ");
  
  dtostrf(depth, 3,2, tempBuffer);
  strcat(replyBuffer, tempBuffer);
  strcat(replyBuffer, " ");
  
  dtostrf(heading, 3,1, tempBuffer);
  strcat(replyBuffer, tempBuffer);
  strcat(replyBuffer, " ");
  
  dtostrf(voltage, 2,1, tempBuffer);
  strcat(replyBuffer, tempBuffer);
  strcat(replyBuffer, " ");
  
  dtostrf(sensor0, 3,3, tempBuffer);
  strcat(replyBuffer, tempBuffer);
  strcat(replyBuffer, " ");
  
  dtostrf(sensor1, 3,3, tempBuffer);
  strcat(replyBuffer, tempBuffer);
  strcat(replyBuffer, " ");
  
  Udp.beginPacket(broadcast, topsidePort);
  Udp.write(replyBuffer);
  Udp.endPacket();
}


//Setup the pins at the start of the program.
void setupPins()
{
  motorPins[0] = 2;
  motorPins[1] = 3;
  motorPins[2] = 5;
  servoPins[0] = 6;
  servoPins[1] = 7;
  relayPins[0] = 8;
  relayPins[1] = 9;
  relayPins[2] = A0;
  
  for(int i=0;i<numberRelays;i++)
  {
    relayVal[i] = 0;
    pinMode(relayPins[i], OUTPUT);  //set as an output
    digitalWrite(relayPins[i], LOW);  //turn off
  }
  for(int i=0;i<numberServos;i++)
  {
    servoVal[i] = 90;  //set a safe value
    servos[i].attach(servoPins[i]);  //finish initializing the servos
  }
  for(int i=0;i<numberMotors;i++)
  {
    motorVal[i] = 1500;  //set the motors neutral
    motors[i].attach(motorPins[i]);  //finish initialzing the motors
  }
  
}
