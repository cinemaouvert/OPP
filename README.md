1. INSTALLATION
===================================

1.1 Mac OS X
-----------------------------------

Il faut d'abord avoir installé VLC sur votre machine. Ensuite, copiez les libraries dynamique qui se trouve dans le répertoire d'installation de VLC dans un chemin standard (/usr/local/lib ou /usr/lib par exemple). Cela permet à OPP de trouver les libraries dont il a besoin.

Une autre solution (à éviter tout de même) consiste à modifier la variable d'environnement DYLD_LIBRARY_PATH pour qu'elle contienne le chemin des libraries de VLC. Dans ce cas, il faut ajouter la ligne suivante dans un script shell qui est chargé automatiquement au démarrage d'une session (.profile ou .bash_profile la plupart du temps) :

  export DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:/Applications/VLC.app/Contents/MacOS/lib"
