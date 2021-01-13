## ASSIGNMENT LINUX ON RASPBERRY PI3B+

I tested the program on a raspberry pi 3B+. 

The starting condition is to have the Linux machine/virtual machine ready to build a kernel module with the command bitbake. 

If everything is correct, in the *poky* folder (located in the home directory), we should have, among other things, a folder named *meta-example*. 

Inside the latter, we should have another folder called *recipes-examples*. We have to copy/move  the two folder, nomi delle cartelle, inside recipies-examples. If we want to use the terminal, we can use the follwing command:



```bash
COMANDI TERMINALE DI COPIA

mv ...
mv ..
mv ..

```

In addition to that, we have to go in the following path "/home/yoctotrainee/poky/build_rpi3/conf" and open the file **bblayers.conf**. 



Inside the field BBLAYERS ?= "" we have to add the follwing line /home/yoctotrainee/poky/meta-example \ . Otherwise, we can simply substitute the bblayers.conf, with the one provided in the repo. 



In the same folder, namely ```/home/yoctotrainee/poky/build_rpi3/conf ```, we need to replace the file layer.conf with the one provided in the github repo. After this, we can simply build everything with the command ``` bitbake core-image-minimal ```. 



DEPLOY on the board  va testato



con rpi-imager funziona, ma bisogna farlo dal terminale. dd prova



Poi collega al router con cavo ethernet, find ip address, ssh, e poi run the program.

