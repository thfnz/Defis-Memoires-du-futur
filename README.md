# Défis d'IPhy - Mémoires du futur

Ce programme permet d'écrire et de lire une ligne complète de la carte mémoire.

## Changement de l'état d'un pin - Manipulation directe des ports de la carte Arduino
Afin de gagner en efficacité et en temps de calcul, ce programme n'utilise pas les fonctions de base pour changer l'état des pins de la carte (digitalRead et digitalWrite) mais manipule directement ses ports.

### Ports et registres
Les microcontrolleurs intégrés aux cartes arduinos sont composés de 3 ports :
- le port **D** est responsable des **pins numériques 0 à 7**;
- le port **B** est responsable des **pins numériques 8 à 13**;
- le port **B** est responsable des **pins analogiques**.

Chaque port est lui-même composé de 3 registres à décalages (concrètement 3 variables bianaires appelées dans le code).
- Le registre **PORT*** permet le changement de l'état d'un pin;
- Le registre **PIN*** permet la lecture de l'état d'un pin.

#### Exemple
Si les pins numériques **1** et **3** sont à l'état haut et les pins **0**, **2**, **4**, **5**, **6** et **7** sont à l'état bas, on aura :
```
PIND = 0101 0000 = PORTD
```
Attention à l'ordre de lecture ! Le bit de poids faible code ici l'état du pin 7 et non celui du pin 0.

Pour plus d'informations, cf https://docs.arduino.cc/retired/hacking/software/PortManipulation/.


### Passage à l'état haut
Il est donc possible de passer l'état d'un pin de l'état bas à l'état haut en appliquant l'opération arithmétique binaire **OU inclusif** à la variable **PORT***.

#### Exemple
On reprend l'exemple précédent. On souhaite passer le pin 5 à l'état haut.

```
    0101 0000 
OU  0000 0100
=   0101 0100
```

### Passage à l'état bas
De la même façon, il est possible de passer l'état d'un pin de l'état haut à l'état bas en appliquant l'opération arithmétique binaire **ET NON** à la variable **PORT***.

#### Exemple
On reprend l'exemple précédent. On souhaite passer le pin 5 à l'état bas.

```
        0101 0100 
ET NON  0000 0100 
=       0101 0000
```
### Lecture d'un pin
Pour lire l'état d'un pin, il suffit d'isoler sa valeur en appliquant l'opération arithmétique binaire **ET** à la variable **PIN*** puis d'interpréter le résultat (nul ou non).

#### Exemple
Lecture du pin 1.

```
    0101 0000 
ET  0100 0000
=   0100 0000
=   64
=/= 0
```

### Implémentation
Ainsi, pour lire ou écrire l'état d'un pin, le programe appelle les fonctions **pON** (passage à l'état haut), **pOFF** (passage à l'état bas) et **lect** définies en entête du code.

