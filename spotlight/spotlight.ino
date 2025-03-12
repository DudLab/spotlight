const unsigned int N_LIGHTS = 11;  
const unsigned int LIGHT_IDS[N_LIGHTS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const unsigned int LIGHT_PINS[N_LIGHTS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
const int ID_2_PIN_OFFSET = 1; // PIN = ID + ID_2_PIN_OFFSET
bool L_STATES[N_LIGHTS] = {LOW};  

void setup() {
    Serial.begin(115200);
    for (int i = 0; i < N_LIGHTS; i++) {
        pinMode(LIGHT_PINS[i], OUTPUT);
        digitalWrite(LIGHT_PINS[i], L_STATES[i]);
    }
    Serial.println("Send space-separated light IDS followed by 'T', 'ON', or 'OFF'.");
}


void loop() {
    if (Serial.available()) {

        // read a full command
        String input = Serial.readStringUntil('\n');  // Read full input line
        input.trim();  // Remove whitespace
        processCommand(input);
    }
}


// process the input command
void processCommand(String input) {
    unsigned int pinsToToggle[N_LIGHTS] = {0};  
    int count = 0;
    String command = "";

    char *ptr = strtok((char *)input.c_str(), " ");
    while (ptr != NULL) {
        if (isDigit(ptr[0])) {  
            int pin = atoi(ptr) + ID_2_PIN_OFFSET;
            for (int i = 0; i < N_LIGHTS; i++) {
                if (LIGHT_PINS[i] == pin) {
                    pinsToToggle[count++] = i;
                    break;
                }
            }
        } else {  
            command = String(ptr);
        }
        ptr = strtok(NULL, " ");
    }

    if(command == "ONA")
    {
        for (int i = 0; i < N_LIGHTS; i++) {
            
            L_STATES[i] = HIGH;
            digitalWrite(LIGHT_PINS[i], HIGH);
        }
    }

    if(command == "OFFA")
    {
        for (int i = 0; i < N_LIGHTS; i++) {
            
            L_STATES[i] = LOW;
            digitalWrite(LIGHT_PINS[i], LOW);
        }
    }

    if(command == "TA")
    {
        for (int i = 0; i < N_LIGHTS; i++) {
            
            L_STATES[i] = !L_STATES[i];
            digitalWrite(LIGHT_PINS[i], L_STATES[i]);
        }
    }

    if (command == "T") {  
        for (int i = 0; i < count; i++) {
            int idx = pinsToToggle[i];
            L_STATES[idx] = !L_STATES[idx];
            digitalWrite(LIGHT_PINS[idx], L_STATES[idx]);
        }
    } else if (command == "ON") {
        for (int i = 0; i < count; i++) {
            int idx = pinsToToggle[i];
            L_STATES[idx] = HIGH;
            digitalWrite(LIGHT_PINS[idx], HIGH);
        }
    } else if (command == "OFF") {
        for (int i = 0; i < count; i++) {
            int idx = pinsToToggle[i];
            L_STATES[idx] = LOW;
            digitalWrite(LIGHT_PINS[idx], LOW);
        }
    }

    Serial.print("Command executed: ");
    Serial.println(input);
}
