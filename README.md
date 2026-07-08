# IoT DoS Vulnerability Demonstrator

## Project Title
IoT DoS Vulnerability Demonstrator

## Student Name
Arfa Naaz

## USN
1SP23IC009

## Semester
5th Semester – Mini Project

## Project Description/Objectives
The IoT DoS Vulnerability Demonstrator is an interactive, hardware-based platform built on the ESP32 microcontroller that demonstrates real-world Denial of Service (DoS) vulnerabilities in IoT devices through controlled attack simulations and a real-time web dashboard.

Most cybersecurity learning resources stay theoretical — explaining attacks without ever showing their actual effect on hardware. This project takes a hands-on approach instead: it turns an ESP32 into a safe, self-contained target for common DoS attack vectors, with a live browser dashboard that visualizes exactly what happens to memory, response time, and connections while an attack is running.

**Objectives:**
- Develop an interactive educational platform to execute and observe DoS attack vectors on an ESP32.
- Implement multiple attack mechanisms: Memory Exhaustion, Request Flooding, and Slowloris connection holding.
- Build a real-time web dashboard displaying live system resource consumption and attack impact.
- Demonstrate combined, multi-vector attack scenarios that mimic real-world conditions.
- Provide educational context on vulnerabilities, impacts, and mitigation strategies.

## Technologies/Tools Used

**Hardware**
- ESP32-DevKitV1 (dual-core, 240 MHz, 520 KB SRAM, 4 MB Flash, Wi-Fi 802.11 b/g/n)
- DHT11 Temperature & Humidity Sensor (GPIO4)
- Built-in LED (GPIO2) for visual status indication
- USB 5V/2A power supply

**Software**
- Arduino Core for ESP32
- ESP32 Web Server library (HTTP/1.1, REST API, JSON data exchange)
- HTML5, CSS3, JavaScript (dashboard frontend)

## Steps to Execute the Project

1. **Wire the hardware.**
   Connect the DHT11 sensor to GPIO4 (VCC, GND, DATA). The onboard LED on GPIO2 needs no extra wiring.

2. **Set up the Arduino IDE.**
   - Install the ESP32 board support package (Boards Manager → search "esp32" → install).
   - Install any required libraries (e.g., `DHT sensor library`, `WebServer`, `ArduinoJson`) via Library Manager.

3. **Open the project sketch** (e.g., `IoT_DoS_Demonstrator.ino`) in the Arduino IDE.

4. **Update the Wi-Fi credentials** near the top of the sketch with your own network name and password:

   ```cpp
   // ---- Wi-Fi Configuration ----
   const char* ssid     = "Your_WiFi_Name";     // replace with your Wi-Fi SSID
   const char* password = "Your_WiFi_Password"; // replace with your Wi-Fi password
   ```

   > Example:
   > ```cpp
   > const char* ssid     = "HomeNetwork_5G";
   > const char* password = "MySecurePass123";
   > ```

5. **Select the board and port.**
   Tools → Board → "ESP32 Dev Module", and Tools → Port → select the COM port your ESP32 is connected to.

6. **Upload the sketch** to the ESP32 (Sketch → Upload, or click the Upload button).

7. **Open the Serial Monitor** (115200 baud) to confirm the board connects to Wi-Fi. It will print the assigned IP address, for example:

   ```
   Connecting to WiFi.....
   WiFi connected!
   IP address: 192.168.1.42
   ```

8. **Open the dashboard.**
   In a web browser (on the same Wi-Fi network), navigate to the IP address shown in the Serial Monitor, e.g. `http://192.168.1.42`.

9. **Run the demonstrations.**
   Use the Attack Control Panel to trigger Memory Exhaustion, Request Flood, Slowloris, or Combined attacks, and watch memory usage, response time, and status update live.

10. **Reset between runs.**
    Click **Reset Device & Counters** to restore the system to a clean baseline before the next demonstration.

## Results

- Reproduced heap fragmentation and resource exhaustion typical of constrained IoT devices.
- Showed HTTP server saturation and service-availability loss relevant to DDoS scenarios.
- Demonstrated the stealth/detection challenge posed by Slowloris-style connection holding.
- Combined-attack testing showed realistic, compounding multi-vector impact and longer recovery time.
- Ran stable over 4–6 hour continuous sessions (chip temperature 45–55 °C, no thermal issues).

## Disclaimer
Built strictly for **educational and personal learning purposes** in a controlled environment. Only run these attack simulations against hardware you own or are explicitly authorized to test.# IoT DoS Vulnerability Demonstrator

