# Driver-LIS3DSH

Ce driver permet d'utiliser l'accéléromètre LIS3DSH de STMicroelectronics.

## Accéléromètre
C'est un capteur permettant de mesurer l'accélération linéaire de l'objet sur lequel il est fixé, dans les 3 axes de l'espace X, Y et Z.

## Schémas de câblage

## Paramètres
Toutes les informations de paramètrage du LIS3DSH sont disponibles sur https://www.st.com/resource/en/datasheet/lis3dsh.pdf

Pour ce driver, les paramètres modifiables sont :
```
power
- ON
- OFF

axe
- X et Y
- X uniquement
- Y uniquement

scale
- 2G
- 4G
- 6G
- 8G
- 16G
```

Le LIS3DSH communique avec le microcontroleur via la SPI1.
Les registres utilisés sont :
```
CTRL_REG4
CTRL_REG5
OUT_X_L
OUT_X_H
OUT_Y_L
OUT_Y_H
```


## Architecture logicielle
