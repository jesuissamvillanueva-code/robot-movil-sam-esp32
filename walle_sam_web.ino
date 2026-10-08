#include <WiFi.h>
#include <WebServer.h>

// Credenciales de la red WiFi compartida desde el celular
const char* ssid = "iPhone";
const char* password = "villanueva";

WebServer server(80);

// Pines PWM del ESP32 conectados al driver L298N Mini
const int IN1 = 16;
const int IN2 = 17;
const int IN3 = 18;
const int IN4 = 19;

// Pin del LED azul integrado en el ESP32
const int LED_AZUL = 2; 

// Propiedades PWM del ESP32
const int freq = 5000;
const int resolution = 8; // Resolución de 8 bits (0 a 255)

// Velocidad máxima al 100%
int speed = 255;

// Interfaz Web HTML/CSS - DISEÑO WALL-E / VEHÍCULO INDUSTRIAL + "SAM"
const char html_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
  <title>SAM - Robot Móvil</title>
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Russo+One&display=swap');
    
    body { 
      font-family: 'Russo One', sans-serif; 
      text-align: center; 
      margin: 0;
      padding-top: 15px; 
      /* Fondo gris oscuro industrial */
      background-color: #2b2b2b; 
      background-image: repeating-linear-gradient(45deg, #222 25%, transparent 25%, transparent 75%, #222 75%, #222), repeating-linear-gradient(45deg, #222 25%, #2b2b2b 25%, #2b2b2b 75%, #222 75%, #222);
      background-position: 0 0, 10px 10px;
      background-size: 20px 20px;
      color: #E5A91A; 
      /* Protecciones iOS */
      -webkit-touch-callout: none;
      -webkit-user-select: none;
      -khtml-user-select: none;
      -moz-user-select: none;
      -ms-user-select: none;
      user-select: none;
    }
    
    /* Título principal con el nombre */
    h1 {
      font-size: 55px;
      color: #E5A91A;
      margin: 0;
      letter-spacing: 5px;
      text-shadow: 3px 3px 0px #111, 6px 6px 10px rgba(0,0,0,0.8);
    }

    h2 { 
      color: #fff; 
      font-size: 20px;
      text-transform: uppercase; 
      letter-spacing: 3px; 
      margin-top: 5px;
      margin-bottom: 25px;
      background: #111;
      display: inline-block;
      padding: 8px 25px;
      border-radius: 8px;
      border: 2px solid #555;
      box-shadow: 0 4px 8px rgba(0,0,0,0.5);
    }

    .grid { 
      display: grid; 
      grid-template-columns: 1fr 1fr 1fr; 
      gap: 12px; 
      max-width: 340px; 
      margin: 0 auto; 
    }
    .btn { 
      padding: 20px 10px; 
      font-size: 18px; 
      font-family: 'Russo One', sans-serif;
      cursor: pointer; 
      border-radius: 8px; 
      /* Color amarillo maquinaria Wall-E */
      background-color: #E5A91A; 
      background-image: linear-gradient(to bottom, #f4c038, #d89b0d);
      color: #111; 
      border: 3px solid #111;
      box-shadow: 0 6px 0 #8a6308, 0 10px 10px rgba(0,0,0,0.5), inset 0px 2px 5px rgba(255,255,255,0.4);
      touch-action: manipulation; 
      transition: all 0.1s ease; 
      /* Protecciones iOS */
      -webkit-touch-callout: none;
      -webkit-user-select: none;
      user-select: none;
      -webkit-tap-highlight-color: transparent;
      outline: none;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
    }
    .btn span { font-size: 28px; margin-bottom: 5px; }
    
    .btn:active { 
      transform: translateY(6px);
      box-shadow: 0 0px 0 #8a6308, 0 4px 4px rgba(0,0,0,0.5), inset 0px 4px 8px rgba(0,0,0,0.4);
      background-image: linear-gradient(to bottom, #d89b0d, #b88308);
    }

    /* Botón central rojo de freno (Fondo de Wall-E en la foto) */
    .btn-stop { 
      background-color: #D1101D; 
      background-image: linear-gradient(to bottom, #ed212f, #b30b15);
      color: #fff;
      box-shadow: 0 6px 0 #7a060d, 0 10px 10px rgba(0,0,0,0.5), inset 0px 2px 5px rgba(255,255,255,0.4);
    }
    .btn-stop:active { 
      background-image: linear-gradient(to bottom, #b30b15, #8a060e);
      box-shadow: 0 0px 0 #7a060d, 0 4px 4px rgba(0,0,0,0.5), inset 0px 4px 8px rgba(0,0,0,0.6);
    }
    
    /* Forma especial para acelerador y reversa (más altos) */
    .pedal { padding: 30px 10px; }

    .empty { background: transparent; border: none; box-shadow: none; pointer-events: none; }
    
    .status-box {
      margin-top: 30px;
      padding: 15px;
      border: 4px solid #444;
      background: #111;
      display: inline-block;
      min-width: 260px;
      border-radius: 8px;
      box-shadow: inset 0 0 15px rgba(0,0,0,1);
    }
    .status { 
      font-size: 16px; 
      color: #E5A91A; 
      letter-spacing: 1px;
    }
  </style>
</head>
<body oncontextmenu="return false;">
  <h1>SAM</h1>
  <h2>ROBOT MÓVIL</h2>
  <div class="grid">
    <div class="empty"></div>
    <button class="btn pedal" onmousedown="sendCommand('/forward')" onmouseup="sendCommand('/stop')" ontouchstart="sendCommand('/forward')" ontouchend="sendCommand('/stop')"><span>▲</span>ACELERAR</button>
    <div class="empty"></div>
    
    <button class="btn" onmousedown="sendCommand('/left')" onmouseup="sendCommand('/stop')" ontouchstart="sendCommand('/left')" ontouchend="sendCommand('/stop')"><span>◀</span>IZQ</button>
    <button class="btn btn-stop" onclick="sendCommand('/stop')"><span>■</span>FRENO</button>
    <button class="btn" onmousedown="sendCommand('/right')" onmouseup="sendCommand('/stop')" ontouchstart="sendCommand('/right')" ontouchend="sendCommand('/stop')"><span>▶</span>DER</button>
    
    <div class="empty"></div>
    <button class="btn pedal" onmousedown="sendCommand('/backward')" onmouseup="sendCommand('/stop')" ontouchstart="sendCommand('/backward')" ontouchend="sendCommand('/stop')"><span>▼</span>REVERSA</button>
    <div class="empty"></div>
  </div>
  
  <div class="status-box">
    <div class="status" id="estado">MOTOR EN ESPERA</div>
  </div>

  <script>
    function sendCommand(cmd) {
      fetch(cmd);
      let states = {
        '/forward':'[ ESTADO: ACELERANDO ]', 
        '/backward':'[ ESTADO: EN REVERSA ]', 
        '/left':'[ DIRECCIÓN: IZQUIERDA ]', 
        '/right':'[ DIRECCIÓN: DERECHA ]', 
        '/stop':'[ FRENO ACTIVADO ]'
      };
      document.getElementById('estado').innerText = states[cmd];
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  // Configurar LED azul integrado
  pinMode(LED_AZUL, OUTPUT);
  digitalWrite(LED_AZUL, LOW); 

  // Configuración PWM
  ledcAttach(IN1, freq, resolution);
  ledcAttach(IN2, freq, resolution);
  ledcAttach(IN3, freq, resolution);
  ledcAttach(IN4, freq, resolution);

  stopMotors();

  // Conexión WiFi con parpadeo de LED Azul
  Serial.print("\nIniciando secuencia de red...\nBuscando SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  bool ledState = false;
  while (WiFi.status() != WL_CONNECTED) {
    ledState = !ledState;
    digitalWrite(LED_AZUL, ledState ? HIGH : LOW); // Parpadea el LED mientras conecta
    delay(500);
    Serial.print(".");
  }
  
  // Enciende el LED azul fijo al conectar exitosamente
  digitalWrite(LED_AZUL, HIGH); 
  
  Serial.println("\n\n>>> ENLACE ESTABLECIDO <<<");
  Serial.print("IP DEL TABLERO DE CONTROL: ");
  Serial.println(WiFi.localIP());

  // Rutas del servidor web
  server.on("/", []() { server.send(200, "text/html", html_page); });
  server.on("/forward", []() { moveForward(); server.send(200, "text/plain", "OK"); });
  server.on("/backward", []() { moveBackward(); server.send(200, "text/plain", "OK"); });
  server.on("/left", []() { moveLeft(); server.send(200, "text/plain", "OK"); });
  server.on("/right", []() { moveRight(); server.send(200, "text/plain", "OK"); });
  server.on("/stop", []() { stopMotors(); server.send(200, "text/plain", "OK"); });

  server.begin();
}

void loop() {
  server.handleClient();
}

// ==========================================
// LÓGICA DE MOTORES
// ==========================================

void moveForward() {
  Serial.println("[TABLERO] ACELERADOR PISADO: Ambos motores hacia adelante");
  ledcWrite(IN1, 0);     
  ledcWrite(IN2, speed); // Motor A Adelante
  ledcWrite(IN3, 0);     
  ledcWrite(IN4, speed); // Motor B Adelante
}

void moveBackward() {
  Serial.println("[TABLERO] REVERSA ACTIVADA: Ambos motores hacia atrás");
  ledcWrite(IN1, speed); // Motor A Atrás
  ledcWrite(IN2, 0);     
  ledcWrite(IN3, speed); // Motor B Atrás
  ledcWrite(IN4, 0);     
}

// Girar a la izquierda
void moveLeft() {
  Serial.println("[TABLERO] VOLANTE A LA IZQUIERDA");
  ledcWrite(IN1, speed); // Motor A Atrás
  ledcWrite(IN2, 0);     
  ledcWrite(IN3, 0);     
  ledcWrite(IN4, speed); // Motor B Adelante
}

// Girar a la derecha
void moveRight() {
  Serial.println("[TABLERO] VOLANTE A LA DERECHA");
  ledcWrite(IN1, 0);     
  ledcWrite(IN2, speed); // Motor A Adelante
  ledcWrite(IN3, speed); // Motor B Atrás
  ledcWrite(IN4, 0);     
}

void stopMotors() {
  Serial.println("[TABLERO] FRENO DE MANO: Motores apagados");
  ledcWrite(IN1, 0);
  ledcWrite(IN2, 0);
  ledcWrite(IN3, 0);
  ledcWrite(IN4, 0);
}