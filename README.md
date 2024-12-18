# RC Car with ESP32

This project demonstrates how to control an RC car using an ESP32 microcontroller. The ESP32 acts as a Wi-Fi access point and hosts a web server for remote control via a browser-based interface.

## Features

- **SoftAP Mode**: The ESP32 creates its own Wi-Fi network for direct connection.

- **Web Interface**: Users can control the car via an intuitive web-based interface.

- **Motor Control**: Forward, backward, left, and right movements with speed control.

- **Servo Steering**: Servo-controlled steering for turning left or right.

- **Customizable Settings**: Easily modify the Wi-Fi credentials, servo configurations, and motor pins.

## Components Required

- Car Chasis (i used lego with 4 wheels of an old car)

- ESP32 microcontroller (i used ESP32 WROOM 32U with external antenna)

- L298N motor driver module 

- DC motor for movement (i used one but you can add more)

- Servo motor for steering 

- Power source (e.g., battery pack, or 7v battery with connector) 

- Breadboard and jumper wires

## Pin Configuration

| Component          | Pin on ESP32 |
| ------------------ | ------------ |
| Servo Motor        | GPIO 13      |
| Right Motor Enable | GPIO 23      |
| Right Motor Pin 1  | GPIO 18      |
| Right Motor Pin 2  | GPIO 19      |

## Getting Started

### Prerequisites

- Arduino IDE installed.

- ESP32 board support installed in the Arduino IDE.

- Libraries:
  
  - [WiFi.h](https://github.com/espressif/arduino-esp32)
  
  - [WebServer.h](https://github.com/espressif/arduino-esp32)
  
  - [ESP32Servo](https://github.com/jkb-git/ESP32Servo)

### Setup Instructions

1. **Clone the Repository**
   
   ```
   git clone https://github.com/ignorant05/RC-Car-ESP32.git
   ```

2. **Open the Project in Arduino IDE**
   
   - Open `RC_Car_ESP32.ino` in Arduino IDE.

3. **Install Required Libraries**
   
   - Open the Arduino Library Manager (`Sketch` -> `Include Library` -> `Manage Libraries...`).
   
   - Search for and install `ESP32Servo`.

4. **Upload the Code**
   
   - Select the correct ESP32 board and COM (this depends on your os like its /dev/ttyUSB0 in linux) port in the Arduino IDE(or any other IDE).
   
   - Upload the code to the ESP32.

5. **Connect to the Wi-Fi Network**
   
   - On your device, connect to the Wi-Fi network named `WiFi-RC-Car` with the password `Control123`(you can change these btw).

6. **Control the Car**
   
   - Open a browser and navigate to the IP address displayed in the serial monitor.
   
   - Use the web interface to control the car.

## Web Interface

The web interface allows real-time control of the RC car. It includes buttons for forward, backward, left, right, stop, and reset commands. The interface is responsive and works on both desktop and mobile devices.

### Screenshots

## Customization

- **Wi-Fi Credentials**: Change the `SSID` and `Password` constants in the code.

- **Motor Pins**: Modify the `enableRightMotor`, `RightMotorPin1`, and `RightMotorPin2` constants to match your hardware setup.

- **Servo Range**: Adjust the servo angle range in `myServo.attach()`.

## Troubleshooting

- **Access Point Not Starting**:
  
  - Check the serial monitor for error messages.
  
  - Ensure the ESP32 is powered properly.

- **No Response to Commands**:
  
  - Verify the wiring connections.
  
  - Check the browser console for errors.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgments

- [ESP32Servo Library](https://github.com/jkb-git/ESP32Servo)

- [Arduino ESP32 Core](https://github.com/espressif/arduino-esp32)

---

Feel free to contribute to this project by submitting issues or pull requests!
