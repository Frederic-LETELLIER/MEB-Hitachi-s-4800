#include "fenetre_config.h"


///////////////////////////////////////////
// Creation de l'objet
Fenetre_Config::Fenetre_Config(QWidget *parent) : QDialog(parent)
{
        ui.setupUi(this);

  if(QDir("config").exists() == 0)       // on regarde si le dossier config existe
  {
        QDir().mkdir("config");     // creation du dossier config
  }


  // on charge les donnees sur le disque dur
  charger();
}



///////////////////////////////////////////
// Fermer la fenetre et enregistrer
void Fenetre_Config::on_pushButton_clicked()
{
  // on ferme la fenetre
  this->hide();

  // on sauve les resultats sur le disque dur
  sauver();

} 



///////////////////////////////////////////
// Sauver les donnees sur le disque dur
void Fenetre_Config::sauver(void)
{
  // recuperation des donnees
  QString value1 = ui.lineEdit_1->text();
  QString value2 = ui.lineEdit_2->text();
  QString value3 = ui.lineEdit_3->text();
  QString value4 = ui.lineEdit_4->text();

  QFile file;
  file.setFileName("config/config.ini");
  if( file.exists() )
    file.remove();
  file.open(QIODevice::Append | QIODevice::Text );
  QTextStream out(&file);
  out << value1 << Qt::endl;   // endl permet le retour a la ligne
  out << value2 << Qt::endl;
  out << value3 << Qt::endl;
  out << value4;
  file.close();

} 


///////////////////////////////////////////
// Charger les donnees sur le disque dur
void Fenetre_Config::charger(void)
{
  QFile file;
  file.setFileName("config/config.ini");
  if( !file.exists() ) // on verifie l'existence du fichier
    return;      

  // creation des variables
  QString value1 = "192.168.140.87";
  QString value2 = "87";
  QString value3 = "50";
  QString value4 = "1";

  // on lit ligne après ligne le fichier
  file.open(QIODevice::ReadOnly | QIODevice::Text );
  QTextStream in(&file);
  QString line;
  line = in.readLine(); 
  value1 = line;
  line = in.readLine();
  value2 = line;
  line = in.readLine();
  value3 = line;
  line = in.readLine();
  value4 = line;
  file.close();

  // affichage
  ui.lineEdit_1->setText(value1);
  ui.lineEdit_2->setText(value2);
  ui.lineEdit_3->setText(value3);
  ui.lineEdit_4->setText(value4);
} 


///////////////////////////////////////////
// Donne le port
quint16 Fenetre_Config::get_port(void)
{
  quint16 port;
  QString port_string;

  port_string = ui.lineEdit_2->text();
  port = port_string.toUShort();

  return port;
}


///////////////////////////////////////////
// Donne l'adresse ip
QString Fenetre_Config::get_ip(void)
{
  QString ip;
  ip = ui.lineEdit_1->text();
  return ip;
}




