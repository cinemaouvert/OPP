#Intro

Open Projection Program (OPP) is free software for open cinema projection.

#Developer

##Mac OS X

Il faut d'abord avoir installé VLC sur votre machine. Ensuite, copiez les libraries dynamique qui se trouve dans le répertoire d'installation de VLC dans un chemin standard (/usr/local/lib ou /usr/lib par exemple). Cela permet à OPP de trouver les libraries dont il a besoin.

Une autre solution (à éviter tout de même) consiste à modifier la variable d'environnement DYLD_LIBRARY_PATH pour qu'elle contienne le chemin des libraries de VLC. Dans ce cas, il faut ajouter la ligne suivante dans un script shell qui est chargé automatiquement au démarrage d'une session (.profile ou .bash_profile la plupart du temps) :

  export DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:/Applications/VLC.app/Contents/MacOS/lib"

#Build

You can build the application with qmake or QtCreator. Refer to Qt documentation for all explanations on the topic.

##Create a .dmg for OSX deployment

Change directory to the location you build the application. Then run the following command :

    macdeployqt opp.app -dmg

It should output a file named opp.dmg into the buld directory. Then, you can copy and distribute it for an easier installation on Mac.

##Windows NSIS

We use NSIS to create the Windows installer of the OPP.

You could generate the final installer executable by using NSIS software and load the NSIS script located at <\opp-path>/windows/opp.nsis.

It will generate the final executable named OPPSetup.exe. You can distribute it to users for an easy installation on Windows platforms.

#License

This software is free software published under GPL license. View the LICENSE file for the full details.

Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>