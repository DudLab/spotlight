#include <digitalWriteFast.h>

const uint8_t laserQty = 9;
uint8_t laserPins[laserQty]   = {7,8,9,10,2,6,5,4,3};  // Physical pins to which the lasers are soldered
uint8_t laserOrder[laserQty]  = {2,3,5,6,7,1,4,8,9};  // Ball position ID that each laser points to (1-indexed)
// uint8_t laserOrder[laserQty]  = {1,2,3,4,5,6,7,8,9};  // Laser ID (for troubleshooting only)


// Function to process serial data. Should only expect 2 bytes of data, so throw out everything else.
void serialProcess()
{

  int8_t    selectedBallID;       // ID of the ball that should be lit up (1-indexed)
  int8_t    selectedLaserID;      // ID of the laser associated with the selected ball (1-indexed)
  uint8_t   selectedLaserPin;     // Pin that controls the selected laser
  int8_t    selectedLaserState;   // State that the laser should be set to.
  bool      applyToAllLasers;     // apply 

  // Get the first two characters
  char cmdBuffer[2] = {0};
  if(Serial.readBytesUntil(' ', cmdBuffer, 2)==0)
  {
    Serial.println(F("No serial data found!"));
    return;
  }

  // Throw away the rest
  while (Serial.available() > 0) {
    Serial.read();
  }

  // Convert from ASCII to decimal
  int8_t bufVal0 = cmdBuffer[0]-48;
  int8_t bufVal1 = cmdBuffer[1]-48;

  // Sweep through the available ball IDs. If you've chosen an ID that exists, save that ball ID, save its associated laser ID, and save the pin of that laser
   
  for(uint8_t i=0; i<laserQty; i++)
  {
    if(bufVal0 == laserOrder[i])
    {
      selectedLaserID = i+1;
      selectedBallID = bufVal0;
      selectedLaserPin = laserPins[i];
      break;
    }    
    else if(bufVal0 == 0)
    {
      applyToAllLasers = true;
      selectedLaserID = -1;
      Serial.println(F("Apply to all lasers"));
      break;
    }

    // If the ID you chose does not exist, then set the ID to an error condition to be caught later
    selectedLaserID = -1;
  }

  // Check if the requested laser state is valid,
  // Otherwise, set the state to an error condition to be caught later
  selectedLaserState = -1;
  if((bufVal1==0 || bufVal1==1))
  {
    selectedLaserState = bufVal1;
  }
  
  // If the laser ID was legit
  if(selectedLaserID>0)
  {
    // And the requested state was legit
    if(selectedLaserState>=0)
    {

      // Then sweep through all the lasers, turning off all except the requested laser 
      for(uint8_t i=1; i<=laserQty; i++)
      {

        // Once the loop has come to the selected laser
        if(i == selectedLaserID)
        {
          // Set the laser to the requested state
          digitalWriteFast(selectedLaserPin,selectedLaserState); 
          
          // And print out info about your selections
          char sprintfBuff[38];
          sprintf(sprintfBuff, "Ball: %d | Laser: %d | State: %d",selectedBallID,selectedLaserID,selectedLaserState);
          Serial.println(sprintfBuff);
          break; // Only used for now until other lasers are added

        } else
        {
          // Turn off all other lasers (only allowing 1 laser for now)
          digitalWriteFast(laserPins[i],LOW); 
        }
      }
    } else
    {
      // If an invalid state was chosen, inform the user of such
      Serial.println(F("That was not a correct laser state."));
    }
  } else if(applyToAllLasers)
  {
    Serial.println("Turning off all lasers.");
    // Then sweep through all the lasers, turning off all except the requested laser 
    for(uint8_t i=1; i<=laserQty; i++)
    {

      // Once the loop has come to the selected laser
      
        // Set the laser to the requested state
        digitalWriteFast(i,0); 
        
        // And print out info about your selections
        // char sprintfBuff[38];
        // sprintf(sprintfBuff, "Ball: %d | Laser: %d | State: %d",selectedBallID,selectedLaserID,0);
        // Serial.println(sprintfBuff);
        

       
    }

  } else
  {
    // If an invalid ball ID was chosen, inform the user of such
    Serial.println(F("That was not a correct ball ID."));
  }
}

void setup()
{
  
  // Ensure all lasers are turned off before continuing
  for(uint8_t i=0;i<laserQty;i++)
  {
    pinModeFast(laserPins[i],OUTPUT);
    digitalWriteFast(laserPins[i],LOW);
  }
  
  // Don't forget to set your serial terminal to a baud rate of 115200
  Serial.begin(115200);
  Serial.setTimeout(200); // Speed up the serial response if not enough bytes are provided
  delay(20);

  // Wait until a serial terminal has been opened before continuing
  while(!Serial);
  Serial.println(F("Spotlight--Arduino--Ready!"));
  Serial.println(F("X0 / X1 will turn off / on the laser for light id X"));
  Serial.println(F("00 will turn off all the laser lights"));
}

void loop()
{
  // Check for new serial data
  if(Serial.available())
  {
    serialProcess();
  }

  delay(50);
}
