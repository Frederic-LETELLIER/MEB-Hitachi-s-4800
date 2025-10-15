#qmake option
QT          += core
QT          += gui
QT          += widgets #permet de faire un include <widgets>
QT          += network #permet de faire un include <network> TCP/UDP
QT          += serialport #permet de faire un include <serialport> RS232

# version de QT 6.9.2

TARGET = "Hitachi_s-4800_rs232"  # Nom du fichier .exe
TEMPLATE = app

CONFIG += release
CONFIG += qt
CONFIG += thread   # ajoute l'option qui permet de faire tourner plusieurs threads en meme temps
CONFIG -= debug   # il n'est pas necessaire de creer un fichier exe pour le debug

# Nom des repertoires qui vont heberger les fichiers compiles
MOC_DIR      = moc
OBJECTS_DIR  = obj
DESTDIR      = release
UI_DIR       = ui

########################################

# Liste des fichiers composant le projet

FORMS       += tpg300.ui
FORMS       += fenetre_parametres.ui
#FORMS       = *.ui # permet de charger tous les fichiers ui du dossier

HEADERS     += tpg300.h
HEADERS     += fenetre_parametres.h
HEADERS     += thread_principal.h
#HEADERS     += *.h  # permet de charger tous les fichiers ui du dossier

SOURCES     += main.cpp
SOURCES     += tpg300.cpp
SOURCES     += send_receive.cpp
SOURCES     += fenetre_parametres.cpp
SOURCES     += affichage.cpp
SOURCES     += gestion_commandes.cpp
SOURCES     += serveurtcp.cpp
SOURCES     += stop.cpp
SOURCES     += thread_principal.cpp
#SOURCES     += *.cpp  # permet de charger tous les fichiers ui du dossier

