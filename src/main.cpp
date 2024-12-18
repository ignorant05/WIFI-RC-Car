#include <Arduino.h> // Remove this if you are using Arduino IDE
#include <WiFi.h> // to set the ESP32 as SoftAP
#include <WebServer.h> // To make the ESP32 hots a webserver to remote control the Car
#include <ESP32Servo.h> // Library to use the servo with ESP32

// Declare the ESP32 softAP Credentials (you can change them as you wish just make sure the pasword is >8)
const String SSID = "WiFi-RC-Car";
const String Password = "Control123";

// Pin definitions
#define SERVO_PIN 13

int enableRightMotor = 23;
int RightMotorPin1 = 18;
int RightMotorPin2 = 19;

// Servo variable
Servo myServo;

// Hosting a webserver on port 80
WebServer server(80);

// Define maw motor speed
#define MAX_SPEED 255

// Declate the PWM frequency, resolution and Channel
const int PWMFreq = 1000;
const int PWMResolution = 8;
const int RightMotorPWMSpeedChannel = 4;

// Function to control motor speed
void Move(int Speed) {
  if (Speed < 0) {
    digitalWrite(RightMotorPin1, LOW);
    digitalWrite(RightMotorPin2, HIGH);
  } else if (Speed > 0) {
    digitalWrite(RightMotorPin1, HIGH);
    digitalWrite(RightMotorPin2, LOW);
  } else {
    digitalWrite(RightMotorPin1, LOW);
    digitalWrite(RightMotorPin2, LOW);
  }

  ledcWrite(RightMotorPWMSpeedChannel, abs(Speed));
}

// Function to handle the control logic from commands
void handleControl() {
  String command = server.arg("cmd");
  int Speed = 0;
  
  if (command == "forward") {
    Speed = MAX_SPEED;
    Serial.println("Moving Forward");
    delay(100); // Small delay
  } else if (command == "backward") {
    Speed = -MAX_SPEED;
    Serial.println("Moving Backward");
    delay(100); // Small delay
  } else if (command == "left") {
    myServo.write(75);
    Serial.println("Turning Left");
    delay(100); // Allow servo to stabilize
  } else if (command == "right") {
    myServo.write(120);
    delay(100); // Allow servo to stabilize
    Serial.println("Turning Right");
  } else if (command == "stop") {
    Speed = 0;
    myServo.write(100);
    delay(100); // Center servo when stopping
    Serial.println("Stopping");
  } else if (command == "Reset") {
    myServo.write(100);
    delay(100); // Allow servo to stabilize
    Serial.println("Resetting Servo to Neutral");
  } else {  // Honestly idk why i put this here even if it's useless but i won't delete it ;)
    Serial.println("Unknown command");
    server.send(400, "text/plain", "Unknown command");
    return;
  }
  
  // send the command to the sever
  server.send(200, "text/plain", "Command received: " + command);
  Move(Speed);
}

// Function to initialize motor and servo pins
void SetupPinModes() {
  pinMode(enableRightMotor, OUTPUT);
  pinMode(RightMotorPin1, OUTPUT);
  pinMode(RightMotorPin2, OUTPUT);

  myServo.attach(SERVO_PIN, 500, 2400);

  ledcSetup(RightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcAttachPin(enableRightMotor, RightMotorPWMSpeedChannel);
}

// HTML Page for web interface
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no, maximum-scale=1">
    <title>RC Car Control</title>
    <style>
        /* Disable scrolling */
        body {
            margin: 0;
            height: 100vh;
            overflow: hidden;
            text-align: center;
            font-family: Arial, sans-serif;
            display: flex;
            flex-direction: column;
            justify-content: center;
            align-items: center;
        }
        /* Button styles */
        button {
            width: 120px;
            height: 60px;
            margin: 10px;
            font-size: 18px;
            border: none;
            border-radius: 5px;
            background-color: #007bff;
            color: white;
            cursor: pointer;
            box-shadow: 0px 4px 6px rgba(0, 0, 0, 0.1);
            transition: background-color 0.2s, transform 0.1s;
        }
        button:active {
            background-color: #0056b3;
            transform: translateY(2px); /* Simulate button press */
        }
        button:disabled {
            background-color: #ccc;
            cursor: not-allowed;
        }
    </style>
</head>
<body>
    <h1>RC Car Control</h1>
    <button 
        onmousedown="startCommand('forward')" 
        onmouseup="stopCommand()" 
        ontouchstart="startCommand('forward')" 
        ontouchend="stopCommand()">Forward</button><br>
    <button onclick="sendCommand('left')">Left</button>
    <button onclick="sendCommand('right')">Right</button><br>
    <button 
        onmousedown="startCommand('backward')" 
        onmouseup="stopCommand()" 
        ontouchstart="startCommand('backward')" 
        ontouchend="stopCommand()">Backward</button>
    <button onclick="sendCommand('Reset')">Reset</button>
</body>

<script>
    // Disable zoom gestures
    document.addEventListener('gesturestart', function (e) {
        e.preventDefault();
    });
    document.addEventListener('gesturechange', function (e) {
        e.preventDefault();
    });
    document.addEventListener('gestureend', function (e) {
        e.preventDefault();
    });

    // Disable double-tap zoom
    let lastTouchEnd = 0;
    document.addEventListener('touchend', function (event) {
        const now = (new Date()).getTime();
        if (now - lastTouchEnd <= 300) {
            event.preventDefault();
        }
        lastTouchEnd = now;
    }, false);

    let commandInterval;

    function startCommand(command) {
        sendCommand(command);
        commandInterval = setInterval(() => sendCommand(command), 100); // Send repeatedly while held
    }

    function stopCommand() {
        clearInterval(commandInterval);
        sendCommand('stop'); // Stop when released
    }

    function sendCommand(command) {
        fetch('/control?cmd=' + command)
            .then(response => response.text())
            .then(data => console.log(`Command sent: ${command}, Server response:`, data))
            .catch(err => console.error('Error:', err));
    }
</script>
</html>
)rawliteral";

// The setup function
void setup() {
  // Calling the setup function for pinModes
  SetupPinModes();

  // Starting the serial monitor for debugging purposes
  Serial.begin(115200);

  // Set the ESP32 as its own Access Point
  if (WiFi.softAP(SSID.c_str(), Password.c_str())) {
    Serial.println("Access Point started successfully!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("Failed to start Access Point");
  }

  // Starting he server 
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  server.on("/control", handleControl);
  server.begin();
  Serial.println("Web server started!");

  // initialize the servo and the motor to be in default condition
  myServo.write(100);
  Move(0);
}

void loop() {
  // Serve the client the html webpage to conrtol the car continiously
  server.handleClient();
}