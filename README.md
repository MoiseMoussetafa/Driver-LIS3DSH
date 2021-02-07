# Driver-LIS3DSH
Ce driver permet d'utiliser l'accéléromètre **LIS3DSH** de **STMicroelectronics**.  
Ce pilote a été conçu à l'aide de **STM32CubeIDE** sur la carte **STM32F407G-DISC1**.

Lien Git : https://github.com/MoiseMoussetafa/Driver-LIS3DSH

## Accéléromètre
C'est un capteur permettant de mesurer l'accélération linéaire de l'objet sur lequel il est fixé, dans les 3 axes de l'espace X, Y et Z.  
L'accélération linéaire permet alors de mesurer l'inclinaison en X et Y.

## SPI
Ce driver fonctionne grâce au bus SPI.  
Il y a donc 4 signaux importants pour cette communication :
- Signal d'horloge (SCK) qui cadence la communication
- Chip Select (CS) permettant d'activer ou non la communication
- MISO correspondant au signal entrant dans le microcontroleur (Master) et sortant du LIS3DSH (Slave)
- MOSI correspondant au signal sortant du microcontroleur (Master) et entrant dans le LIS3DSH (Slave)

Parmi les caractéristiques du fonctionnement de cette SPI :
- le 1e bit du signal MISO est à 1 lorsqu'une lecture est effectuée. Sinon c'est une écriture.
- CS est à l'état bas pour activer la communication. Sinon il est à l'état haut.

## Utilisation
Ce driver permet d'allumer les LEDS 3, 4, 5 et 6 en fonction de l'inclinaison de la carte et des paramètres choisis.

L'utilisation de ce driver se fait grâce aux 2 fichiers : `LIS3DSH.h` et `LISH3DSH.c`.

---
## Schémas de câblage
La pin PA5 correspond au signal d'horloge de la SPI entre le microcontroleur et le LIS3DSH.  
La pin PA7 correspond au signal MOSI de la SPI entre le microcontroleur et le LIS3DSH.  
La pin PA6 correspond au signal MISO de la SPI entre le microcontroleur et le LIS3DSH.  
La pin PE3 correspond au signal CS, permettant d'activer ou non la communication SPI entre le microcontroleur et le LIS3DSH.  
Les pins PE1 et PE2 sont spécifiques aux fonctions d'interruption. Elles ne sont pas utilisées sur ce driver.

Les pins PD12, 13, 14 et 15 correspondent à des GPIO du microcontroleur, permettant de controler l'allumage des LEDS correspondantes sur le circuit.

***Câblage du LIS3DSH :***

