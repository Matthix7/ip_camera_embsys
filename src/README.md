## Serveur

Le dossier serveur contient le code necessaire à la compilation du serveur pour la raspberry PI.
Le serveur est capable d'accueillir un client à la fois, tant que celui-ci ne s'est pas déconnecter il est impossible pour un 2ème client d'y accéder. Le serveur se lance au démarrage de la raspberry PI sur le port 128000.

Lorsqu'un client est connecté, la LED branché sur le pin GPIO 24 s'allume.
Lorsqu'une photo est en cours d'acquisition et de transfert, la LED branché sur le pin GPIO 23 s'allume.

Nous n'avons pas réussi à créer une librairie partagé pour V4L donc nous réalisons un appel système pour la prise de photo pour que le programme fonctionne tout de même. 
