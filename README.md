# fouille-reader

Microprogramme Arduino pour la lecture des carrés de fouille Age of Bots et le pilotage de LEDs pour la visualisation du stock du robot.

La carte utilisé est l'interface Nano générique de 2022.

## Communication I2C

### Lecture d'un carré de fouille

- requête : `F`
- réponse : 0-4 
    - 0 : en l'air
    - 1 : inconnu
    - 2 : jaune
    - 3 : violet
    - 4 : interdit

### Changement de couleur du stock

- requête : `SRRGB?0` (`S` suivi de 6 `R|G|B|?|0`)
- réponse : 0

### Changement de couleur des ventouses

- requête : `VR0` (`V` suivi de 2 `R|G|B|?|0`)
- réponse : 0
