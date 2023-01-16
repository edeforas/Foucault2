# Foucault2
Foucault2 is an Amateur Telescope Making freeware to help with mirror design and polishing.

Its keep the polishing history and help you to improve your mirror using your past experience.
The surface computation follow the book "Construction du telescope d'amateur " de J. Texereau.

Do not hesitate to send remarks, comments, contributions, suggestions...

Windows binaries can be downloaded at: 
http://edeforas.free.fr
or
in the bin/ folder

//////////////////////////////////////////////
Evolution from P. Crubillé
The main point is to implement unmasked phtographic foucault.

This need before immage aspect to make several modifications.
- zones are no more defined by the bound of the cooder screen, but by the center of  the zone (Hm).
- List of Hm is associated with each measure set and is no more a direct Mirror attribute.
- Couder screen can still be defined but from the screen.
- the maximum number of zone is enlarged up to 64 (MaxZones constant in Mirror.h).
- I add some features, more significative is the "transverse error" graph but also "obstruction" and "edge mask" definition (this is very usefull testing plate Smith).
- The file format is completed with adequate format to store unmasked foucault measures.
- A "Foucault Unmasked Measure" entry is add in the "Tasks" menu.
  - "Mirror angle" is for the mirror axe in test. Really this is just a comment, it should be add in "Foucault Measure" too.
  - "Slit angle" allow to rotate the foucaults slit. Images are rotates before analyse.
  - "RGB channel" specify the color channel to use So you can for example use a green led and have some red light in the office.
    - Colors images are reduced to grey scale images (as they are store in memory).
    - options are available to select how to detect the mirror in the image. Only "3 points" is on the way. "images cropped" allow to pass this step..
    - "callibration image" is unused at this time, Not sure the position of the mirror is always exactly the same, but it can be usefull for future work.
    - To select the images, just enter (or browse) for the first image in the set and set the "number of images". (Images are loaded in memory, but this is rather fast)
    - "number format is here only if your images name format is not in this form.
    - "delta hx between images" is the foucault offset.
    - The analysis is in two steps:
      - "crop images" to found mirror center and radius. If nothing append, it can be because some file in the set does not exist". If center and radius are not close between images, either you lnow why, either there can be a detection error.
      - "compute null zones" attemp to found an internal and and external zone in each images. if it is not thee same, there is a crown...
      - Analyse is now done, jus "generate mmeasure" to add it in the mirror project history.
      - clickint in the array, the softare draw the image either original, cropped or diff/rotated depending the "Draw image as" menu.


The null zones detections is in two step:
    - found the mirror in the image:
      - first, the image is rotated, so it is close to be horzontaly symmetric.
      - following regularly spaced horizontals lines, for each, found the point 
 with the maximum gradient from black to color.
      - choose 3 or 4 "good" points and compute the intersection for the bisectrices of two segments.
      - check two horizontal lines symetric to the y_center that their x is very very close.

    - crop the image.
    
    - diff image with its 180 rotation. That is compute the axial difference of this image. So we get an image very close to a foucault with a wire.

    - compute the null zones in this images:
      for each point in the radius, build an ordered array:
      	  - for each angle value from -10 degres to +10 degres with a smmall step (for example 0,1 degre),
	    - put in the array the closer pixel value.
	    - forget 1/4 of the smaller values, and 1/4 the greatest.
	    - compute the average of the remainders (so half the values).
	  - set the result in an radius indexed array.
	  - in this array, found from the center and from the edge a zone with near 0 (black) values and get the average position of the internal and of the external zones.
	  
