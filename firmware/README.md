# Firmware

Arduino UNO source code for the Anti Sleep Alarm prototype.

```text
firmware/
└── anti_sleep_alarm/
    └── anti_sleep_alarm.ino   ← open this in Arduino IDE
```

## Quick start

1. Open `anti_sleep_alarm/anti_sleep_alarm.ino` in Arduino IDE  
2. Board: **Arduino UNO**  
3. Select COM port → Upload  
4. Optional: Serial Monitor at **115200** baud  

## Pin summary

| Function | Pin |
|----------|-----|
| Blink sensor | D2 |
| Buzzer | D12 |
| Relay | D13 |

Full wiring: [`../docs/wiring.md`](../docs/wiring.md)  
Firmware design notes: [`../docs/software.md`](../docs/software.md)
