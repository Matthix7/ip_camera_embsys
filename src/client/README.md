# Client: How to use the ip camera from the user's point of vue

For the program to start, you will beforehand need to install the following libraries:
- tkinter
- pillow
- socket (probably installed by default)

Then, in a linux terminal, under client repository: `python3 client.py`  
A TKinter window should pop up. In this window you can ask for an image acquisition on the server side, that will transmit the image to you (provided you are in the same network), save it in the repository under name "current_frame.jpeg" and display it in the window. Do not forget to provide the IP adress in the text bar at the bottom of the window, else this will not work.


## How it works
- When a click on the "Acquisition" button is done, TKinter launches the connection procedure if the connection does not exist yet.
- This connection is done using TCP sockets. If the server accepts it (meaning you are the only client at this instant), then the client sends a request for a new image acquisition.
- Once the image acquisition has been done, the client first receives a message that indicates the size of the image and then a second message containing all the bytes of the image (fits in one buffer).
- These bytes are re-assembled as an image locally.
  
