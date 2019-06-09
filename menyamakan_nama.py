# Copyright:
# 
# Sudiro
#        [at] SudiroEEN@gmail.com
# available: 
#        on my github: 
#           github.com/sudiroeen

import os
import shutil
import cv2

folder = '../__datasetMata/'
files = os.listdir(folder)

files = [f for f in files if f.split('.')[-1] is not '*~']

for l in range(len(files)):
   oldName = folder + files[l]
   newName = "../datasetMata/" + 'mata_' + str(l) + '.jpg'
   print "oldName:\t" + oldName, "\tnewName: \t" + newName
   
   cv2.imwrite(newName, cv2.imread(oldName))
   os.remove(oldName)
