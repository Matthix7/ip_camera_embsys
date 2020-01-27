## Serveur

Le dossier serveur contient le code necessaire à la compilation du serveur pour la raspberry PI.
Le serveur est capable d'accueillir un client à la fois, tant que celui-ci ne s'est pas déconnecter il est impossible pour un 2ème client d'y accéder. Le serveur se lance au démarrage de la raspberry PI sur le port 128000.

Lorsqu'un client est connecté, la LED branché sur le pin 24 s'allume.
