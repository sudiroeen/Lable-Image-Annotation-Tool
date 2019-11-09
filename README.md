# Lable-Image-Annotation-Tool

- Can used to annotate image to semantic segmentation purpose

+ Video: https://www.youtube.com/watch?v=JBdBkea8DZI

# How To Use:
1. Compiling:
  - [g++ annotation_corner.cpp -o annotation_corner -std=c++11 -I /usr/local/include/opencv2 -L /usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs]

2. Running:
  - [./annotation_corner -name_img=hand.jpg -name_yaml=hand.yaml]

3. To annotate the object, left click mouse
4. To undo annotate, right click mouse
5. To save annotation, klik [s] on keyboard
6. To finish annotation process, click [ESC] on keyboard


# How To Contribute:
  Just send pull request
