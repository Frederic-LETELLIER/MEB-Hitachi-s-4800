#ifndef THREAD_PRINCIPAL_H
#define THREAD_PRINCIPAL_H

#include <QThread>
#include <QWidget>
#include <QtGui>

class TPG300;

class RenderThread : public QThread
{
    Q_OBJECT

public:
  void run();

  TPG300 *pt; // pointeur vers TPG300
  int etat;   // si etat=2 ==> erreur

  int fin_traitement;

  signals:
    void affichageHTML_main_thread(QString ligne, QString couleur);
    void affichage_main_thread(QString ligne);  // affichage sur l'ecran
    void affichage_choix_main_thread(int choix);  // affichage sur l'ecran



};

#endif // THREAD_PRINCIPAL_H
