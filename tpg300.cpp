
#include "tpg300.h"
#include "send_receive.cpp"          // Fonctions pour envoyer et recevoir
#include "affichage.cpp"             // Fonctions pour afficher sur l'ecran
#include "gestion_commandes.cpp"     // Fonctions pour gerer les commandes
#include "serveurtcp.cpp"            // Fonctions pour gerer le serveur TCP
#include "stop.cpp"                  // Fonctions pour arreter les envois



TPG300::TPG300(QWidget *parent) : QMainWindow(parent)
{
  ui.setupUi(this); // chargement de l'interface graphique

  ui.pushButton_execution->setEnabled(false); // on desactive le bouton

  // initialisation des variables
  commande_rs232 = ""; // initialisation de la variable
  port = NULL; // initialisation du port serie

  // on vide les listes de commandes
  liste_commandes.clear();
  liste_commandes_TCP.clear();

  // creation du thread qui met a jour les valeurs
  thread.pt = this; // on donne le pointeur vers TPG300
  thread.etat = 0; // on initialise la valeur
  thread.start();  // on demarre le thread

  // Je fais une connexion entre les 2 thread
  //connect(&thread, &RenderThread::affichageHTML_main_thread, this, &TPG300::affichageHTML_thread,Qt::BlockingQueuedConnection);
  //connect(&thread, &RenderThread::affichage_main_thread, this, &TPG300::affichage_thread,Qt::BlockingQueuedConnection);
  connect(&thread, &RenderThread::affichage_choix_main_thread, this, &TPG300::affichage_choix_thread,Qt::BlockingQueuedConnection);

  connect(this, &TPG300::affichageHTML_main_thread2, this, &TPG300::affichageHTML_thread,Qt::BlockingQueuedConnection);
  connect(this, &TPG300::affichage_main_thread2, this, &TPG300::affichage_thread,Qt::BlockingQueuedConnection);


}







/////////////////////////////////////////////
// Execution des commandes
/////////////////////////////////////////////
void TPG300::on_pushButton_execution_clicked(void)
{

    // Pour connaitre l'identifiant du Thread
    qDebug() << "Current thread (tpg300):" << QThread::currentThread();

        // On vérifie qu'aucune commande est en cours d'exécution
  if( !liste_commandes_TCP.isEmpty() )
  {
    affichageHTML("Erreur : Commande TCP en cours d'exécutation. Attendre sa fin", "rouge");
    return;
  }
  if( thread.etat == 1 )
  {
    affichageHTML("Erreur : Commande en cours d'exécutation. Attendre sa fin", "rouge");
    return;
  }

  ui.pushButton_execution->setEnabled(false);  // on desactive le bouton

  // creation du fichier de log pour l'enregistrement
  QDateTime date = QDateTime::currentDateTime();
  nom_fichier_log = "./logs/log_MEB_" + date.toString( "yyyy-MM-dd_HH-mm-ss") + ".txt";

  // on regarde si le dossier logs existe
  if(QDir("logs").exists() == false)
  {
    QDir().mkdir("logs");
  }

  affichageHTML( "Début du traitement des commandes à exécuter" , "noir");

  thread.etat = 1; // on dit au thread d'executer les commandes

}


/////////////////////////////////////////////
// Lecture du fichier de commandes
/////////////////////////////////////////////
void TPG300::on_pushButton_lecture_fichier_clicked(void)
{
  // on efface l'ecran
  // ui.textBrowser->setText("");
  ui.textBrowser->clear();

  // on efface la liste des commandes
  liste_commandes.clear();

  // on affiche le nom du fichier
  affichageHTML("Fichier : " + fenetre_parametres.ui.lineEdit_2->text(), "noir");

  // on initialise la variable
  commande_recue_avant = "";


  // selection du fichier
  QFile file;
  file.setFileName(fenetre_parametres.ui.lineEdit_2->text());
  if( !file.exists() )
  {
    affichageHTML("Erreur : Fichier de commandes non trouvé", "rouge");
    affichageHTML("Cliquez sur le bouton Configuration pour mettre la bonne adresse vers le Fichier de commandes non trouvé", "rouge");
      return;
  }

  // ouverture du fichier en lecture
  file.open(QIODevice::ReadOnly | QIODevice::Text );

  // lecture de toutes les lignes
  affichageHTML("Affichage des commandes présentes dans le fichier : " ,"noir");
  QString ligne;
  int nb_lignes = 0;
  while (!file.atEnd())
  {
    ligne = file.readLine();
    ligne = ligne.remove("\n"); // suppression retour a la ligne
    ligne = ligne.remove(QChar('\x0D')); // suppression de <CR>
    ligne = ligne.remove(QChar('\x0A')); // suppression de <LF>
    affichageHTML(ligne,"bleu");

    // on remplit la liste des commandes
    // liste_commandes << ligne;
    liste_commandes.append(ligne);

    nb_lignes++;
  }

  //qDebug("<debug> nombre lignes : %d", nb_lignes);
  affichageHTML("Nombre de commandes : " + QString::number(nb_lignes) ,"noir");

  if(liste_commandes.count() == 0 )
  {
    affichageHTML("Erreur : Aucune commande trouvée", "rouge");
    return;
  }

  affichageHTML("","noir");

  file.close(); // fermeture du fichier

  // on active le bouton
  ui.pushButton_execution->setEnabled(true);

}


/////////////////////////////////////////////
// Ouverture de la fenetre de configuration
/////////////////////////////////////////////
void TPG300::on_pushButton_configuration_clicked(void)
{
  // on affiche la fenetre
  fenetre_parametres.show();
}


/////////////////////////////////////////////
// Serveur TCP
/////////////////////////////////////////////
void TPG300::on_pushButton_tcp_clicked(void)
{
  // creation du serveur TCP
  serveurTcp = new QTcpServer(this);

  bool testTcp;
  int port_tcp = fenetre_parametres.ui.lineEdit_3->text().toInt();
  affichageHTML(QString::number(port_tcp),"noir");

  testTcp = serveurTcp->listen(QHostAddress::Any, port_tcp);
  if(testTcp == false)  // erreur pour l'ecoute
  {
    // on defait serveurTcp
    delete serveurTcp;

    // on met le bouton en rouge
    QPalette palette;
    palette.setColor(QPalette::ButtonText, QColor(255,0,0)); // texte en rouge
    ui.pushButton_tcp->setPalette(palette);

    affichageHTML("Erreur : Pas de serveur TCP", "rouge");

    return;
  }


  // on desactive le bouton
  ui.pushButton_tcp->setEnabled(false);

  // on met le bouton en vert
  QPalette palette;
  palette.setColor(QPalette::ButtonText, QColor(0,100,0)); // texte en vert
  ui.pushButton_tcp->setPalette(palette);

  affichageHTML("Lancement du serveur TCP", "noir");

  // on ecoute les signaux qui indiquent une connexion au serveur
  connect(serveurTcp, SIGNAL(newConnection()), this, SLOT(newconnectiontcp()));

  return;
}