## Project Title
IoT DoS Vulnerability Demonstrator

## Student Name
Shobha Sharma V

## USN
1SP23IC049

## Semester
5th Semester – Mini Project

## Project Description/Objectives
The IoT DoS Vulnerability Demonstrator is an interactive, hardware-based platform built on the ESP32 microcontroller that demonstrates real-world Denial of Service (DoS) vulnerabilities in IoT devices through controlled attack simulations and a real-time web dashboard.

Most cybersecurity learning resources stay theoretical — explaining attacks without ever showing their actual effect on hardware. This project takes a hands-on approach instead: it turns an ESP32 into a safe, self-contained target for common DoS attack vectors, with a live browser dashboard that visualizes exactly what happens to memory, response time, and connections while an attack is running.

**Objectives:**
- Develop an interactive educational platform to execute and observe DoS attack vectors on an ESP32.
- Implement multiple attack mechanisms: Memory Exhaustion, Request Flooding, and Slowloris connection holding.
- Build a real-time web dashboard displaying live system resource consumption and attack impact.
- Demonstrate combined, multi-vector attack scenarios that mimic real-world conditions.
- Provide educational context on vulnerabilities, impacts, and mitigation strategies.

## Technologies/Tools Used

**Hardware**
- ESP32-DevKitV1 (dual-core, 240 MHz, 520 KB SRAM, 4 MB Flash, Wi-Fi 802.11 b/g/n)
- DHT11 Temperature & Humidity Sensor (GPIO4)
- Built-in LED (GPIO2) for visual status indication
- USB 5V/2A power supply

**Software**
- Arduino Core for ESP32
- ESP32 Web Server library (HTTP/1.1, REST API, JSON data exchange)
- HTML5, CSS3, JavaScript (dashboard frontend)

## Steps to Execute the Project

1. **Wire the hardware.**
   Connect the DHT11 sensor to GPIO4 (VCC, GND, DATA). The onboard LED on GPIO2 needs no extra wiring.

2. **Set up the Arduino IDE.**
   - Install the ESP32 board support package (Boards Manager → search "esp32" → install).
   - Install any required libraries (e.g., `DHT sensor library`, `WebServer`, `ArduinoJson`) via Library Manager.

3. **Open the project sketch** (e.g., `IoT_DoS_Demonstrator.ino`) in the Arduino IDE.

4. **Update the Wi-Fi credentials** near the top of the sketch with your own network name and password:

   ```cpp
   // ---- Wi-Fi Configuration ----
   const char* ssid     = "Your_WiFi_Name";     // replace with your Wi-Fi SSID
   const char* password = "Your_WiFi_Password"; // replace with your Wi-Fi password
   ```

   > Example:
   > ```cpp
   > const char* ssid     = "HomeNetwork_5G";
   > const char* password = "MySecurePass123";
   > ```

5. **Select the board and port.**
   Tools → Board → "ESP32 Dev Module", and Tools → Port → select the COM port your ESP32 is connected to.

6. **Upload the sketch** to the ESP32 (Sketch → Upload, or click the Upload button).

7. **Open the Serial Monitor** (115200 baud) to confirm the board connects to Wi-Fi. It will print the assigned IP address, for example:

   ```
   Connecting to WiFi.....
   WiFi connected!
   IP address: 192.168.1.42
   ```

8. **Open the dashboard.**
   In a web browser (on the same Wi-Fi network), navigate to the IP address shown in the Serial Monitor, e.g. `http://192.168.1.42`.

9. **Run the demonstrations.**
   Use the Attack Control Panel to trigger Memory Exhaustion, Request Flood, Slowloris, or Combined attacks, and watch memory usage, response time, and status update live.

10. **Reset between runs.**
    Click **Reset Device & Counters** to restore the system to a clean baseline before the next demonstration.

## Results

- Reproduced heap fragmentation and resource exhaustion typical of constrained IoT devices.
- Showed HTTP server saturation and service-availability loss relevant to DDoS scenarios.
- Demonstrated the stealth/detection challenge posed by Slowloris-style connection holding.
- Combined-attack testing showed realistic, compounding multi-vector impact and longer recovery time.
- Ran stable over 4–6 hour continuous sessions (chip temperature 45–55 °C, no thermal issues).

## Disclaimer
Built strictly for **educational and personal learning purposes** in a controlled environment. Only run these attack simulations against hardware you own or are explicitly authorized to test.# IoT DoS Vulnerability Demonstrator

