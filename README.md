
# ESP32 Tic Tac Toe on OLED

This is a **Tic Tac Toe game** developed using an **ESP32 microcontroller**, **push buttons** for input, and an **SSD1306 OLED display** for output. The project features both **single-player** and **multiplayer** modes.

In single-player mode, the game includes **three difficulty levels**: *Easy*, *Medium*, and *Hard*, offering a fun and challenging experience against an AI opponent.

---

# Features

- Two game modes: Single-player (vs AI) and Multiplayer (2-player)
- AI with three difficulty levels
- Intuitive button-based navigation
- Visual display optimized for 128x64 OLED

---

## Hardware Used

| Component         | Details                          |
|------------------|----------------------------------|
| Microcontroller   | ESP32 DevKit v1                  |
| Display           | SSD1306 OLED (128x64, I²C)       |
| Input             | 6 Push Buttons (Navigation + Back) |
| Breadboard        | For prototyping                  |
| Jumper Wires      | For connections                  |

---

## Pin Configuration

### Button Pins
| Button   | ESP32 Pin |
|----------|------------|
| Left     | D13        |
| Right    | D12        |
| Up       | D14        |
| Down     | D27        |
| Select   | D26        |
| Back     | D25        |

### OLED Pins
| OLED Pin | ESP32 Pin |
|----------|------------|
| GND      | GND        |
| VCC      | 3.3V       |
| SCL      | D22        |
| SDA      | D21        |

---

## Libraries Used

- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)

Install via **Arduino Library Manager** or PlatformIO.

---

## Preview
[Main Manu](image/MainMenu.jpg)
[Levels](images/Levels.jpg)
[Game on OLED](images/Game.jpg)


---


## License

This project is open-source and available under the [MIT License](LICENSE).

---

