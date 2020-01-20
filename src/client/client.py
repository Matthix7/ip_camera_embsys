# -*- coding: utf-8 -*-
"""
Created on Tue Jan  7 09:31:12 2020

@author: Matthieu
SOCKETS : 
https://openclassrooms.com/fr/courses/235344-apprenez-a-programmer-en-python/234698-le-reseau
THREADS:
https://openclassrooms.com/fr/courses/235344-apprenez-a-programmer-en-python/2235545-la-programmation-parallele-avec-threading

"""

import socket
from time import time
from tkinter import *
from interface import Interface


import sys

if __name__ == "__main__":
    
    if len(sys.argv) != 2:
        "Usage: python3 client.py 'server_ip'"
        
    else:
        hote = sys.argv[1]
        port = 12800
        
        connexion_avec_serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        connexion_avec_serveur.connect((hote, port))
        
        msg_recu = connexion_avec_serveur.recv(1024).decode()
        
        
        if msg_recu != "1":
            print("Le serveur a refusé la connexion: déjà occupé.")
        else:
            print("Connexion établie avec le serveur sur le port {}".format(port))
        
            # Interface graphique
            fenetre = Tk()
            fenetre.geometry("600x500+300+100")
            interface = Interface(fenetre, connexion_avec_serveur)
            interface.mainloop()
            
            
            print("Fermeture de la connexion")
            connexion_avec_serveur.close()
            
            try:
                interface.destroy()
            except:
                pass


