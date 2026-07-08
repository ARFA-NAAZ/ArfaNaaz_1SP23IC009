/*
 * WARNING: This code intentionally contains security vulnerabilities
 * for educational purposes only. DO NOT use in production systems.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// CONFIGURATION 
#define DHT_PIN 4           // DHT11 sensor connected to GPIO 4
#define LED_PIN 2           // Built-in LED on GPIO 2
#define SENSOR_READ_INTERVAL 10000  

// WiFi Credentials 
const char* ssid = " ";
const char* password = " ";

// GLOBAL OBJECTS 
WebServer server(80);
DHT dht(DHT_PIN, DHT11);

// Memory attack tracking - FIXED SIZE TO MATCH
String* memoryBlocks[20] = {nullptr};
int blockCount = 0;

// Global variables for tracking
volatile unsigned long requestCount = 0;
volatile unsigned long freeMemory = 300000;
volatile int slowlorisConnections = 0;
volatile bool isUnderAttack = false;
volatile bool isCritical = false;
volatile bool isWarning = false;
String memoryHog = "";

// LED State Management
unsigned long lastLedChange = 0;
bool ledState = false;
int ledPattern = 0; // 0=ON (Normal), 1=slow blink (Attacks), 2=fast blink (Combined)

// Attack tracking with timers
unsigned long memoryAttackEndTime = 0;
unsigned long floodAttackEndTime = 0;
unsigned long slowlorisAttackEndTime = 0;

// Attack durations (how long LED should blink after attack)
const unsigned long ATTACK_BLINK_DURATION = 10000; // 10 seconds

// LED CONTROL FUNCTIONS 

void updateLED() {
    unsigned long currentMillis = millis();
    
    switch(ledPattern) {
        case 0: // SOLID ON - Normal operation
            digitalWrite(LED_PIN, HIGH);
            break;
            
        case 1: // MEDIUM BLINK - Any single attack (400ms interval - FASTER)
            if (currentMillis - lastLedChange >= 400) {
                ledState = !ledState;
                digitalWrite(LED_PIN, ledState);
                lastLedChange = currentMillis;
            }
            break;
            
        case 2: // FAST BLINK - Combined attack (150ms interval - FASTER)
            if (currentMillis - lastLedChange >= 150) {
                ledState = !ledState;
                digitalWrite(LED_PIN, ledState);
                lastLedChange = currentMillis;
            }
            break;
    }
}

void setLEDPattern(int pattern) {
    ledPattern = pattern;
    lastLedChange = millis();
    ledState = false;
    
    // Immediately set the initial state
    if (pattern == 0) {
        digitalWrite(LED_PIN, HIGH); // Solid ON for normal
    } else {
        digitalWrite(LED_PIN, LOW); // Start with LED off for blinking patterns
    }
}

// ==================== ATTACK TRACKING FUNCTIONS ====================

bool isAnyAttackActive() {
    unsigned long currentTime = millis();
    return (currentTime < memoryAttackEndTime) || 
           (currentTime < floodAttackEndTime) || 
           (currentTime < slowlorisAttackEndTime);
}

bool isCombinedAttackActive() {
    unsigned long currentTime = millis();
    int activeAttacks = 0;
    if (currentTime < memoryAttackEndTime) activeAttacks++;
    if (currentTime < floodAttackEndTime) activeAttacks++;
    if (currentTime < slowlorisAttackEndTime) activeAttacks++;
    return activeAttacks >= 2;
}

void startMemoryAttackBlink() {
    memoryAttackEndTime = millis() + ATTACK_BLINK_DURATION;
    Serial.println("💡 Memory attack ");
}

void startFloodAttackBlink() {
    floodAttackEndTime = millis() + ATTACK_BLINK_DURATION;
    Serial.println("💡 Flood attack ");
}

void startSlowlorisAttackBlink() {
    slowlorisAttackEndTime = millis() + ATTACK_BLINK_DURATION;
    Serial.println("💡 Slowloris attack ");
}

// ==================== STATUS LOGIC FIX ====================

String calculateStatus() {
    freeMemory = ESP.getFreeHeap(); // Update memory reading
    
    // Reset attack flags
    isUnderAttack = false;
    isCritical = false;
    isWarning = false;
    
    // Check memory conditions FIRST
    if (freeMemory < 120000) {
        isCritical = true;
    } else if (freeMemory < 180000) {
        isWarning = true;
    }
    
    // Check if under active attack
    bool anyAttackActive = isAnyAttackActive();
    if (slowlorisConnections >= 2 || requestCount > 50 || anyAttackActive) {
        isUnderAttack = true;
    }
    
    if (isCombinedAttackActive()) {
        // Combined attack - Fast blink
        setLEDPattern(2);
        return "COMBINED ATTACK";
    } else if (anyAttackActive) {
        // Single attack - Medium blink
        setLEDPattern(1);
        return "UNDER ATTACK";
    } else if (isCritical) {
        // Critical memory but no active attacks - Medium blink
        setLEDPattern(1);
        return "CRITICAL";
    } else if (isWarning) {
        // Warning memory but no active attacks - Medium blink
        setLEDPattern(1);
        return "WARNING";
    } else {
        // Normal - Solid ON
        setLEDPattern(0);
        return "NORMAL";
    }
}

// ==================== WEB SERVER HANDLERS ====================

void handleRoot() {
    requestCount++;
    
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>IoT DoS Vulnerability Demonstrator</title>
    <style>
        :root {
            --dark: #0a0e17;
            --darker: #05080f;
            --primary: #00d4ff;
            --secondary: #7b61ff;
            --accent: #00ff88;
            --warning: #ffb800;
            --critical: #ff4757;
            --text: #e2e8f0;
        }
        
        body {
            font-family: 'Courier New', monospace;
            background: linear-gradient(135deg, var(--darker) 0%, var(--dark) 100%);
            color: var(--text);
            margin: 0;
            padding: 20px;
            min-height: 100vh;
        }
        
        .container {
            max-width: 1200px;
            margin: 0 auto;
        }
        
        .header {
            text-align: center;
            margin-bottom: 30px;
            padding: 20px;
            background: rgba(0, 212, 255, 0.1);
            border-radius: 15px;
            border: 1px solid var(--primary);
        }
        
        .header h1 {
            color: var(--primary);
            margin: 0;
            font-size: 2.5em;
            text-shadow: 0 0 10px rgba(0, 212, 255, 0.5);
        }
        
        .header .subtitle {
            color: var(--secondary);
            font-size: 1.2em;
        }
        
        .dashboard-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 20px;
            margin-bottom: 30px;
        }
        
        .card {
            background: rgba(123, 97, 255, 0.1);
            border: 1px solid var(--secondary);
            border-radius: 12px;
            padding: 25px;
            position: relative;
            overflow: hidden;
        }
        
        .card::before {
            content: '';
            position: absolute;
            top: 0;
            left: 0;
            right: 0;
            height: 3px;
            background: linear-gradient(90deg, var(--primary), var(--accent));
        }
        
        .card h3 {
            color: var(--primary);
            margin-top: 0;
            border-bottom: 1px solid rgba(0, 212, 255, 0.3);
            padding-bottom: 10px;
        }
        
        .sensor-value {
            font-size: 2.8em;
            font-weight: bold;
            text-align: center;
            margin: 15px 0;
            text-shadow: 0 0 15px currentColor;
        }
        
        .temp { color: var(--critical); }
        .humidity { color: var(--primary); }
        .memory { color: var(--accent); }
        
        .status-panel {
            background: rgba(0, 212, 255, 0.05);
            border: 1px solid var(--primary);
            border-radius: 12px;
            padding: 20px;
            margin: 20px 0;
        }
        
        .status-indicator {
            display: inline-block;
            padding: 8px 16px;
            border-radius: 20px;
            font-weight: bold;
            font-size: 0.9em;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        
        .status-normal {
            background: var(--accent);
            color: var(--dark);
            box-shadow: 0 0 15px rgba(0, 255, 136, 0.5);
        }
        
        .status-warning {
            background: var(--warning);
            color: var(--dark);
            box-shadow: 0 0 15px rgba(255, 184, 0, 0.5);
            animation: warningPulse 2s infinite;
        }
        
        .status-critical {
            background: var(--critical);
            color: white;
            box-shadow: 0 0 15px rgba(255, 71, 87, 0.5);
            animation: criticalPulse 1s infinite;
        }
        
        .status-under-attack {
            background: #e83e8c;
            color: white;
            box-shadow: 0 0 15px rgba(232, 62, 140, 0.5);
            animation: attackPulse 0.5s infinite;
        }
        
        .status-combined-attack {
            background: linear-gradient(45deg, #ff0000, #800000);
            color: white;
            box-shadow: 0 0 20px rgba(255, 0, 0, 0.7);
            animation: combinedPulse 0.3s infinite;
        }
        
        .metric {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 10px 0;
            border-bottom: 1px solid rgba(123, 97, 255, 0.2);
        }
        
        .metric:last-child {
            border-bottom: none;
        }
        
        .metric-label {
            color: var(--text);
            opacity: 0.8;
        }
        
        .metric-value {
            color: var(--primary);
            font-weight: bold;
        }
        
        .attack-panel {
            background: rgba(255, 71, 87, 0.05);
            border: 1px solid var(--critical);
            border-radius: 12px;
            padding: 25px;
            margin: 20px 0;
        }
        
        .attack-panel h2 {
            color: var(--critical);
            margin-top: 0;
        }
        
        button {
            background: linear-gradient(135deg, var(--secondary), var(--primary));
            color: var(--dark);
            border: none;
            padding: 15px 25px;
            margin: 8px 0;
            border-radius: 8px;
            cursor: pointer;
            font-family: 'Courier New', monospace;
            font-weight: bold;
            font-size: 16px;
            width: 100%;
            transition: all 0.3s ease;
            text-transform: uppercase;
            letter-spacing: 1px;
        }
        
        button:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 20px rgba(123, 97, 255, 0.4);
        }
        
        button.attack {
            background: linear-gradient(135deg, var(--critical), #ff6b81);
            color: white;
        }
        
        button.reset {
            background: linear-gradient(135deg, var(--accent), #00cc78);
            color: var(--dark);
        }
        
        .info-display {
            background: rgba(0, 0, 0, 0.3);
            padding: 10px;
            border-radius: 6px;
            font-family: monospace;
            font-size: 0.8em;
            word-break: break-all;
            margin: 10px 0;
            border: 1px solid rgba(123, 97, 255, 0.3);
        }
        
        .pulse {
            animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
            0% { opacity: 1; }
            50% { opacity: 0.7; }
            100% { opacity: 1; }
        }
        
        @keyframes criticalPulse {
            0% { transform: scale(1); }
            50% { transform: scale(1.05); }
            100% { transform: scale(1); }
        }
        
        @keyframes attackPulse {
            0% { opacity: 1; }
            50% { opacity: 0.6; }
            100% { opacity: 1; }
        }
        
        @keyframes warningPulse {
            0% { opacity: 1; }
            50% { opacity: 0.8; }
            100% { opacity: 1; }
        }
        
        @keyframes combinedPulse {
            0% { transform: scale(1); opacity: 1; }
            50% { transform: scale(1.1); opacity: 0.7; }
            100% { transform: scale(1); opacity: 1; }
        }
        
        .glow {
            box-shadow: 0 0 20px currentColor;
        }
        
        .log-panel {
            background: rgba(0, 0, 0, 0.5);
            border: 1px solid var(--secondary);
            border-radius: 8px;
            padding: 15px;
            margin: 15px 0;
            height: 150px;
            overflow-y: auto;
            font-family: monospace;
            font-size: 0.9em;
        }
        
        .log-entry {
            margin: 5px 0;
            padding: 5px;
            border-left: 3px solid var(--primary);
        }
        
        .log-warning {
            border-left-color: var(--warning);
            color: var(--warning);
        }
        
        .log-critical {
            border-left-color: var(--critical);
            color: var(--critical);
        }
    </style>
</head>
<body>
    <div class="container">
        <!-- Header -->
        <div class="header">
            <h1>🔐 IoT DoS Vulnerability Demonstrator</h1>
            <div class="subtitle">Real-Time ESP32 Vulnerability Simulation Environment</div>
        </div>
        
        <!-- Sensor Data Grid -->
        <div class="dashboard-grid">
            <div class="card">
                <h3>🌡️ TEMPERATURE</h3>
                <div class="sensor-value temp" id="temperature">--°C</div>
                <div class="info-display">
                    Sensor: DHT11 - GPIO 4<br>
                    Last Update: <span id="tempTime">--</span>
                </div>
            </div>
            
            <div class="card">
                <h3>💧 HUMIDITY</h3>
                <div class="sensor-value humidity" id="humidity">--%</div>
                <div class="info-display">
                    Sensor: DHT11 - GPIO 4<br>
                    Last Update: <span id="humTime">--</span>
                </div>
            </div>
            
            <div class="card">
                <h3>💾 MEMORY</h3>
                <div class="sensor-value memory" id="memory">-- bytes</div>
                <div class="info-display">
                    Heap Status: Active<br>
                    Last Check: <span id="memTime">--</span>
                </div>
            </div>
        </div>
        
        <!-- Status Panel -->
        <div class="status-panel">
            <h3>DEVICE STATUS OVERVIEW</h3>
            <div class="metric">
                <span class="metric-label">Requests Handled:</span>
                <span class="metric-value" id="requestCount">0</span>
            </div>
            <div class="metric">
                <span class="metric-label">Slowloris Connections:</span>
                <span class="metric-value" id="slowlorisCount">0</span>
            </div>
            <div class="metric">
                <span class="metric-label">System Status:</span>
                <span class="status-indicator status-normal" id="statusText">NORMAL</span>
            </div>
            <div class="metric">
                <span class="metric-label">Free Memory:</span>
                <span class="metric-value" id="freeMemory">--</span>
            </div>
            <div class="metric">
                <span class="metric-label">Last Update:</span>
                <span class="metric-value" id="lastUpdate">--</span>
            </div>
        </div>
        
        <!-- Attack Log -->
        <div class="card">
            <h3>📋 ATTACK LOG</h3>
            <div class="log-panel" id="attackLog">
                <div class="log-entry">System started - Monitoring active</div>
            </div>
        </div>
        
        <!-- Attack Panel -->
        <div class="attack-panel">
            <h2>🚨 SECURITY VULNERABILITY DEMONSTRATION</h2>
            <p>Click buttons below to demonstrate different security vulnerabilities:</p>
            
            <button class="attack" onclick="startMemoryAttack()">
                💥 MEMORY EXHAUSTION ATTACK
            </button>
            
            <button class="attack" onclick="startRequestFlood()">
                🌊 REQUEST FLOOD ATTACK
            </button>
            
            <button class="attack" onclick="startSlowLoris()">
                🐌 SLOWLORIS ATTACK 
            </button>
            
            <button class="attack pulse" onclick="startCombinedAttack()">
                ⚡ COMBINED ATTACK 
            </button>

            <button class="attack" onclick="startNuclearAttack()" style="background: linear-gradient(135deg, #ff0000, #800000);">
                💣 NUCLEAR ATTACK (CRASH)
            </button>
        </div>
        
        <!-- Control Panel -->
        <div class="card">
            <button class="reset" onclick="resetDevice()">
                🔄 RESET DEVICE & COUNTERS
            </button>
        </div>
    </div>

    <script>
        const TARGET_IP = ")rawliteral" + WiFi.localIP().toString() + R"rawliteral(";

        function logAttack(message, type = '') {
            const log = document.getElementById('attackLog');
            const timestamp = new Date().toLocaleTimeString();
            const entry = document.createElement('div');
            entry.className = 'log-entry' + (type ? ' log-' + type : '');
            entry.textContent = `[${timestamp}] ${message}`;
            log.appendChild(entry);
            log.scrollTop = log.scrollHeight;
        }

        // Update device status every 2 seconds
        function updateStatus() {
            fetch('/status')
                .then(response => {
                    if (!response.ok) {
                        throw new Error('Device not responding');
                    }
                    return response.json();
                })
                .then(data => {
                    // Update sensor values
                    document.getElementById('temperature').textContent = data.temp.toFixed(1) + '°C';
                    document.getElementById('humidity').textContent = data.humidity.toFixed(1) + '%';
                    document.getElementById('memory').textContent = data.freeMemory.toLocaleString() + ' bytes';
                    document.getElementById('requestCount').textContent = data.requests;
                    document.getElementById('freeMemory').textContent = data.freeMemory.toLocaleString() + ' bytes';
                    document.getElementById('slowlorisCount').textContent = data.slowlorisConnections;
                    
                    // Update status with theme
                    const statusText = document.getElementById('statusText');
                    statusText.textContent = data.status;
                    
                    // Remove all status classes
                    statusText.className = 'status-indicator';
                    
                    // Add appropriate status class
                    if(data.status === 'NORMAL') {
                        statusText.classList.add('status-normal');
                    } else if(data.status === 'WARNING') {
                        statusText.classList.add('status-warning');
                    } else if(data.status === 'CRITICAL') {
                        statusText.classList.add('status-critical');
                    } else if(data.status === 'UNDER ATTACK') {
                        statusText.classList.add('status-under-attack');
                    } else if(data.status === 'COMBINED ATTACK') {
                        statusText.classList.add('status-combined-attack');
                    }
                    
                    // Update timestamps
                    const now = new Date();
                    document.getElementById('lastUpdate').textContent = now.toLocaleTimeString();
                    document.getElementById('tempTime').textContent = now.toLocaleTimeString();
                    document.getElementById('humTime').textContent = now.toLocaleTimeString();
                    document.getElementById('memTime').textContent = now.toLocaleTimeString();
                    
                })
                .catch(error => {
                    console.log('Device might be down:', error);
                    document.getElementById('statusText').textContent = 'OFFLINE';
                    document.getElementById('statusText').className = 'status-indicator status-critical';
                    document.getElementById('freeMemory').textContent = 'DEVICE CRASHED';
                    document.getElementById('slowlorisCount').textContent = '0';
                    logAttack('Device crashed - DoS attack successful!', 'critical');
                });
        }

        // Attack functions - IMMEDIATELY trigger LED on click
        function startMemoryAttack() {
            logAttack(' Starting Memory Exhaustion Attack...', 'warning');
            fetch('/memory-attack')
                .then(response => response.text())
                .then(data => {
                    console.log('Memory attack result:', data);
                    logAttack(' Memory attack completed ', 'warning');
                    updateStatus();
                })
                .catch(error => {
                    console.log('Attack failed:', error);
                    logAttack('Memory attack failed - device may be crashing', 'critical');
                });
        }

        function startRequestFlood() {
            logAttack(' Starting Request Flood Attack...', 'warning');
            // Send multiple requests rapidly
            for(let i = 0; i < 15; i++) {
                fetch('/flood-attack')
                    .then(response => response.text())
                    .then(data => {
                        if(i === 0) logAttack('Flood attack in progress', 'warning');
                    })
                    .catch(error => {
                        if(i === 0) logAttack(' Flood attack failing - device overloaded', 'critical');
                    });
            }
            setTimeout(updateStatus, 1000);
        }

        function startSlowLoris() {
            logAttack('Starting REAL Slowloris Attack...', 'warning');
            
            for(let i = 0; i < 3; i++) {
                const xhr = new XMLHttpRequest();
                xhr.open('GET', '/slow-attack', true);
                xhr.timeout = 35000;
                
                xhr.onload = function() {
                    logAttack(`Slowloris connection ${i+1} completed `, 'warning');
                    updateStatus();
                };
                
                xhr.onerror = function() {
                    logAttack(` Slowloris connection ${i+1} failed`, 'critical');
                };
                
                xhr.ontimeout = function() {
                    logAttack(` Slowloris connection ${i+1} timed out`, 'warning');
                };
                
                xhr.send();
                logAttack(`Slowloris connection ${i+1} opened - holding for 30s`, 'warning');
            }
            
            updateStatus();
        }

        function startCombinedAttack() {
            if(confirm(' WARNING: Combined attack will trigger multiple attack vectors! Continue?')) {
                logAttack(' LAUNCHING COMBINED ATTACK - Multiple vectors activated!', 'critical');
                
                // Launch all attacks with delays
                startMemoryAttack();
                
                setTimeout(() => {
                    startMemoryAttack(); // Second memory attack
                }, 500);
                
                setTimeout(() => {
                    startRequestFlood();
                }, 1000);
                
                setTimeout(() => {
                    startSlowLoris();
                }, 1500);
                
                alert(' Combined attack launched! ');
            }
        }

        function startNuclearAttack() {
            if(confirm(' NUCLEAR OPTION: This will IMMEDIATELY crash the device! Continue?')) {
                logAttack(' ACTIVATING NUCLEAR OPTION - Immediate crash!', 'critical');
                fetch('/kill')
                    .then(() => logAttack('Nuclear launch detected...', 'critical'))
                    .catch(() => logAttack(' Device crashing...', 'critical'));
            }
        }

        function resetDevice() {
            if(confirm('Reset device counters and status?')) {
                fetch('/reset')
                    .then(() => {
                        logAttack('Device reset completed - LED back to solid ON', '');
                        updateStatus();
                    })
                    .catch(error => {
                        logAttack('Reset failed - device may be crashed', 'critical');
                    });
            }
        }

        // Auto-update every 2 seconds
        setInterval(updateStatus, 2000);
        updateStatus(); // Initial call
        
        // Add some dynamic effects
        setInterval(() => {
            const elements = document.querySelectorAll('.sensor-value');
            elements.forEach(el => {
                el.classList.toggle('pulse');
            });
        }, 4000);
        
        // Initial log
        logAttack('System monitoring started - Status: NORMAL');
    </script>
</body>
</html>
    )rawliteral";
    
    server.send(200, "text/html", html);
}

void handleStatus() {
    requestCount++;
    
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    // Calculate status with proper logic
    String status = calculateStatus();
    
    String json = "{";
    json += "\"temp\":" + String(isnan(temp) ? 0 : temp) + ",";
    json += "\"humidity\":" + String(isnan(humidity) ? 0 : humidity) + ",";
    json += "\"freeMemory\":" + String(freeMemory) + ",";
    json += "\"requests\":" + String(requestCount) + ",";
    json += "\"slowlorisConnections\":" + String(slowlorisConnections) + ",";
    json += "\"status\":\"" + status + "\"";
    json += "}";
    
    server.send(200, "application/json", json);
    
    // Serial output for debugging
    Serial.printf(" REQ: %lu |  MEM: %lu |  SLOW: %d |  STATUS: %s |  LED: %d\n", 
                  requestCount, freeMemory, slowlorisConnections, status.c_str(), ledPattern);
}

void handleMemoryAttack() {
    requestCount++;
    Serial.println(" MEMORY ATTACK - Allocating 100KB instantly!");
    
    // IMMEDIATELY start LED blinking
    startMemoryAttackBlink();
    
    // Aggressive memory allocation
    if (blockCount < 20) {
        memoryBlocks[blockCount] = new String();
        for(int i = 0; i < 2000; i++) {
            *memoryBlocks[blockCount] += "MEMORY_ATTACK_1234567890_ABCDEFGHIJKLMNOPQRSTUVWXYZ_";
            if (memoryBlocks[blockCount]->length() > 10000) break;
        }
        blockCount++;
        
        Serial.printf(" Allocated block %d - String length: %d\n", 
                     blockCount, memoryBlocks[blockCount-1]->length());
    } else {
        Serial.println(" MAXIMUM MEMORY BLOCKS ALLOCATED - SYSTEM CRITICAL");
    }
    
    freeMemory = ESP.getFreeHeap();
    
    String response = " MEMORY ATTACK - SYSTEM CRITICAL!\n";
    response += "Allocated blocks: " + String(blockCount) + "\n";
    response += "Approx memory used: " + String(blockCount * 10000) + " bytes\n";
    response += "Free memory: " + String(freeMemory) + " bytes\n";
    response += "Total requests: " + String(requestCount);
    
    server.send(200, "text/plain", response);
    
    Serial.printf(" Memory After Attack: %lu bytes | Blocks: %d\n", freeMemory, blockCount);
    
    // Auto-crash if memory too low
    if (freeMemory < 120000) {
        Serial.println(" CRITICAL MEMORY - FORCING REBOOT IN 3 SECONDS ");
        delay(3000);
        ESP.restart();
    }
}

void handleFloodAttack() {
    requestCount += 25;
    freeMemory = ESP.getFreeHeap();
    
    Serial.println("MEGA FLOOD ATTACK - Added 25 requests instantly!");
    
    // IMMEDIATELY start LED blinking
    startFloodAttackBlink();
    
    // Create memory pressure during flood
    String floodData = "";
    for(int i = 0; i < 500; i++) {
        floodData += "FLOOD_ATTACK_DATA_";
    }
    
    String response = " MEGA FLOOD ATTACK!\n";
    response += "Requests increased by: 25\n";
    response += "Total requests: " + String(requestCount) + "\n";
    response += "Memory pressure applied: " + String(floodData.length()) + " bytes\n";
    response += "Free memory: " + String(freeMemory) + " bytes";
    
    server.send(200, "text/plain", response);
}

void handleSlowAttack() {
    requestCount++;
    slowlorisConnections++;
    
    Serial.printf("🐌 SLOWLORIS ATTACK - Connection %d opened (30 seconds)\n", slowlorisConnections);
    
    // IMMEDIATELY start LED blinking
    startSlowlorisAttackBlink();
    
    WiFiClient client = server.client();
    
    if (client.connected()) {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println("Connection: keep-alive");
        client.println("Keep-Alive: timeout=30");
        client.println();
        
        unsigned long startTime = millis();
        unsigned long lastSend = millis();
        
        while (client.connected() && (millis() - startTime < 30000)) {
            if (millis() - lastSend > 5000) {
                unsigned long elapsed = (millis() - startTime) / 1000;
                String data = "Slowloris active: " + String(elapsed) + "s / 30s\n";
                client.print(data);
                lastSend = millis();
                Serial.printf("🐌 Connection %d alive: %lu/30 seconds\n", slowlorisConnections, elapsed);
            }
            
            delay(100);
            updateLED(); // Keep LED updated during slowloris
            
            if (ESP.getFreeHeap() < 50000) {
                Serial.println(" Breaking Slowloris - critical memory");
                break;
            }
        }
        
        client.print(" Slowloris connection completed\n");
    }
    
    client.stop();
    slowlorisConnections--;
    Serial.printf(" SLOWLORIS Connection %d closed\n", slowlorisConnections + 1);
}

void handleKillSwitch() {
    Serial.println(" KILL SWITCH ACTIVATED - IMMEDIATE CRASH! ");
    
    server.send(200, "text/plain", " DEVICE CRASHING IN 3 SECONDS...");
    
    delay(100);
    
    // Force crash with LED solid on
    setLEDPattern(0); // Solid ON before crash
    Serial.println(" Starting infinite memory allocation...");
    while(true) {
        String* crashString = new String();
        for(int i = 0; i < 10000; i++) {
            *crashString += "CRASH_THIS_DEVICE_NOW_1234567890_ABCDEFGHIJKLMNOPQRSTUVWXYZ_";
        }
        Serial.printf(" Allocated crash block: %d bytes\n", crashString->length());
        delay(100);
    }
}

void handleReset() {
    // Free all memory properly
    for(int i = 0; i < 20; i++) {
        if (memoryBlocks[i] != nullptr) {
            delete memoryBlocks[i];
            memoryBlocks[i] = nullptr;
        }
    }
    blockCount = 0;
    
    memoryHog = "";
    requestCount = 0;
    slowlorisConnections = 0;
    freeMemory = ESP.getFreeHeap();
    
    // Reset attack timers
    memoryAttackEndTime = 0;
    floodAttackEndTime = 0;
    slowlorisAttackEndTime = 0;
    
    // Reset LED to normal (Solid ON)
    setLEDPattern(0);
    
    Serial.println(" SYSTEM RESET - All memory freed, counters reset!");
    
    server.send(200, "text/plain", 
        "System reset complete!\n" +
        String("Memory freed: All allocations cleared\n") +
        "Request counter: Reset to 0\n" +
        "Slowloris connections: Reset to 0\n" +
        "Free memory: " + String(freeMemory) + " bytes\n" +
        "System restored to normal operation");
}

// ==================== SETUP ====================

void setup() {
    // Initialize Serial
    Serial.begin(115200);
    delay(1000);
    
    // Initialize LED pin
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH); // Start with LED ON (Normal)
    
    Serial.println("\n\n========================================");
    Serial.println("IoT Vulnerability Demonstration ");
    Serial.println("ESP32 DoS Vulnerability Demo ");
    Serial.println("========================================\n");
    
    // Initialize DHT sensor
    dht.begin();
    Serial.println(" DHT11 sensor initialized");
    
    // Connect to WiFi
    Serial.print(" Connecting to WiFi: ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n WiFi connected!");
        Serial.print(" IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\n WiFi connection failed!");
        Serial.println("Please check your credentials");
        while(1) { delay(1000); }
    }
    
    // Setup server routes
    server.on("/", handleRoot);
    server.on("/status", handleStatus);
    server.on("/memory-attack", handleMemoryAttack);
    server.on("/flood-attack", handleFloodAttack);
    server.on("/slow-attack", handleSlowAttack);
    server.on("/reset", handleReset);
    server.on("/kill", handleKillSwitch);
    
    // Handle 404 errors
    server.onNotFound([]() {
        requestCount++;
        server.send(404, "text/plain", "Endpoint not found. Available: /, /status, /memory-attack, /flood-attack, /slow-attack, /reset, /kill");
    });
    
    server.begin();
    Serial.println(" Web server started successfully!");
    Serial.println(" Access the dashboard at: http://" + WiFi.localIP().toString());
    
    // Initial memory reading
    freeMemory = ESP.getFreeHeap();
    Serial.printf("🎯 Initial Memory: %lu bytes | Ready for attacks!\n", freeMemory);
}

// ==================== MAIN LOOP ====================

void loop() {
    server.handleClient();
    
    // Update LED status
    updateLED();
    
    // Periodic sensor reading
    static unsigned long lastSensorRead = 0;
    if (millis() - lastSensorRead > SENSOR_READ_INTERVAL) {
        float temp = dht.readTemperature();
        float humidity = dht.readHumidity();
        
        if (!isnan(temp) && !isnan(humidity)) {
            Serial.printf(" Sensor Read - Temp: %.1f°C, Humidity: %.1f%%\n", temp, humidity);
        } else {
            Serial.println(" Failed to read from DHT sensor!");
        }
        
        lastSensorRead = millis();
    }
    
    // Auto-reset if memory too low
    freeMemory = ESP.getFreeHeap();
    if (freeMemory < 30000) {
        Serial.println("\n CRITICAL MEMORY LEVEL REACHED ");
        Serial.printf("Free Memory: %lu bytes - Device will reboot!\n", freeMemory);
        setLEDPattern(2); // Fast blink before reboot
        delay(2000);
        ESP.restart();
    }
    
    // Status monitoring
    static unsigned long lastStatusPrint = 0;
    if (millis() - lastStatusPrint > 15000) {
        float temp = dht.readTemperature();
        float humidity = dht.readHumidity();
        Serial.printf("\n[Status] Memory: %lu bytes | Requests: %lu | Slowloris: %d | Temp: %.1f°C | Humidity: %.1f%% | LED Pattern: %d\n",
                     freeMemory, requestCount, slowlorisConnections, 
                     isnan(temp) ? 0 : temp, isnan(humidity) ? 0 : humidity, ledPattern);
        lastStatusPrint = millis();
    }
    
    delay(10); // Small delay for LED updates
}