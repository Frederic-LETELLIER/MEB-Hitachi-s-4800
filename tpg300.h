#ifndef TPG300_H
#define TPG300_H

#include <QWidget>
#include <QtGui>
#include <QtCore>
#include <QtNetwork>   // TPC+UDP
#include <QSerialPort> // RS232
#include <QThread>

#include "ui_tpg300.h" // interface graphique avec QMainWindow
#include "fenetre_parametres.h" // fenetre de configuration de la mesure
#include "thread_principal.h" // pour faire tourner les commandes en parallele

class TPG300 : public QMainWindow
{
  Q_OBJECT

// variables et fonctions
public:
  TPG300(QWidget *parent = 0); // constructeur

  Ui::TPG300 ui; // interface graphique principale

  PARAMETRES fenetre_parametres; // classe PARAMETRES qui permet de configurer des elements

  RenderThread thread; // le thread qui echange avec le MEB

  QSerialPort *port; // port qui sert pour les echanges RS232
  QString commande_rs232; // La commande RS232 envoyee par le MEB et recue par l'ordinateur
  QString commande_rs232_complet; // ajout des octets de verification
  QDateTime date; // la date jour et heure

  QList<QString> liste_commandes; // liste avec les commandes a envoyer au MEB
  QList<QString> liste_commandes_TCP; // liste avec les commandes a envoyer au MEB
  QString nom_fichier_log; // nom du fichier pour les logs
  QString commande_recue_avant; // nom du fichier pour les logs

  QTcpServer *serveurTcp; // Pointeur vers le serveur TCP

  void configure_rs232(void);   // configuration du RS232
  int receive_rs232(int nombre_a_lire); // reception de donnees par RS232
  void receive_rs232_tout(void); // reception de donnees par RS232
  int send_rs232_short(QString cmd_short); // envoi de donnees par RS232
  int send_rs232_long(int longueur, QString cmd_long); // envoi de donnees par RS232

  void affichageHTML(QString ligne, QString couleur);  // affichage sur l'ecran
  void affichage(QString ligne);  // affichage sur l'ecran
  void fermeture_port(void);  // fermeture de la connexion RS232

  void traitement_commandes(QString commande_a_executer); // traitement des commandes a executer

  void ecriture_logs(QString ligne_a_ecrire1); // gestion des logs



private slots:
  void on_pushButton_configuration_clicked(void);  // fenetre configuration
  void on_pushButton_lecture_fichier_clicked(void);  // lecture du fichier de commandes
  void on_pushButton_execution_clicked(void);  // execution des commandes
  void on_pushButton_tcp_clicked(void);  // execution du serveur tcp
  void on_pushButton_stop_clicked(void);  // Arret des envois de commandes
  void newconnectiontcp(void); // signal pour dire connexion au serveur


  signals:
  void affichageHTML_main_thread2(QString ligne, QString couleur);
  void affichage_main_thread2(QString ligne);  // affichage sur l'ecran
  void affichage_choix_main_thread2(int choix);  // affichage sur l'ecran

  public slots:
    void affichageHTML_thread(QString ligne, QString couleur);  // affichage sur l'ecran
    void affichage_thread(QString ligne);  // affichage sur l'ecran
    void affichage_choix_thread(int choix);  // affichage sur l'ecran

};

#endif
