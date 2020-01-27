# -*- coding: utf-8 -*-
"""
Created on Tue Jan  7 10:06:41 2020

@author: catam
"""
from tkinter import Label, Button, Frame, BOTH, Canvas, Tk, StringVar, Entry
from PIL import Image, ImageTk  
import socket
from numpy.random import randint

class Interface(Frame):
    
    """Notre fenêtre principale.
    Tous les widgets sont stockés comme attributs de cette fenêtre."""
    
    def __init__(self, fenetre):
        
        Frame.__init__(self, fenetre)
        self.pack(expand = True, fill=BOTH)        
        
        self.connected = False        
        self.connection = None
        
        
#         Création de nos widgets
        self.message = Label(self, text="Commande de la caméra")
        self.message.pack(side = "top")
        
        
        # Input server ip address
        self.input_txt = StringVar() 
        self.ip_address = "Server IP address"
        self.input_txt.set("Server IP address")
        self.entree = Entry(fenetre, textvariable=self.input_txt , width=30)
        self.entree.pack(side = "top")
        
        
        
        self.aquisition_button = Button(self, text="Acquisition", padx=5, pady=15, 
                                command=self.aquisition_command, bg = 'red')
        self.aquisition_button.pack(side="bottom")
        
        self.load_button = Button(self, text="Advert", padx=26, pady=15, 
                                command=self.chargerImage, bg = 'green')
        self.load_button.pack(side="bottom")
        
        self.dicimg = {}
        self.size = (320, 240)
        self.cadre = Canvas(self,width=self.size[0],height=self.size[1],bg="black")
        self.cadre.pack()
        
        
        
    def chargerImage(self): 
        images = ["ENSTAVENGERS.png", "team.jpg", "montage.jpg"]
        index_image = randint(len(images))
        images = ["ENSTAVENGERS.png", "team.jpg", "montage.jpg"]
        img=Image.open("Ads/"+images[index_image]) 
        img = img.resize((self.size[0], self.size[1]))
        photo = ImageTk.PhotoImage(img) 
        self.dicimg['img1'] = photo        
        _ = self.cadre.create_image(self.size[0]//2,self.size[1]//2,image=photo) 
        
    
    
    def get_connection(self):
        self.ip_address = self.input_txt.get()
        if self.ip_address == "Server IP address":
            print("Please fill in the server IP address")
            return None
        
        elif self.connected:
            return self.connection
        
        else:
            hote = self.ip_address
            port = 12800
            
            connexion_avec_serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            connexion_avec_serveur.connect((hote, port))
            
            msg_recu = connexion_avec_serveur.recv(1024).decode()            
            
            if msg_recu != "1":
                print("Connection refused by the server: already in use.")
                return None
            else:
                print("Connection established on port {}".format(port))
                self.connection = connexion_avec_serveur
                self.connected = True
                return self.connection
            
    
    
    
    def aquisition_command(self):
        connection = self.get_connection()
        
        if connection is not None:
            demand = b'1' 
            connection.send(demand)    
            print("Sent acquisition command...")
            
            msg_recu = connection.recv(1).decode()
#            print("Conf demande: ", msg_recu)
            
            if msg_recu == "1": # Le serveur accepte le transfert
                outfile = open('current_frame.jpg', 'wb')
                print("Beginning transfert...")
                
                image_size_bytes = connection.recv(1024).split(b'$')[1]
#                print(image_size_bytes)
                image_size = eval(image_size_bytes)
                size_downloaded = 0
                
                print("Image size = ", image_size)
                confirmation = b'1'
                connection.send(confirmation)  
                
                while size_downloaded < image_size:
                    received = connection.recv(1024)
                    size_downloaded += len(received)
                    outfile.write(received)
                        
                        
                print("Transfert finished ! ", size_downloaded)
                outfile.close()
                
                
                img=Image.open("current_frame.jpg") 
                img = img.resize((self.size[0], self.size[1]))
                photo = ImageTk.PhotoImage(img) 
                self.dicimg['img1'] = photo        
                _ = self.cadre.create_image(self.size[0]//2,self.size[1]//2,image=photo)
                    
        
        
        
if __name__ == "__main__":
    fenetre = Tk()
    fenetre.geometry("600x400+300+100")
    
    interface = Interface(fenetre)
    
    interface.mainloop()