## Project Title
IoT DoS Vulnerability Demonstrator

## Student Name
Shobha Sharma V

## USN
1SP23IC049

## Semester
5th Semester – Mini Project

## Project Description/Objectives
The IoT DoS Vulnerability Demonstrator is an interactive, hardware-based platform built on the ESP32 microcontroller that demonstrates real-world Denial of Service (DoS) vulnerabilities in IoT devices through controlled attack simulations and a real-time web dashboard.

Most cybersecurity learning resources stay theoretical — explaining attacks without ever showing their actual effect on hardware. This project takes a hands-on approach instead: it turns an ESP32 into a safe, self-contained target for common DoS attack vectors, with a live browser dashboard that visualizes exactly what happens to memory, response time, and connections while an attack is running.

**Objectives:**
- Develop an interactive educational platform to execute and observe DoS attack vectors on an ESP32.
- Implement multiple attack mechanisms: Memory Exhaustion, Request Flooding, and Slowloris connection holding.
- Build a real-time web dashboard displaying live system resource consumption and attack impact.
- Demonstrate combined, multi-vector attack scenarios that mimic real-world conditions.
- Provide educational context on vulnerabilities, impacts, and mitigation strategies.

## Technologies/Tools Used

**Hardware**
- ESP32-DevKitV1 (dual-core, 240 MHz, 520 KB SRAM, 4 MB Flash, Wi-Fi 802.11 b/g/n)
- DHT11 Temperature & Humidity Sensor (GPIO4)
- Built-in LED (GPIO2) for visual status indication
- USB 5V/2A power supply

**Software**
- Arduino Core for ESP32
- ESP32 Web Server library (HTTP/1.1, REST API, JSON data exchange)
- HTML5, CSS3, JavaScript (dashboard frontend)

## Steps to Execute the Project

1. **Wire the hardware.**
   Connect the DHT11 sensor to GPIO4 (VCC, GND, DATA). The onboard LED on GPIO2 needs no extra wiring.

2. **Set up the Arduino IDE.**
   - Install the ESP32 board support package (Boards Manager → search "esp32" → install).
   - Install any required libraries (e.g., `DHT sensor library`, `WebServer`, `ArduinoJson`) via Library Manager.

3. **Open the project sketch** (e.g., `IoT_DoS_Demonstrator.ino`) in the Arduino IDE.

4. **Update the Wi-Fi credentials** near the top of the sketch with your own network name and password:

   ```cpp
   // ---- Wi-Fi Configuration ----
   const char* ssid     = "Your_WiFi_Name";     // replace with your Wi-Fi SSID
   const char* password = "Your_WiFi_Password"; // replace with your Wi-Fi password
   ```

   > Example:
   > ```cpp
   > const char* ssid     = "HomeNetwork_5G";
   > const char* password = "MySecurePass123";
   > ```

5. **Select the board and port.**
   Tools → Board → "ESP32 Dev Module", and Tools → Port → select the COM port your ESP32 is connected to.

6. **Upload the sketch** to the ESP32 (Sketch → Upload, or click the Upload button).

7. **Open the Serial Monitor** (115200 baud) to confirm the board connects to Wi-Fi. It will print the assigned IP address, for example:

   ```
   Connecting to WiFi.....
   WiFi connected!
   IP address: 192.168.1.42
   ```

8. **Open the dashboard.**
   In a web browser (on the same Wi-Fi network), navigate to the IP address shown in the Serial Monitor, e.g. `http://192.168.1.42`.

9. **Run the demonstrations.**
   Use the Attack Control Panel to trigger Memory Exhaustion, Request Flood, Slowloris, or Combined attacks, and watch memory usage, response time, and status update live.

10. **Reset between runs.**
    Click **Reset Device & Counters** to restore the system to a clean baseline before the next demonstration.

## Results

- Reproduced heap fragmentation and resource exhaustion typical of constrained IoT devices.
- Showed HTTP server saturation and service-availability loss relevant to DDoS scenarios.
- Demonstrated the stealth/detection challenge posed by Slowloris-style connection holding.
- Combined-attack testing showed realistic, compounding multi-vector impact and longer recovery time.
- Ran stable over 4–6 hour continuous sessions (chip temperature 45–55 °C, no thermal issues).

## Disclaimer
Built strictly for **educational and personal learning purposes** in a controlled environment. Only run these attack simulations against hardware you own or are explicitly authorized to test.# IoT DoS Vulnerability Demonstrator

