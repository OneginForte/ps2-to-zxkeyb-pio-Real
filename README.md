## Контроллер ZX Spectrum клавиатуры из usb и ps/2 на ключах MT8816
## Используется RP2040-zero (waveshare_rp2040_zero)

На основе проектов 🔗: 
- [No0ne](https://github.com/No0ne/ps2x2pico)
- [billgilbert7000](https://github.com/billgilbert7000/usb_to_zx_spectrum)
- [MikhaelKaa](https://github.com/MikhaelKaa/usb2spectrum)

### Для корректной работы TinyUSB необходим [patch1](https://github.com/OneginForte/ps2-to-zxkeyb-pio-Real/blob/Real/tinyusb-0.17.0-dualusb.patch)
и [patch2](https://github.com/OneginForte/ps2-to-zxkeyb-pio-Real/blob/Real/0002-Add-missing-reset-recovery-delay.patch) из корня. 
### Актуальная прошивка в папке [firmware](https://github.com/OneginForte/ps2-to-zxkeyb-pio-Real/blob/Real/firmware/ps2hidtozx.uf2)

### Используемые выводы PICO 

Для работы с MT8816:
- AX0 `GPIO4`
- AX1 `GPIO5`
- AX2 `GPIO6`
- AX3 `GPIO7`
- AY0 `GPIO8`
- AY1 `GPIO9`
- AY2 `GPIO10`
- STBMT `GPIO11`
- CSMT  `GPIO12`
- RSTMT `GPIO13`
- DATMT `GPIO14`

Для интерфейса PS/2: 
-  PS/2 DATA `GPIO2` 
-  CLOCK  `GPIO3`

Для порта PIO-USB: 
- D+ `GPIO0` 
- D- `GPIO1`

Переназначение выводов порта DEBUG: 
- В данной ревизии платы не используются

Соединение выводов MT8816 для Пентагон 128:

На порту Y выводы с Dx соединяются 1 к 1, как на изображении. 
Ремап сделан программно.

![Connection](https://github.com/OneginForte/ps2-to-zxkeyb-pio-Real/blob/Real/doc/conn.PNG)  

В папке Board размещена актуальная схема и герберы платы.

![Sheme](https://github.com/OneginForte/ps2-to-zxkeyb-pio-Real/blob/Real/doc/MT8816-keyb.png)  