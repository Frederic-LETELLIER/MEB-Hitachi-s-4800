#include "fenetre_parametres.h"


///////////////////////////////////////////
// Creation de l'objet
PARAMETRES::PARAMETRES(QWidget *parent) : QDialog(parent)
{
        ui.setupUi(this);

        // on regarde si le dossier config existe
        if(QDir("config").exists() == false)
        {
            qDebug("<debug> (PARAMETRES) Creation du dossier config");
            QDir().mkdir("config"); // creation du dossier config
        }

        // on regarde si le dossier scripts existe
        if(QDir("scripts").exists() == false)
        {
            qDebug("<debug> (PARAMETRES) Creation du dossier scripts");
            QDir().mkdir("scripts"); // creation du dossier scripts
        }

        // on regarde si le dossier logs existe
        if(QDir("logs").exists() == false)
        {
            qDebug("<debug> (PARAMETRES) Creation du dossier logs");
            QDir().mkdir("logs");  // creation du dossier logs
        }


  // on charge les donnees sur le disque dur
  charger();

  return;

}



///////////////////////////////////////////
// Fermer la fenetre et enregistrer
void PARAMETRES::on_pushButton_rs232_clicked()
{

  // on ferme/cache la fenetre
  this->hide();

  // on sauve les resultats sur le disque dur
  sauver();


  return;
}



///////////////////////////////////////////
// Sauver les donnees sur le disque dur
void PARAMETRES::sauver(void)
{


    // recuperation des donnees
  QString value1 = ui.lineEdit_1->text();
  QString value2 = ui.lineEdit_2->text();
  QString value3 = ui.lineEdit_3->text();
  QString value4 = ui.lineEdit_4->text();

  QFile file;
  file.setFileName("config/config.ini");

  // si le fichier config.ini existe deja, on supprime le fichier
  if( file.exists() )
    file.remove();

  file.open(QIODevice::Append | QIODevice::Text );
  QTextStream out(&file);
  out << value1 << Qt::endl;
  out << value2 << Qt::endl;
  out << value3 << Qt::endl;
  out << value4;
  file.close();


  return;

}


///////////////////////////////////////////
// Charger les donnees sur le disque dur
void PARAMETRES::charger(void)
{

  // on regarde si un fichier config.ini existe
  // si le fichier n'existe pas, on sort de cette fonction
  QFile file;
  file.setFileName("config/config.ini");
  if( !file.exists() )
    return;      

  // creation des variables par defaut
  QString value1 = "COM1"; 
  QString value2 = "fichier.txt";
  QString value3 = "57000";
  QString value4 = "4000";

  file.open(QIODevice::ReadOnly | QIODevice::Text );
  QTextStream in(&file);
  QString line;
  line = in.readLine(); 
  if (line != "")
    value1 = line;
  line = in.readLine();
  if (line != "")
    value2 = line;
  line = in.readLine();
  if (line != "")
    value3 = line;
  line = in.readLine();
  if (line != "")
    value4 = line;
  file.close();

  // affichage des valeurs dans les lineEdit
  ui.lineEdit_1->setText(value1);
  ui.lineEdit_2->setText(value2);
  ui.lineEdit_3->setText(value3);
  ui.lineEdit_4->setText(value4);


  return;
}


///////////////////////////////////////////
// Connexion a l'appareil
int PARAMETRES::connexion(QSerialPort *port)
{

  int retour = 0; // 0 = tout va bien

  // recuperation des donnees du port RS232
  QString nomport;
  nomport = ui.lineEdit_1->text(); // Numero port COM

  // si la connexion existe deja, on la ferme
  if(port != NULL)
  {
       port->close(); // on ferme le port
       delete port;
       port = NULL;
  }

  // connexion
  if(port == NULL)
  {
       port = new QSerialPort(nomport);
  }

   port->open(QIODevice::ReadWrite);
   //qDebug("<debug> (connexion) etat ouverture port : %d", port->isOpen());

  // on regarde si le port est bien ouvert
  if(port->isOpen() != true) // si port n'est pas ouvert
  {
    qDebug("<debug> (connexion) Erreur ouverture du port");
    port->close();
    delete port;
    port = NULL;

    retour = -1;
    return retour; // on renvoie -1 pour signaler le probleme
  }

  // Configuration du port RS232
  port->setBaudRate(QSerialPort::Baud4800); //Vitesse de transfert = 4800 bauds
  port->setDataBits(QSerialPort::Data8); // longueur des données = 8 bits
  port->setParity(QSerialPort::NoParity);  // contrôle de parité = aucune
  port->setStopBits(QSerialPort::OneStop); // Stop bits = 1 bit
  port->setFlowControl(QSerialPort::NoFlowControl); // pas de controle du flux

  port_local = port; // on fait une copie de l'adresse du pointeur

  return retour;

}

///////////////////////////////////////////
// Selection du fichier avec les commandes
void PARAMETRES::on_pushButton_fichier_clicked()
{
  QString nom_ficher = "";
  nom_ficher = QFileDialog::getOpenFileName(this,"Choisir le fichier avec les commandes pour le MEB", "scripts", "Fichier texte (*.txt)");
  if(nom_ficher != "")
    ui.lineEdit_2->setText(nom_ficher);
}



///////////////////////////////////////////
// Envoi du delai d'attente
int PARAMETRES::delai_attente(void)
{

    QString delai_ms_txt = ui.lineEdit_4->text(); // on recupere le texte
    int delai_ms = delai_ms_txt.toInt();          // conversion du texte en un nombre entier int

    return delai_ms;    // renvoi du nombre

}






