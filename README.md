# fouille-reader

Microprogramme Arduino pour la lecture des carrés de fouille Age of Bots et le pilotage de LEDs.

## Communication I2C

### Lecture d'un carré de fouille

- requête : `F`
- réponse : 0-5 (en l'air, inconnu, jaune, violet, interdit)

### Changement de couleur du stock

- requête : `SRRGB?0` (`S` suivi de 6 `R|G|B|?|0`)
- réponse : 0

### Changement de couleur des ventouses

- requête : `VR0` (`V` suivi de 2 `R|G|B|?|0`)
- réponse : 0