## Project Title
IoT DoS Vulnerability Demonstrator

## Student Name
Shobha Sharma V

## USN
1SP23IC049

## Semester
5th Semester – Mini Project

## Project Description/Objectives
The IoT DoS Vulnerability Demonstrator is an interactive, hardware-based platform built on the ESP32 microcontroller that demonstrates real-world Denial of Service (DoS) vulnerabilities in IoT devices through controlled attack simulations and a real-time web dashboard.

Most cybersecurity learning resources stay theoretical — explaining attacks without ever showing their actual effect on hardware. This project takes a hands-on approach instead: it turns an ESP32 into a safe, self-contained target for common DoS attack vectors, with a live browser dashboard that visualizes exactly what happens to memory, response time, and connections while an attack is running.

**Objectives:**
- Develop an interactive educational platform to execute and observe DoS attack vectors on an ESP32.
- Implement multiple attack mechanisms: Memory Exhaustion, Request Flooding, and Slowloris connection holding.
- Build a real-time web dashboard displaying live system resource consumption and attack impact.
- Demonstrate combined, multi-vector attack scenarios that mimic real-world conditions.
- Provide educational context on vulnerabilities, impacts, and mitigation strategies.

## Technologies/Tools Used

**Hardware**
- ESP32-DevKitV1 (dual-core, 240 MHz, 520 KB SRAM, 4 MB Flash, Wi-Fi 802.11 b/g/n)
- DHT11 Temperature & Humidity Sensor (GPIO4)
- Built-in LED (GPIO2) for visual status indication
- USB 5V/2A power supply

**Software**
- Arduino Core for ESP32
- ESP32 Web Server library (HTTP/1.1, REST API, JSON data exchange)
- HTML5, CSS3, JavaScript (dashboard frontend)

## Steps to Execute the Project

1. **Wire the hardware.**
   Connect the DHT11 sensor to GPIO4 (VCC, GND, DATA). The onboard LED on GPIO2 needs no extra wiring.

2. **Set up the Arduino IDE.**
   - Install the ESP32 board support package (Boards Manager → search "esp32" → install).
   - Install any required libraries (e.g., `DHT sensor library`, `WebServer`, `ArduinoJson`) via Library Manager.

3. **Open the project sketch** (e.g., `IoT_DoS_Demonstrator.ino`) in the Arduino IDE.

4. **Update the Wi-Fi credentials** near the top of the sketch with your own network name and password:

   ```cpp
   // ---- Wi-Fi Configuration ----
   const char* ssid     = "Your_WiFi_Name";     // replace with your Wi-Fi SSID
   const char* password = "Your_WiFi_Password"; // replace with your Wi-Fi password
   ```

   > Example:
   > ```cpp
   > const char* ssid     = "HomeNetwork_5G";
   > const char* password = "MySecurePass123";
   > ```

5. **Select the board and port.**
   Tools → Board → "ESP32 Dev Module", and Tools → Port → select the COM port your ESP32 is connected to.

6. **Upload the sketch** to the ESP32 (Sketch → Upload, or click the Upload button).

7. **Open the Serial Monitor** (115200 baud) to confirm the board connects to Wi-Fi. It will print the assigned IP address, for example:

   ```
   Connecting to WiFi.....
   WiFi connected!
   IP address: 192.168.1.42
   ```

8. **Open the dashboard.**
   In a web browser (on the same Wi-Fi network), navigate to the IP address shown in the Serial Monitor, e.g. `http://192.168.1.42`.

9. **Run the demonstrations.**
   Use the Attack Control Panel to trigger Memory Exhaustion, Request Flood, Slowloris, or Combined attacks, and watch memory usage, response time, and status update live.

10. **Reset between runs.**
    Click **Reset Device & Counters** to restore the system to a clean baseline before the next demonstration.

## Results

- Reproduced heap fragmentation and resource exhaustion typical of constrained IoT devices.
- Showed HTTP server saturation and service-availability loss relevant to DDoS scenarios.
- Demonstrated the stealth/detection challenge posed by Slowloris-style connection holding.
- Combined-attack testing showed realistic, compounding multi-vector impact and longer recovery time.
- Ran stable over 4–6 hour continuous sessions (chip temperature 45–55 °C, no thermal issues).

## Disclaimer
Built strictly for **educational and personal learning purposes** in a controlled environment. Only run these attack simulations against hardware you own or are explicitly authorized to test.
