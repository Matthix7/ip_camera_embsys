# Les communications client/serveur pour le transfert d'image

## Client: caméra IP côté utilisateur
Pour que le programme fonctionne, il vous faudra au préalable installer les librairies suivantes: 
- tkinter
- pillow
- socket (probablement installée par défaut)  

Puis dans un terminal, exécutez dans le répertoire client: `python3 client.py` 
Une fenêtre TKinter devrait apparaître. Dans cette fenêtre vous pouvez demander une acquisition d'image côté serveur, qui transmet l'image au client (à condition que vous soyez bien dans le même réseau), l'enregistre dans le répertoire sous le nom "current_frame.jpeg" et l'affiche dans la fenêtre. Si un autre client est connecté lors de la demande, celle-ci n'aboutira pas et vous devrez attendre la déconnexion de l'autre.
N'oubliez pas de bien renseigner l'adresse IP du serveur dans la barre de texte en bas de la fenêtre.

### Fonctionnement
- Lorsqu'un click sur le bouton "Acquisition", TKinter lance la procédure de connexion au serveur à moins que la connexion n'existe déjà.
- Cette connexion se fait socket TCP. Si le serveur accepte la connexion (il faut être le seul client à cet instant), alors le client envoie une requête pour une nouvelle acquisition d'image.
- Une fois l'acquisition effectuée, le client reçoit tout d'abord un message qui lui indique la taille de l'image puis un ensemble de messages contenant tous les octets de l'image.
- Ces octets sont ré-assemblés localement pour former une image.

## Serveur

Le dossier serveur contient le code nécessaire à la compilation du serveur pour la raspberry PI.
Le serveur est capable d'accueillir un client à la fois, tant que celui-ci ne s'est pas déconnecté il est impossible pour un 2ème client d'y accéder. Le serveur se lance au démarrage de la raspberry PI sur le port 12800.

Lorsqu'un client est connecté, la LED branchée sur le pin 24 s'allume.
