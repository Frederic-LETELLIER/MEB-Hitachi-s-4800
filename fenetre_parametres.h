#ifndef PARAMETRES_H
#define PARAMETRES_H

#include "ui_fenetre_parametres.h"
#include <QWidget>
#include <QtGui>
#include <QtCore>
#include <QtNetwork>   // TPC/UDP
#include <QSerialPort> // RS232
#include <QFileDialog>

class PARAMETRES : public QDialog
{
    Q_OBJECT

public:
    PARAMETRES(QWidget *parent = 0);

    Ui::PARAMETRES ui; // interface graphique
  
    void sauver(void); // sauvegarder les resultats sur le disque dur
    void charger(void); // charger les resultats se trouvant sur le disque dur

    int connexion(QSerialPort *port); // permet de se connecter a la jauge 1
    QSerialPort *port_local; // port qui sert pour les echanges RS232

    int delai_attente(void); // transmet le delai d'attente


private slots:
    void on_pushButton_rs232_clicked();  // sauver et fermer
    void on_pushButton_fichier_clicked();  // selectionner le fichier avec les commandes

};

#endif

