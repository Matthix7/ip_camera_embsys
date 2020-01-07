# -*- coding: utf-8 -*-
"""
Created on Tue Jan  7 10:06:41 2020

@author: catam
"""
from tkinter import Label, Button, Frame, BOTH, Canvas, Tk
from PIL import Image, ImageTk  

class Interface(Frame):
    
    """Notre fenêtre principale.
    Tous les widgets sont stockés comme attributs de cette fenêtre."""
    
    def __init__(self, fenetre, connexion):
        
        Frame.__init__(self, fenetre)
        self.pack(expand = True, fill=BOTH)        
        
        self.connexion = connexion
#         Création de nos widgets
        self.message = Label(self, text="Commande de la caméra")
        self.message.pack(side = "top")
        
        self.aquisition_button = Button(self, text="Acquisition", padx=5, pady=15, 
                                command=self.aquisition_command, bg = 'red')
        self.aquisition_button.pack(side="bottom")
        
        self.load_button = Button(self, text="Load", padx=26, pady=15, 
                                command=self.chargerImage, bg = 'green')
        self.load_button.pack(side="bottom")
        
        self.dicimg = {}
        self.size = (320, 240)
        self.cadre = Canvas(self,width=self.size[0],height=self.size[1],bg="black")
        self.cadre.pack()
        
        
        
    def chargerImage(self): 
        img=Image.open("ENSTAVENGERS.png") 
        img = img.resize((self.size[0], self.size[1]))
        photo = ImageTk.PhotoImage(img) 
        self.dicimg['img1'] = photo        
        item = self.cadre.create_image(self.size[0]//2,self.size[1]//2,image=photo) 
        
    
    def aquisition_command(self):
        """Il y a eu un clic sur le bouton.        
        On envoie la demande au serveur."""
        
        msg_a_envoyer = "1"
        # Peut planter si vous tapez des caractères spéciaux
        msg_a_envoyer = msg_a_envoyer.encode()
        # On envoie le message    
        self.connexion.send(msg_a_envoyer)
        
        msg_recu = self.connexion.recv(1024)
        
        print(msg_recu.decode()) # Là encore, peut planter s'il y a des accents
        
        
        
if __name__ == "__main__":
    fenetre = Tk()
    fenetre.geometry("600x400+300+100")
    
    interface = Interface(fenetre, None)
    
    interface.mainloop()
    # interface.destroy()      
        
#            img = ImageTk.PhotoImage(file="ENSTAVENGERS.png")
#        label = Label(self, image=img)
#        label.pack()