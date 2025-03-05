#include <SPI.h>
#include <LoRa.h>

// LoRa Module Pins
#define LORA_SS 10
#define LORA_RST 9
#define LORA_DIO0 2

// Motor Driver Pins
#define RIGHT_FORWARD  3  // IN1
#define RIGHT_BACKWARD 6  // IN2
#define LEFT_FORWARD   5  // IN3
#define LEFT_BACKWARD  4  // IN4
#define RIGHT_SPEED    7  // ENA (PWM)
#define LEFT_SPEED     8  // ENB (PWM)

// Variables
unsigned char Received_Command = 0, Speed_index = 0;
unsigned int Run_Stop_Counter = 0;
unsigned char Run_Stop_Mode = 0; // 0 = Stop, 1 = Run

void setup() {
  Serial.begin(9600);
  
  // Initialize Motor Pins
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_SPEED, OUTPUT);
  pinMode(LEFT_SPEED, OUTPUT);

  // LoRa Initialization
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  
  if (!LoRa.begin(433E6)) {  // Set LoRa frequency to 433 MHz
    Serial.println("LoRa initialization failed!");
    while (true);
  }

  Serial.println("LoRa Receiver Started!");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    Serial.println("Packet Received");
    
    Received_Command = LoRa.read();
    Speed_index = LoRa.read();

    Serial.print("Command: ");
    Serial.print(Received_Command);
    Serial.print(" | Speed: ");
    Serial.println(Speed_index);
  }
// Received_Command = LoRa.read();
    //Speed_index = LoRa.read();

  /***************** Speed Control Logic ********************/
  if(Run_Stop_Mode==0)    // Stop
  {
    digitalWrite(RIGHT_FORWARD,LOW);
    digitalWrite(RIGHT_BACKWARD,LOW);
    digitalWrite(LEFT_FORWARD,LOW);
    digitalWrite(LEFT_BACKWARD,LOW);
    Run_Stop_Counter++;
    if(Run_Stop_Counter>=((5-Speed_index)*100))   // Speed_index = 1 -> Min speed,   5 -> Max Speed.
    {
      Run_Stop_Counter = 0;
      Run_Stop_Mode = 1;
    }
  }
  else if(Run_Stop_Mode==1)    // Run
  {
    Run_Stop_Counter++;
    if(Run_Stop_Counter>=((Speed_index)*100))   // Speed_index = 1 -> Min speed,   5 -> Max Speed.
    {
      Run_Stop_Counter = 0;
      Run_Stop_Mode = 0;
    }
  

  if(Received_Command == 1)            //move forward(all motors rotate in forward direction)
    {
      digitalWrite(LEFT_FORWARD,HIGH);
      digitalWrite(RIGHT_FORWARD,HIGH);
      Serial.println(" 11111");
    }
    else if(Received_Command == 2)      //move reverse (all motors rotate in reverse direction)
    {
      digitalWrite(LEFT_BACKWARD,HIGH);
      digitalWrite(RIGHT_BACKWARD,HIGH);
      Serial.println(" 22222");
    }
    else if(Received_Command == 3)      //turn right (left side motors rotate in forward direction, right side motors rotates in backward direction)
    {
      digitalWrite(LEFT_FORWARD,HIGH);
      digitalWrite(RIGHT_BACKWARD,HIGH);
      Serial.println(" 333333");
    }
    else if(Received_Command == 4)      //turn left (right side motors rotate in forward direction, left side motors rotates in backward direction)
    {
      digitalWrite(RIGHT_FORWARD,HIGH);
      digitalWrite(LEFT_BACKWARD,HIGH);
      Serial.println("44444");
    }
    else if(Received_Command == 0)      //STOP (all motors stop)
    {      
      digitalWrite(RIGHT_FORWARD,LOW);
      digitalWrite(RIGHT_BACKWARD,LOW);
      digitalWrite(LEFT_FORWARD,LOW);
      digitalWrite(LEFT_BACKWARD,LOW);
      Serial.println(" NONE 1");
    }
    else                                  //STOP (all motors stop) , If any other command is received.
    {
      digitalWrite(RIGHT_FORWARD,LOW);
      digitalWrite(RIGHT_BACKWARD,LOW);
      digitalWrite(LEFT_FORWARD,LOW);
      digitalWrite(LEFT_BACKWARD,LOW);
      Serial.println("NONE 2");
    }}
    delay(100);
}

void stopMotors() {
  digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(LEFT_BACKWARD, LOW);
}
