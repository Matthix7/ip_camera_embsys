## Compilation

### La caméra est-elle détectée ?

Connexion à la raspberry via usb ou ssh
passage en cession root
taper la ligne de commande ``modprobe bcm2835-v4l2``
vérifier la présence de la caméra : ``ls /dev/video0``

### Installation de V4L :

Mettre le dépot git sur le docker (ici dans root/buildroot-precompiled-2017.08/docs)
Taper la ligne de commande ``./autogen.sh``
Taper la ligne de commande ``./configure``
-> On voit que ``checking for gcc... gcc`` est le mauvais compilateur
-> On voit que ``checking for v4l2_open in -lv4l2... no`` nous dit que la librairie n'est pas reconnue
Nous sommes dans ``root@b2aabf6f7d42:~/buildroot-precompiled-2017.08/docs/v4l2grab-master# `` on lance la configuration ``./configure CC=../../output/host/usr/bin/arm-linux-gcc`` pour indiquer le bon compilateur. Mais l'option ``--host`` est nécessaire pour la cross compilation.

Pour la simplicité on ajoute au path ``export PATH=$PATH:/root/buildroot-precompiled-2017.08/output/host/bin/``
La commande finale est donc ``./configure --host=arm-linux``

Commenter la ligne ``#undef malloc`` dans config.h.in

Taper la ligne de commande ``make clean`` pour supprimer l'ancien début de configuration
Relancer le ``./configure --host=arm-linux``
Faire un make -> la compilation s'effectue sans accros.

### Compilation de serveur.c

arm-linux-gcc 
