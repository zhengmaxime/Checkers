# Jeu de dames, liste de tâches

## 3ème soutenance

### Répartition des tâches

|  | % Avancement prévu S2 | État | Qui ? |
| ------ | ------ | ----- | ---- |
| Menu | 80 % | OK | Mohamed, Rodrigue |
| Jouabilité | 80 % |avance sur planning| Maxime |
| Graphismes | 60 % |avance sur planning| Mohamed, Rodrigue |
| IA | 50 % |retard sur planning | Adrien, ... |
| Site Web | 80 % |OK| Mohamed

### Menu / Graphismes (Mohamed & Rodrigue)

##### Important

- [ ] Ajouter des boutons au menu principal comme le niveau de difficulté, ouvrir fichier (si c'est possible)
- [ ] Ajouter des boutons à côté du damier : undo, redo, save, quit, abandon
- [ ] Message de fin de partie dans la fenêtre de jeu

##### Moins important

- [ ] Utiliser Gtk pour ouvrir/sauvegarder une partie
(voir https://developer.gnome.org/gtk3/stable/GtkFileChooserDialog.html)
- [ ] Afficher des informations sur la partie dans la fenêtre de jeu (nombre de pièces restantes, le joueur qui doit jouer...)
(voir SDL_ttf)

##### Bonus
- [ ] Animations lorsqu'une pièce se déplace

### Jouabilité (Maxime)

- [x] Permettre au joueur d'abandonner une partie

##### Bonus

- [ ] Suggérer un coup au joueur humain s'il est en difficulté
- [ ] Gestion des fichiers PDN (voir https://en.wikipedia.org/wiki/Portable_Draughts_Notation)

### IA (Adrien, ...)

- [ ] Fonction d'évaluation
- [ ] Minimax optimisé

##### Bonus

- [ ] Élagage alpha-bêta pour améliorer Minimax
- [ ] Tables de finale

### Divers

- [ ] Rapport de projet (50 pages annexes non comprises)
- [ ] Site Web


##### 2ème soutenance
- [x] Undo/redo
- [x] Trace du coup effectué
- [x] Jouer en cliquant
- [x] Mode 1 joueur contre ordinateur
- [x] Trouver tous les coups possibles (si aucune prise possible)
- [x] Fin de partie lorsque plus aucun coup possible
- [x] IA (random)
- [x] Menu simple dans la console
- [x] Menu graphique
- [x] Rapport de soutenance

##### 1ère soutenance
- [x] Mode 2 joueurs

- [x] Affichage console
- [x] Affichage graphique (SDL)

- [x] Gestion d'erreur pour déplacement sans prise
- [x] Transformation en dame

- [x] Trouver les coups obligatoires (avec prise)
    - [x] Prise simple
    - [x] Rafle
- [x] Jouer une rafle
- [x] Vérification saisie joueur

- [x] Comptage des pièces
- [x] Abandon joueur
- [x] Fin de partie lorsque plus aucune pièce en jeu

- [x] Sauvegarde du plateau dans un fichier texte
- [x] Chargement d'un plateau depuis un fichier texte (problèmes)

- [x] Site Web
