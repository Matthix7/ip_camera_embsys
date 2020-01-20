# -*- coding: utf-8 -*-
"""
Created on Mon Jan 20 15:38:54 2020

@author: catam
"""

infile = open("ENSTAVENGERS.png", 'rb')

bin_list = []
byte = infile.read(1)
while byte != b"":
    bin_list.append(byte)
    byte = infile.read(1)
infile.close()


outfile = open('Test_binary.png', 'wb')
for byte in bin_list:
    print(byte)
    outfile.write(byte)
outfile.close()