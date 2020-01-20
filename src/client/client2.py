# -*- coding: utf-8 -*-
"""
Created on Tue Jan  7 09:31:12 2020

@author: Matthieu
SOCKETS : 
https://openclassrooms.com/fr/courses/235344-apprenez-a-programmer-en-python/234698-le-reseau
THREADS:
https://openclassrooms.com/fr/courses/235344-apprenez-a-programmer-en-python/2235545-la-programmation-parallele-avec-threading

"""

# 192.168.8.51

from time import time
from tkinter import *
from interface import Interface


if __name__ == "__main__":
    
    # Interface graphique
    fenetre = Tk()
    fenetre.geometry("600x500+300+100")
    interface = Interface(fenetre)
    interface.mainloop()
    
    
    print("Fermeture de la connexion")
    try:
        interface.connection.close()
    except:
        pass
    
    try:
        interface.destroy()
    except:
        pass


