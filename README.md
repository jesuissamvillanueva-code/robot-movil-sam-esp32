<div align="center">
  
  # 🤖 SAM - Robot Móvil ESP32

  <p align="center">
    <img src="https://img.shields.io/badge/Hardware-ESP32-E34F26?style=for-the-badge&logo=espressif&logoColor=white" alt="ESP32" />
    <img src="https://img.shields.io/badge/Driver-L298N_Mini-00599C?style=for-the-badge" alt="L298N Mini" />
    <img src="https://img.shields.io/badge/Lenguaje-C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++" />
    <img src="https://img.shields.io/badge/Plataforma-iOS_/_Safari-000000?style=for-the-badge&logo=apple&logoColor=white" alt="iOS" />
  </p>

  > **Sistema de control inalámbrico avanzado para robot móvil utilizando un microcontrolador ESP32 y un puente H L298N Mini.**
  > <br> El proyecto genera un punto de acceso web local (`172.20.10.x`) con una interfaz gráfica con temática de maquinaria pesada, optimizada para control táctil desde dispositivos móviles.

</div>

<br>

## 📑 Tabla de Contenidos
- [✨ Características Principales](#-características-principales)
- [📋 Lista de Materiales](#-lista-de-materiales-detallada)
- [🔌 Esquema de Conexiones](#-esquema-de-conexiones-pinout)
- [🚀 Instalación y Uso](#-instalación-y-uso)

---

## ✨ Características Principales
* **Interfaz UI/UX Embebida:** Tablero web almacenado directamente en la memoria PROGMEM del microcontrolador, sin necesidad de tarjetas SD ni servidores externos.
* **Protección Táctil iOS:** Reglas CSS estrictas (`user-select: none;`, `-webkit-touch-callout: none;`) para evitar menús contextuales o selección de texto al mantener presionados los controles de dirección.
* **Conexión Directa M2M:** El ESP32 se enlaza directamente al "Punto de Acceso Personal" del teléfono móvil, eliminando la latencia y la necesidad de routers intermedios.
* **Diseño Libre y Adaptable:** La electrónica es universal. El código y el circuito pueden integrarse en cualquier estructura según la creatividad del usuario.

## 📋 Lista de Materiales Detallada

## 📋 Lista de Materiales Detallada

### 🧠 Lógica y Control
- 🎛️ **1x** Placa de desarrollo ESP32 (WROOM-32, 30 o 38 pines).
- 🕹️ **1x** Módulo Driver de motor L298N Mini (MX1508).

### ⚡ Sistema de Alimentación
- 🔋 **1x** Batería cuadrada de 9V *(Alternativa: 2x Baterías Li-ion 18650 con portapilas para mayor autonomía y amperaje)*.
- 🔌 **1x** Conector/Broche para batería de 9V.
- 📉 **1x** Regulador de voltaje L7805CV *(Para reducir los 9V a 5V limpios para el ESP32)*.
- 🔘 **1x** Interruptor (Switch) de encendido/apagado.

### ⚙️ Mecánica y Movimiento
- 🏎️ **2x** Motores DC con motorreductor (Tipo TT 200RPM 6V).
- 🛞 **2x** Ruedas de tracción (Una para cada lado, acopladas a los motorreductores).
- 🛹 **1x** Rueda loca omnidireccional o rueda libre pasiva *(Actúa como tercer punto de apoyo para mantener el equilibrio del chasis, distribuir el peso y evitar que el robot vuelque hacia adelante o hacia atrás)*.

### 🛠️ Estructura y Cableado
- 🔗 **Varios** Cables jumper (Macho-Macho y Macho-Hembra).
- 🏗️ **Estructura/Chasis libre:** El diseño físico es totalmente adaptable al gusto del creador. Puedes usar un chasis de acrílico comercial, imprimir una estructura en 3D, adaptar el circuito dentro de un carro de juguete existente, o incluso construir una réplica tipo WALL-E. ¡Depende de lo que desees realizar!

## 🔌 Esquema de Conexiones (Pinout)

> 📄 **Recurso visual:** [Ver Diagrama de Conexiones Completo (PDF)](./diagrama_carro_movil.pdf)

| Componente | Terminal | Conexión en ESP32 / Sistema |
| :--- | :--- | :--- |
| **Interruptor** | `Pines` | Intercalado en el cable positivo `(+)` entre la batería y todo el sistema |
| **L298N Mini** | `IN1` | `GPIO 16` |
| **L298N Mini** | `IN2` | `GPIO 17` |
| **L298N Mini** | `IN3` | `GPIO 18` |
| **L298N Mini** | `IN4` | `GPIO 19` |
| **L298N Mini** | `VCC (+)` | Batería 9V `(+)` *(Después del interruptor)* |
| **L298N Mini** | `GND (-)` | Batería 9V `(-)` **Y** `GND` del ESP32 |
| **L298N Mini** | `Motor A` | Motor Derecho |
| **L298N Mini** | `Motor B` | Motor Izquierdo |
| **Regulador 7805**| `Pin 1 (IN)` | Batería 9V `(+)` *(Después del interruptor)* |
| **Regulador 7805**| `Pin 2 (GND)` | `GND` Común *(Unido a toda la tierra del sistema)* |
| **Regulador 7805**| `Pin 3 (OUT)`| Pin `VIN` o `5V` (ESP32) |

## 🚀 Instalación y Uso

1. **Carga del Código:** Abre el archivo `walle_sam_web.ino` en el Arduino IDE y sube el firmware al ESP32. Asegúrate de configurar la velocidad del Monitor Serie a `115200` baudios.
2. **Red Móvil:** En tu celular, activa *Compartir internet* (Punto de acceso). Configura previamente el SSID como `iPhone` y la contraseña como `villanueva`.
3. **Sincronización de Hardware:** Enciende el robot desde el interruptor general. El LED azul interno de la placa (`GPIO 2`) parpadeará mientras establece el enlace de red y se quedará fijo al conectarse con éxito.
4. **Despliegue de Control:** Si usas iOS, abre el navegador Safari e ingresa a la IP asignada (usualmente `172.20.10.2` o `172.20.10.3`) para acceder al tablero de control industrial.

<br>

---
<div align="center">
  <b>Desarrollado por:</b> Samuel Elí Villanueva Chávez <br>
  <i>Tecnología, Informática y Telecomunicaciones (UNDAC) | Sam </i>
</div>
