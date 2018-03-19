#Intro

Open Projection Program (OPP) is free software for open cinema projection.

#Compilation
   ###### Pour compiler sous linux : 
   - Récuperer la dernière version du projet sur le git (ajout d'une option de compilation)
   - Installer vlc, libvlccore-dev, libvlc-dev, g++
   - Installer qt-everywhere-opensource-src-4.8.7
	  - https://download.qt.io/official_releases/qt/4.8/4.8.7/
	  - Tutoriel d'installation : https://doc.qt.io/archives/qt-4.8/install-x11.html
   - Installer la version 2.6.2 de qtCreator (ou n'importe quel version basée sur qt 4.8)
      - Sinon changer les options de compilation de qtcreator pour compiler a partir de qt4 et non qt5              
   - Importer le projet dans qtCreator et compiler
   
   
   ###### Pour compiler sous OSX : 
   - Récupérer la dernière version du projet sur le git (ajout d'une option de compilation)
   - Installer vlc
   - Installer qtCreator depuis avec un compilateur en qt4
   - Une fois la première compilation effectuée, pour lancer le logiciel il faut prendre les dossiers plugins et libs de /Applications/VLC.app/content/MacOs/ pour les mettre dans /CHEMIN VERS LE DEBUG/opp.app/content/MacOs/

   
   ###### Pour compiler sous Windows : 
   - Récupérer la dernière version du projet sur le git (ajout d'une option de compilation)
   - Installer vlc(https://www.videolan.org/vlc/index.fr.html)
   - Installer qtCreator avec un compilateur en qt4  
            (http://fr.openclassrooms.com/forum/sujet/tuto-installer-qt-sous-windows#.U3sDDHV_vWQ)
   - (Si vous avez l'erreur : poll undefined, allez dans le dossier contenant les fichiers .h et modifier la ligne qui pose problème : le #define poll est situé 10 lignes trop bas il faut le remonter un peu)
   - Une fois la première compilation effectuée, pour lancer le logiciel il faut prendre les dossiers plugins et les fichiers libvlc.dll et libvlccore.dll  qui trouve dans le fichier DLL.zip du projet, pour les mettre dans /CHEMIN VERS LE DEBUG/ ou /CHEMIN VERS LE RELEASE/ en fonction de si vous compiler en mode debug ou release.
   

      ###### Généreration et utilisation du fichier de traduction :
   La traduction est gérée par le module QtLinguist. Ce module permet de modifier le fichier "opp_fr.ts" qui contient la traduction du logiciel
   en français.
   - Pour compiler ce fichier il faut cliquer sur Qt : Outils > Externe > Linguist > Publier les traductions (lrelease) afin de générer le fichier
   "opp_fr.qm".
   - Il faut ensuite copier ce fichier "opp_fr.qm" dans le répértoire de compilation (release ou debug) à côté du fichier "opp.exe".

#License

This software is free software published under GPL license.

http://opp.cinemaouvert.fr/

Copyright (C) 2013 Catalogue Ouvert du Cinéma <dev@cinemaouvert.fr>
