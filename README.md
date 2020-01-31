### Examen 2020 de Linux Embarqué
Groupe Bouveron Matthieu, Courjaud Alexandre, Jegat Corentin, Musellec Yann 


## Compilation

Le readme associé se trouve [ici](compilation/readme.md).




## Guide utilisateur
Le readme associé se trouve [ici](src/README.md).

L'interface pour la visualisation du champ de vision de la caméra et la sauvegarde des images est codée sous *Python3* et utilise les librairies *Tkinter* et *Pillow*. L'utilisation de cette interface requiert donc l'installation de ces librairies:
```
sudo apt install python3-tk
sudo apt install python3-pil python3-pil.imagetk
```

## Utilisation de buildroot  
Dans ce projet, buildroot sert à activer les options BR2_PACKAGE_RPI_FIRMWARE_X et BR2_PACKAGE_LIBV4L qui servent respectivement à assurer le support caméra pour la carte et à interagir avec la caméra via l'API V4L. Ces étapes sont transparentes pour nous puisque nous téléchargeons une version précompilée du système d'exploitation pour gagner du temps.
Pour cela on utilise une image Docker : 

```
docker pull pblottiere/embsys-rpi3-buildroot-video
```

Pour créer un conteneur:

````
$ docker run -it pblottiere/embsys-rpi3-buildroot-video /bin/bash
docker# cd /root
docker# ls
buildroot-precompiled-2017.08.tar.gz
docker# tar zxvf buildroot-precompiled-2017.08.tar.gz
````



## Flashage de la carte  

Récupérer l'image sdcard.img 

```
sudo docker cp <dockerID>:/root/buildroot-precompiled-2017.08/output/images/sdcard.img .
```

Puis la mettre sur la carte sd avec la commande dd. Enfin ajouter ou remplacer les fichiers `start_x.elf`, `fixup_x.dat` et `config.txt` (présents dans le dossier buildImage [ici](buildImage)) sur la 1ère partition de la carte SD.
