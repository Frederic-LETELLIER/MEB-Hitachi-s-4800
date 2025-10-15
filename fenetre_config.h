#ifndef Fenetre_Config_H
#define Fenetre_Config_H

#include "ui_fenetre_config.h"
#include <QWidget>
#include <QtGui>

class Fenetre_Config : public QDialog
{
    Q_OBJECT

public:
    Fenetre_Config(QWidget *parent = 0);

    Ui::Fenetre_Config ui; // interface graphique
  
    void sauver(void); // sauvegarder les resultats sur le disque dur
    void charger(void); // charger les resultats se trouvant sur le disque dur

    QString get_ip(); // renvoie l'adresse ip du serveur
    quint16 get_port(); // renvoie le port de connexion

private slots:
    void on_pushButton_clicked();  // sauver et fermer
  
};

#endif

