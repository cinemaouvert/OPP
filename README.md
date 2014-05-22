#Intro

Open Projection Program (OPP) is free software for open cinema projection.

#Compilation
###### Pour compiler sous linux : 
   - Récuperer la dernière version du projet sur le git (ajout d'une option de compilation)
   - Installer vlc,   libvlccore5,    libvlccore-dev,     libvlc-dev,     g++
   - Installer la version 2.4.1 de qtCreator (ou n'importe quel version basée sur qt 4.8)
    - Sinon installer libqt4-core  et changer les options de compilation de qtcreator pour compiler a partir de qt4 et non qt5              
 
   - Importer le projet a qtCreator et compiler
   
   
   ###### Pour compiler sous OSX : 
   - Récupérer la dernière version du projet sur le git (ajout d'une option de compilation)
   - Installer vlc
   - Installer qtCreator depuis avec un compilateur en qt4
   - Une fois la première compilation effectuée, pour lancer le logiciel il faut prendre les dossiers plugins et libs de /Applications/VLC.app/content/MacOs/ pour les mettre dans /CHEMIN VERS LE DEBUG/opp.app/content/MacOs/

   
   ###### Pour compiler sous Windows : 
   - Récupérer la dernière version du projet sur le git (ajout d'une option de compilation)
   - Installer vlc
   - Installer qtCreator avec un compilateur en qt4  
            (http://fr.openclassrooms.com/forum/sujet/tuto-installer-qt-sous-windows#.U3sDDHV_vWQ)
   - Dans le fichier core.pri mettre le bon chemin vers VLC
   - (Si vous avez l'erreur : poll undefined, allez dans le dossier contenant les fichiers .h et modifier la ligne qui pose problème : le #define poll est situé 10 lignes trop bas il faut le remonter un peu)
   - Une fois la première compilation effectuée, pour lancer le logiciel il faut prendre les dossiers plugins et les fichiers libvlc.dll et libvlccore.dll  de votre dossier vlc pour les mettre dans /CHEMIN VERS LE DEBUG/
   
#License

This software is free software published under GPL license.

Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