![MEMS](https://raw.githubusercontent.com/MoiseMoussetafa/Driver-LIS3DSH/main/docs/MEMS.png?token=ARJF43FEMKC5MA4LZPZKZODAFEIJC)

***Câblage des LEDS :***

![LEDS](https://raw.githubusercontent.com/MoiseMoussetafa/Driver-LIS3DSH/main/docs/LEDS.png?token=ARJF43EBSUYUKE75GQGX4LLAFEIKC)


## Paramètres
Toutes les informations de paramètrage du LIS3DSH sont disponibles sur https://www.st.com/resource/en/datasheet/lis3dsh.pdf

Pour ce driver, les paramètres modifiables sont :
```
power
- LIS3DSH_ON : Allumer le module
- LIS3DSH_OFF : Eteindre le module

axe
- LIS3DSH_XY : Activer l'accéléromètre sur les axes X et Y
- LIS3DSH_X : Activer l'accéléromètre sur l'axe X uniquement
- LIS3DSH_Y : Activer l'accéléromètre sur l'axe Y uniquement

scale
- LIS3DSH_SCALE_2G
- LIS3DSH_SCALE_4G
- LIS3DSH_SCALE_6G
- LIS3DSH_SCALE_8G
- LIS3DSH_SCALE_16G
```

Ces paramètres peuvent être changés directement dans le `LIS3DSH.c`, dans la fonction `LIS3DSH_Init_t`
```c
/* Init LIS3DSH */
LIS3DSH_Status LIS3DSH_Init_t(SPI_HandleTypeDef *hspi,
		LIS3DSH_Init *posInitDef)
{
	uint8_t spiData[2] = {0x00, 0x00};	//Data to send to LIS3DSH
	uint8_t spiCheckData[2] = {0x00, 0x00};	//Data to read from LISH3DSH

	//Config REG4
	spiData[0] |= (posInitDef->power | LIS3DSH_ON);	//<---
	spiData[0] |= (posInitDef->axe | LIS3DSH_XY);	//<---

	//Config REG5
	spiData[1] |= (posInitDef->scale | LIS3DSH_SCALE_4G);	//<---
  ...
``` 

Les registres utilisés sont :
```
CTRL_REG4
CTRL_REG5
OUT_X_L
OUT_X_H
OUT_Y_L
OUT_Y_H
```

Le LIS3DSH communique avec le microcontroleur via la SPI1. La configuration de la communication est simplifiée grâce à STM32CubeIDE, à l'aide du fichier .ioc.

Dans `Pinout & Configuration`, on sélectionne `SPI1`, puis : 

Dans **Mode** :

- Mode : Full-Duplex Master  
*Le microcontroleur et au LIS3DSH communiquent en même temps. Le microcontroleur est maitre et le LIS3DSH est esclave.*

Dans **Configuration**, **Parameter Settings** :

- Data Size : 8 Bits
- First Bit : MSB First
- Prescaler (for Baud Rate) : 32
- Clock Polarity (CPOL) : High
- Clock Phase (CPHA) : 2 Edge

*La taille de la donnée communiquée par la SPI1 est en 8 bits. Le 1e bit de donnée est celui de poids fort.*  
*La polarité du signal d'horloge correspond à son état de repos. Pour ce driver, c'est l'état haut.*  
*La phase du signal d'horloge correspond au front de l'horloge utilisé pour la lecture de la valeur d'un bit sur le signal MISO/MOSI. Pour ce driver, c'est le 2e front.*

*Toutes les autres informations sont laissées dans leurs états par défaut.*

Dans `Clock Configuration`, on modifie la valeur de **HCLK (MHz)** à **64**. On obtient alors un Baud Rate de communication de 1 MBits/s (avec le Prescaler précédémment configuré).

## Architecture logicielle

### Diagramme des intéractions
Ce diagramme présente les intéractions entre l'utilisateur, le microcontroleur et le LIS3DSH, après alimentation du système.

![Diagramme interactions](https://raw.githubusercontent.com/MoiseMoussetafa/Driver-LIS3DSH/main/docs/Diagramme_interactions.png?token=ARJF43AIQRMGPNFKQMZHGCLAFEIJ6)

Le microcontroleur et le LIS3DSH se regroupent sur un même objet, en l'occurrence la carte électronique. Ils communiquent constamment entre eux et lorsque l'utilisateur effectue un mouvement de la carte, l'inclinaison est détecté par l'accéléromètre qui retourne une réponse spécifique, provoquant l'allumage des LEDS correspondantes par le microcontroleur.
Ce diagramme présente le cas où l'utilisateur effectue une inclinaison puis une remise à plat. Il est possible d'enchainer les inclinaisons sans faire une remise à plat, les LEDS s'allumeront en conséquence.

### Diagramme de flux
Ce diagramme présente schématiquement l'organisation générale du déroulement du driver.

![Organigramme](https://raw.githubusercontent.com/MoiseMoussetafa/Driver-LIS3DSH/main/docs/Organigramme.png?token=ARJF43AKSB42NCLHBWHAHU3AFEIL2)

Le driver démarre par une initialisation du système. Les données issues de l'accéléromètre sont ensuite récupérées.  
S'il n'y a pas d'inclinaison détéctées, les LEDS restent éteintes et on analyse les données suivantes.  
Dans le cas contraire, on identifie sur l'inclinaison est positive ou non sur l'axe X et la LED correspondante est allumée, puis de même sur l'axe Y.

## Acquisitions à l'oscilloscope
![1CLK2MISO](https://raw.githubusercontent.com/MoiseMoussetafa/Driver-LIS3DSH/main/docs/1CLK_2MISO.png?token=ARJF43DETAQAV55LXVUUG3DAFEIJQ)  
Le signal bleu correspond au signal d'horloge.  
Le signal rouge correspond au signal MISO, la donnée est 1101 0000, soit 0xD0.

![1CLK2MOSI](https://raw.githubusercontent.com/MoiseMoussetafa/Driver-LIS3DSH/main/docs/1CLK_2MOSI.png?token=ARJF43HUIMS7OULXEHMNZYTAFEIJU)  
Le signal bleu correspond au signal d'horloge.  
Le signal rouge correspond au signal MOSI, la donnée est 1010 1010, soit 0xAA.  
Le 1e bit est à 1, signifiant une lecture de registre. Le registre lu est donc 0010 1010, soit 0x2A. Cela correspond à OUT_Y_L (la donnée basse sur l'axe Y).

![1CS2CLK](https://raw.githubusercontent.com/MoiseMoussetafa/Driver-LIS3DSH/main/docs/1CS_2CLK.png?token=ARJF43EGS4Y5ULLRN46OSYDAFEIJY)  
Le signal bleu correspond au signal Chip Select, correspondant à l'activation de la SPI concernée.  
Le signal rouge correspond au signal d'horloge.  
Lorsque CS est à l'état bas, la SPI est activée et le signal d'horloge est envoyé. A l'inverse il n'y a pas de signal d'horloge lorsque CS est à l'état haut, désactivant la SPI.

![1CS2MISO](https://raw.githubusercontent.com/MoiseMoussetafa/Driver-LIS3DSH/main/docs/1CS_2MISO.png?token=ARJF43GS7YFHDGXAWYY5EHTAFEIJ2)  
Le signal bleu correspond au signal Chip Select, correspondant à l'activation de la SPI concernée.  
Le signal rouge correspond au signal MISO. Il y a plusieurs données sur la trame.  
Les données ne circulent que lorsque CS est à l'état bas puisque c'est dans ce cas que la SPI est activée.


## Acquisition avec analyseur logique
![logic](https://raw.githubusercontent.com/MoiseMoussetafa/Driver-LIS3DSH/main/docs/logic.png?token=ARJF43AWZ76X4KPYMPI2T6LAFFFRM)  
Lorsque CS est à l'état bas, CLK est enclenchée.
Sur cette trame, il y a 2 demandes de lecture sur les registres 0x29 et 0x2A, c'est-à-dire la donnée haute sur l'axe X et la donnée basse sur l'axe Y (le 1e bit étant à 1 pour la lecture, on retrouve respectivement 0xA9 et 0xAA).  
Le capteur répond ensuite avec les données 0xFA et 0x28 pour ces 2 registres. Le capteur est à plat, les données récupérées correspondent bien à la marge de valeurs pour le capteur à plat.


---
## Conclusion
L'étude du fonctionnement de la SPI est essentiel pour programmer le driver de cet accéléromètre avec la STM32.  
L'utilisation de STM32CubeIDE facilite grandement les configurations puisqu'il n'est pas nécessaire d'écrire les lignes de code, il suffit de choisir ce qu'on veut et le code est généré automatiquement. Par la suite les librairies sont très pratiques pour coder. Avoir une vision d'ensemble avec des définitions explicites permet de ne pas se perdre avec tout ce qu'on a à notre disposition.    
Finalement, le driver est fonctionnel.
