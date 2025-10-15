#include "thread_principal.h"

#include "tpg300.h"

void RenderThread::run()
{

  // Pour connaitre l'identifiant du Thread
  //qDebug() << "Current thread (renderthread):" << QThread::currentThread();


  etat = 0;
  QString commande_a_executer;

  // on lance une boucle infinie
  while(1)
  {

    // L etat 0 est un etat qui sert uniquement a l'attente
    // c'est pour eviter que le programme prenne trop de ressources sur l'ordinateur
    if(etat == 0)
    {
      QEventLoop evtLoop;
      QTimer::singleShot(500, &evtLoop, SLOT(quit())); // 500 ms d'attente
      evtLoop.exec();


      if(pt->ui.centralwidget->isVisible() == false) // on regarde si la fenetre principale est visible
      {
          qDebug("<debug> (RenderThread) Erreur, fenetre principale non visible");
          QApplication::quit();
      }


    }

    // L etat 1 est un etat qui sert pour le transfert des commandes
    else if(etat == 1)
    {

        //qDebug("<debug> (RenderThread) liste vide : %d", liste_commandes.isEmpty());
        while (!pt->liste_commandes.isEmpty())
        {

            // On recupere la commande a executer
            commande_a_executer = pt->liste_commandes[0];

            // on supprime la commande de la liste
            pt->liste_commandes.removeAt(0);

           qDebug("<debug> (RenderThread) Toto");

            // on transmet la commande pour traitement
            //pt->traitement_test(commande_a_executer);
            pt->traitement_commandes(commande_a_executer);

           //qDebug("<debug> (RenderThread) liste vide : %d", pt->liste_commandes.isEmpty());


            QEventLoop evtLoop;
            QTimer::singleShot(pt->fenetre_parametres.delai_attente(), &evtLoop, SLOT(quit())); // xxx ms d'attente
            evtLoop.exec();

        }

        // On lance une commande dans le main thread
        // Va lancer la fonction (affichage_choix_thread)
        emit affichage_choix_main_thread(1);

        etat = 0; // on a fini de traiter les commandes et on se remet donc en attente


    }


    else if(etat == 2)
    {
      QEventLoop evtLoop;
      QTimer::singleShot(100, &evtLoop, SLOT(quit())); // 100 ms d'attente
      evtLoop.exec();
    }


  } // fin du while(1)
}





