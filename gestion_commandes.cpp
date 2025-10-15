



//////////////////////////////////////////////////////////
/// Fonction qui gere les commandes a executer
///
void TPG300::traitement_commandes(QString commande_a_executer)
{

    // Pour connaitre l'identifiant du Thread
    qDebug() << "Current thread (traitement_commandes):" << QThread::currentThread();

      /*
      // Creation d'un fichier qui sert pour le debuggage
      QFile file_controle;
      file_controle.setFileName("./logs/log_controle.txt");
      if( file_controle.exists() )
         file_controle.remove();
      file_controle.open(QIODevice::Append | QIODevice::Text );
      QTextStream out_control(&file_controle);
      out_control << "Debut fonction traitement_commandes()" << Qt::endl;
      out_control << commande_a_executer << Qt::endl;
      */

      emit affichageHTML_main_thread2("------------------------", "noir");
      emit affichageHTML_main_thread2("Commande à éxécuter : " + commande_a_executer, "noir");


      if(commande_a_executer.contains("MQHSa"))
          emit affichageHTML_main_thread2("(High voltage is turned ON)", "noir");
      else if(commande_a_executer.contains("MQHSb"))
          emit affichageHTML_main_thread2("(High voltage is turned OFF)", "noir");
      else if(commande_a_executer.contains("MQHSc"))
          emit affichageHTML_main_thread2("(AUTO FOCUS is carried out)", "noir");
      else if(commande_a_executer.contains("MQHSi"))
          emit affichageHTML_main_thread2("(Direct photo is carried out)", "noir");
      else if(commande_a_executer.contains("MQHSk"))
        emit affichageHTML_main_thread2("(Scan is run)", "noir");
      else if(commande_a_executer.contains("MQHSr"))
          emit affichageHTML_main_thread2("(Display image is saved in fixed file name)", "noir");
      else if(commande_a_executer.contains("MQHSt"))
          emit affichageHTML_main_thread2("(Display image is saved in fixed file name)", "noir");
      else if(commande_a_executer.contains("MQHZb"))
          emit affichageHTML_main_thread2("(Emission current is set)", "noir");
      else if(commande_a_executer.contains("MQHc"))
          emit affichageHTML_main_thread2("(Stop stage motion)", "noir");
      else if(commande_a_executer.contains("MPHa"))
          emit affichageHTML_main_thread2("(Stage XYR position is returned)", "noir");
      else if(commande_a_executer.contains("MPHb"))
          emit affichageHTML_main_thread2("(Stage registration position is returned)", "noir");
      else if(commande_a_executer.contains("MPHc"))
          emit affichageHTML_main_thread2("(Stage R position is returned)", "noir");
      else if(commande_a_executer.contains("MPHd"))
          emit affichageHTML_main_thread2("(Stage XYZTR position is returned)", "noir");
      else if(commande_a_executer.contains("MPHZA"))
          emit affichageHTML_main_thread2("(Return acceleration voltage)", "noir");
      else if(commande_a_executer.contains("MPHZB"))
          emit affichageHTML_main_thread2("(Emission current is returned)", "noir");
      else if(commande_a_executer.contains("MPHZC"))
          emit affichageHTML_main_thread2("(Emission current is returned)", "noir");
      else if(commande_a_executer.contains("MPHZD"))
          emit affichageHTML_main_thread2("(WD is returned)", "noir");
      else if(commande_a_executer.contains("MPHZE"))
          emit affichageHTML_main_thread2("(Condenser lens notch is returned)", "noir");
      else if(commande_a_executer.contains("MPHZF"))
          emit affichageHTML_main_thread2("(Magnification is returned)", "noir");
      else if(commande_a_executer.contains("MPHZH"))
          emit affichageHTML_main_thread2("(Stigma current is returned)", "noir");
      else if(commande_a_executer.contains("MPHZI"))
          emit affichageHTML_main_thread2("(Beam alignment current is returned)", "noir");
      else if(commande_a_executer.contains("MPHZP"))
          emit affichageHTML_main_thread2("(Aperture alignment current is returned)", "noir");
      else if(commande_a_executer.contains("MPHZQ"))
          emit affichageHTML_main_thread2("(Stigma alignment X current is returned)", "noir");
      else if(commande_a_executer.contains("MPHZR"))
          emit affichageHTML_main_thread2("(Stigma alignment Y current is returned)", "noir");
      else if(commande_a_executer.contains("MPHZS"))
          emit affichageHTML_main_thread2("(Low MAG alignment current is returned)", "noir");
      else if(commande_a_executer.contains("MPHZV"))
          emit affichageHTML_main_thread2("(Objective lens current coarse is returned)", "noir");
      else if(commande_a_executer.contains("MPHZW"))
          emit affichageHTML_main_thread2("(Objective lens current fine is returned)", "noir");
      else if(commande_a_executer.contains("MPHZZ"))
          emit affichageHTML_main_thread2("(Equipment condition data returned together)", "noir");
      else if(commande_a_executer.contains("MPHSc"))
          emit affichageHTML_main_thread2("(Scanning speed is set to SLOW 1)", "noir");
      else if(commande_a_executer.contains("MPHSe"))
          emit affichageHTML_main_thread2("(Scanning speed is set to SLOW 3)", "noir");
      else if(commande_a_executer.contains("MPHZD"))
          emit affichageHTML_main_thread2("(Working distance is returned)", "noir");







      ecriture_logs("------------------------");
      ecriture_logs("Commande à éxécuter : " + commande_a_executer);
      // qDebug("<debug> (traitement_commandes) Commande : %s", commande_a_executer);
      qDebug() << "<debug> (traitement_commandes) Commande : " << commande_a_executer;



      if(commande_a_executer == "")
      {
          emit affichageHTML_main_thread2("Erreur commande vide", "rouge");
          ecriture_logs("Erreur commande vide");
          return;
      }


      // commande speciale
      // on va se contenter d'attendre un certain delai en ms
      if(commande_a_executer.contains("WAIT"))
      {
          commande_a_executer.remove("WAIT");
          int attente_secondes = commande_a_executer.toInt() * 1000;
          emit affichageHTML_main_thread2("ATTENTE", "noir");
          ecriture_logs("ATTENTE");
          QEventLoop evtLoop;
          QTimer::singleShot(attente_secondes, &evtLoop, SLOT(quit())); // Temps d'attente
          evtLoop.exec();
          return;
      }


      // out_control << "01" << Qt::endl;


      int erreur_portrs232=-1;
      // on configure le port de communication
      erreur_portrs232 = fenetre_parametres.connexion(port); // permet de se connecter a l'appareil
      //out_control << "01 a" << Qt::endl;
      port = fenetre_parametres.port_local;
      //out_control << "01 b" << Qt::endl;
      //    qDebug("<debug> erreur rs232 : %d", erreur_portrs232);

      if(erreur_portrs232 == -1)
      {
          //out_control << "erreur_ports232 == -1" << Qt::endl;
          qDebug("<debug> (1]) (traitement_commandes) Erreur ouverture port COM");
          ecriture_logs("Erreur ouverture port COM");
          emit affichageHTML_main_thread2("Erreur ouverture port COM", "rouge");
          emit affichageHTML_main_thread2("Cliquez sur le bouton Configuration pour changer le port COM", "rouge");
          emit affichageHTML_main_thread2("Obligation de quitter le programme", "rouge");

          //out_control << "01 c" << Qt::endl;
          // fermeture_port(); // le port n'est pas ouvert et ne doit donc pas etre ferme
          //out_control << "01 d" << Qt::endl;
          // on vide les listes de commandes
          liste_commandes.clear();
          liste_commandes_TCP.clear();
          ui.pushButton_execution->setEnabled(false);  // on desactive le bouton pour lancer les commandes
          ui.pushButton_tcp->setEnabled(false);  // on desactive le bouton TCP
          ui.pushButton_stop->setEnabled(false);  // on desactive le bouton stop
          ui.pushButton_lecture_fichier->setEnabled(false);  // on desactive le bouton lecture fichier commandes
          //out_control << "01 e" << Qt::endl;
          // attente de 100 ms pour etre sur que la remise a 0 de la liste a bien ete prise en compte
          QEventLoop evtLoop;
          QTimer::singleShot(100, &evtLoop, SLOT(quit())); // Temps d'attente
          evtLoop.exec();
          return;
      }


      // on recupere toutes les donnes RS232
      // c'est une precaution pour etre sur qu'on part de zero avec une connexion reinitalisee
      port->clear(QSerialPort::AllDirections); // on efface tout ce qui est dans le cache
      port->setDataTerminalReady(false); // commande de reset materielle
      port->setRequestToSend(false);
      QThread::msleep(50); // pause de 50 ms
      port->setDataTerminalReady(true);
      port->setRequestToSend(true);
      receive_rs232_tout();




      //out_control << "02" << Qt::endl;

      qDebug("<debug> (2] (traitement_commandes) Etape 1 : send ENQ");
      ecriture_logs("Etape 1 : send ENQ");
      ////////////////////////////
      // Etape 1 : On envoie le ENQ
      // on recupere la date courante
      date = QDateTime::currentDateTime();
      int nb_error = send_rs232_short("ENQ");
      qDebug("<debug> (3] (traitement_commandes) Etape 1 : (bon si erreur=0) erreur : %d", nb_error);
      ecriture_logs("Code erreur envoi ENQ [0] : " + QString::number(nb_error));
      if( nb_error == -1)
      {
          qDebug("<debug> (4] (traitement_commandes) Erreur port NULL");

          emit affichageHTML_main_thread2("Erreur ouverture port COM", "rouge");
          emit affichageHTML_main_thread2("Cliquez sur le bouton Configuration pour changer le port COM", "rouge");
          emit affichageHTML_main_thread2("Obligation de quitter le programme", "rouge");

          emit affichageHTML_main_thread2("Erreur port NULL", "rouge");
          fermeture_port();
          return;
      }
      else if( nb_error == -2)
      {
          qDebug("<debug> (5] (traitement_commandes) Erreur envoi port NULL");
          emit affichageHTML_main_thread2("Erreur envoi commande ENQ", "rouge");
          fermeture_port();
          return;
      }
      //out_control << "02 avant affichage" << Qt::endl;
      qDebug("<debug> (6] (traitement_commandes) (envoi) Send Request (ENQ)");
      //out_control << "02 apres affichage" << Qt::endl;
      nb_error = 0;


      //out_control << "03" << Qt::endl;


      qDebug("<debug> (7] (traitement_commandes) Etape 2 : reception EOT");
      ecriture_logs("Etape 2 : Reception du EOT");
      ////////////////////////////
      // Etape 2 : On recoit le EOT
      if(port != NULL)
        port->waitForReadyRead(2000); // on attend au maximum 2 secondes que des donnees soient disponibles
      // on recupere la date courante
      date = QDateTime::currentDateTime();

      commande_rs232 = ""; // Reinitialisation de la variable
      nb_error = receive_rs232(1);
      ecriture_logs("Code erreur de la reception EOT [1] : " + QString::number(nb_error));
      if( nb_error == -1)
      {
          qDebug("<debug> (8] (traitement_commandes) Erreur de reception du Ready to Receive (EOT) - port NULL");
          emit affichageHTML_main_thread2("Erreur de reception du Ready to Receive (EOT) - port NULL", "rouge");
          fermeture_port();
          return;
      }
      if( nb_error == -2)
      {
          qDebug("<debug> (9] (traitement_commandes) Erreur de reception du Ready to Receive (EOT) - Pas de donnees a recuperer");
          emit affichageHTML_main_thread2("Erreur de reception du Ready to Receive (EOT) - Pas de données à récupérer", "rouge");
          fermeture_port();
          return;
      }
      if( nb_error == -3)
      {
          qDebug("<debug> (traitement_commandes) Erreur de reception du Ready to Receive (EOT) - Pas de donnees lues");
          emit affichageHTML_main_thread2("Erreur de reception du Ready to Receive (EOT) - Pas de données lues", "rouge");
          fermeture_port();
          return;
      }
      if( nb_error == -4)
      {
          qDebug("<debug> (10] (traitement_commandes) Erreur de reception du Ready to Receive (EOT) - Mauvaises donnees lues");
          emit affichageHTML_main_thread2("Erreur de reception du Ready to Receive (EOT) - Mauvaises données lues", "rouge");
          fermeture_port();
          return;
      }
      else
      {
          //out_control << "03 avant affichage" << Qt::endl;
           qDebug() << "<debug> (11] (traitement_commandes) (reception) : " << commande_rs232;
          //out_control << "03 apres affichage" << Qt::endl;
      }

//      qDebug("<debug> (12] (traitement_commandes) etape2 : (taille commande) %d", commande_rs232.size());
      commande_rs232 = "";
      nb_error = 0;



      //out_control << "04" << Qt::endl;



      qDebug("<debug> (13] (traitement_commandes) Etape 3 : envoie commande");
      ecriture_logs("Etape 3 : envoi de la commande");
      ////////////////////////////
      // Etape 3 : On envoie la commande
      // on recupere la date courante
      date = QDateTime::currentDateTime();
      commande_rs232_complet = ""; // initialisation
      nb_error = send_rs232_long(commande_a_executer.size(),commande_a_executer);
      if( nb_error == -1)
      {
          qDebug("<debug> (14] (traitement_commandes) Erreur envoi de la commande");
          emit affichageHTML_main_thread2("Erreur envoi de la commande", "rouge");
          fermeture_port();
          return;
      }
      //out_control << "04 avant affichage" << Qt::endl;
      qDebug() << "<debug> (15] (traitement_commandes) (envoie) : " << commande_rs232_complet;
      //out_control << "04 apres affichage" << Qt::endl;
      ecriture_logs(date.toString( "dd/MM/yyyy HH:mm:ss.zzz") + " (envoi) " + commande_rs232_complet);
      ecriture_logs("Code erreur de envoi commande [0] : " + QString::number(nb_error));





      //out_control << "05" << Qt::endl;




      qDebug("<debug> (16] (traitement_commandes) Etape 4 : reception ACK");
      ecriture_logs("Etape 4 : Reception de ACK");
      ////////////////////////////
      // Etape 4 : On recoit le ACK
      if(port != NULL)
           port->waitForReadyRead(2000); // on attend au maximum 2 secondes que des donnees soient disponibles
      // on recupere la date courante
      date = QDateTime::currentDateTime();
        nb_error = receive_rs232(1);
        ecriture_logs("Code erreur de reception ACK [1] : " + QString::number(nb_error));
        if( nb_error == -1)
        {
            qDebug("<debug> (17] (traitement_commandes) Erreur de reception du Acknowledge (ACK) - port NULL");
            emit affichageHTML_main_thread2("Erreur de reception du Acknowledge (ACK) - port NULL", "rouge");
            fermeture_port();
            return;
        }
        if( nb_error == -2)
        {
            qDebug("<debug> (18] (traitement_commandes) Erreur de reception du Acknowledge (ACK) - Pas de donnees a recuperer");
            emit affichageHTML_main_thread2("Erreur de reception du Acknowledge (ACK) - Pas de données à récupérer", "rouge");
            fermeture_port();
            return;
        }
        if( nb_error == -3)
        {
            qDebug("<debug> (19] (traitement_commandes) Erreur de reception du Acknowledge (ACK) - Pas de donnees lues");
            emit affichageHTML_main_thread2("Erreur de reception du Acknowledge (ACK) - Pas de données lues", "rouge");
            fermeture_port();
            return;
        }
        if( nb_error == -4)
        {
            qDebug("<debug> (20] (traitement_commandes) Erreur de reception du Acknowledge (ACK) - Mauvaises donnees lues");
            emit affichageHTML_main_thread2("Erreur de reception du Acknowledge (ACK) - Mauvaises données lues", "rouge");
            fermeture_port();
            return;
        }
        else
        {
            //out_control << "05 avant affichage" << Qt::endl;
            qDebug() << "<debug> (21] (traitement_commandes) (reception) : " << commande_rs232;
            //out_control << "05 apres affichage" << Qt::endl;
        }
  //    qDebug("<debug> (22] etape4 : %d", commande_rs232.size());
        commande_rs232 = "";
        nb_error = 0;



      //out_control << "06" << Qt::endl;



      qDebug("<debug> (23] (traitement_commandes) Etape 5 : reception ENQ");
      ecriture_logs("Etape 5 : reception de ENQ");
        ////////////////////////////
      // Etape 5 : On recoit le ENQ
        if(port != NULL)
             port->waitForReadyRead(180000); // on attend au maximum 3 minutes que des donnees soient disponibles
        // on recupere la date courante
        date = QDateTime::currentDateTime();
        nb_error = receive_rs232(1);
        ecriture_logs("Code erreur reception ENQ [1] : " + QString::number(nb_error));
        if( nb_error == -1)
        {
            qDebug("<debug> (24] (traitement_commandes) Erreur de reception du Send request (ENQ) - port NULL");
            emit affichageHTML_main_thread2("Erreur de reception du Send request (ENQ) - port NULL", "rouge");
            fermeture_port();
            return;
        }
        if( nb_error == -2)
        {
            qDebug("<debug> (25] (traitement_commandes) Erreur de reception du Send request (ENQ) - Pas de donnees a recuperer");
            emit affichageHTML_main_thread2("Erreur de reception du Send request (ENQ) - Pas de données à récupérer", "rouge");
            fermeture_port();
            return;
        }
        if( nb_error == -3)
        {
            qDebug("<debug> (26] (traitement_commandes) Erreur de reception du Send request (ENQ) - Pas de donnees lues");
            emit affichageHTML_main_thread2("Erreur de reception du Send request (ENQ) - Pas de données lues", "rouge");
            fermeture_port();
            return;
        }
        if( nb_error == -4)
        {
            qDebug("<debug> (27] (traitement_commandes) Erreur de reception du Send request (ENQ) - Mauvaises donnees lues");
            emit affichageHTML_main_thread2("Erreur de reception du Send request (ENQ) - Mauvaises données lues", "rouge");
            fermeture_port();
            return;
        }
        else
        {
            //out_control << "06 avant affichage" << Qt::endl;
            qDebug() << "<debug> (traitement_commandes) (reception) : " << commande_rs232;
            //out_control << "06 apres affichage" << Qt::endl;
        }

        commande_rs232 = "";
        nb_error = 0;


      //out_control << "07" << Qt::endl;


      qDebug("<debug> (traitement_commandes) Etape 6 : envoie EOT");
      ecriture_logs("Etape 6 : envoi EOT");
      ////////////////////////////
      // Etape 6 : On envoie le EOT
      // on recupere la date courante
      date = QDateTime::currentDateTime();
        nb_error = send_rs232_short("EOT");
        ecriture_logs("Code erreur envoi EOT [0] : " + QString::number(nb_error));
        if( nb_error == -1)
        {
            qDebug("<debug> (traitement_commandes) Erreur envoi du Ready to Receive (EOT)");
            emit affichageHTML_main_thread2("Erreur envoi du Ready to Receive (EOT)", "rouge");
            fermeture_port();
            return;
        }
        //out_control << "07 avant affichage" << Qt::endl;
        qDebug("<debug> (traitement_commandes) (envoi) Send Request (ENQ)");
        //out_control << "07 apres affichage" << Qt::endl;
        nb_error = 0;


      //out_control << "08" << Qt::endl;



      qDebug("<debug> (traitement_commandes) Etape 7 : reception commande");
      ecriture_logs("Etape 7 : Reception commande");
        ////////////////////////////
      // Etape 7 : On recoit la commande
        if(port != NULL)
              port->waitForReadyRead(2000); // on attend au maximum 2 secondes que des donnees soient disponibles
        // on recupere la date courante
        date = QDateTime::currentDateTime();
        nb_error = receive_rs232(0);
        ecriture_logs("Code erreur reception commande [>1] : " + QString::number(nb_error));
        if( nb_error == -1)
        {
            qDebug("<debug> (traitement_commandes) Erreur de reception du Termination - port NULL");
            emit affichageHTML_main_thread2("Erreur de reception du Termination - port NULL", "rouge");
            fermeture_port();
            return;
        }
        if( nb_error == -2)
        {
            qDebug("<debug> (traitement_commandes) Erreur de reception du Termination - Pas de donnees a recuperer");
            emit affichageHTML_main_thread2("Erreur de reception du Termination - Pas de données à récupérer", "rouge");
            fermeture_port();
            return;
        }
        if( nb_error == -3)
        {
            qDebug("<debug> (traitement_commandes) Erreur de reception du Termination - Pas de donnees lues");
            emit affichageHTML_main_thread2("Erreur de reception du Termination - Pas de données lues", "rouge");
            fermeture_port();
            return;
        }
        else
        {
              //out_control << "08 avant affichage" << Qt::endl;
              qDebug() << "<debug> (traitement_commandes) (reception) : " << commande_rs232;
              //out_control << "08 apres affichage" << Qt::endl;
              ecriture_logs(date.toString( "dd/MM/yyyy HH:mm:ss.zzz") + " (reception) " + commande_rs232);
              emit affichageHTML_main_thread2("Reception : " + commande_rs232, "noir");
        }


        if (commande_recue_avant == commande_rs232)
        {
            emit affichageHTML_main_thread2("Attention, risque d'erreur (même commande que précédemment)", "rouge");
        }

        commande_recue_avant = commande_rs232;
        commande_rs232 = "";
        nb_error = 0;


      //out_control << "09" << Qt::endl;


        qDebug("<debug> (traitement_commandes) Etape 8 : envoie ACK");
        ecriture_logs("Etape 8 : envoi du ACK");
        ////////////////////////////
        // Etape 8 : On envoie le ACK
        // on recupere la date courante
        date = QDateTime::currentDateTime();
        nb_error = send_rs232_short("ACK");
        ecriture_logs("Code erreur envoi ACK [0] : " + QString::number(nb_error));
        if( nb_error == -1)
        {
              qDebug("<debug> (traitement_commandes) Erreur envoi du Acknowledge (ACK)");
              emit affichageHTML_main_thread2("Erreur envoi du Acknowledge (ACK)", "rouge");
              fermeture_port();
              return;
        }
        //out_control << "09 avant affichage" << Qt::endl;
        qDebug("<debug> (traitement_commandes) (envoi) Acknowledge (ACK)");
        //out_control << "09 apres affichage" << Qt::endl;
        nb_error = 0;


      //out_control << "10" << Qt::endl;


        // on attend quelques secondes secondes
        QString message_attente = "Pause de xxxx ms";
        message_attente.replace(QString("xxxx"),fenetre_parametres.ui.lineEdit_4->text());

        //out_control << "10 avant affichage" << Qt::endl;
        //out_control << "10 apres affichage" << Qt::endl;
        //QEventLoop evtLoop;
        //QTimer::singleShot(fenetre_parametres.ui.lineEdit_4->text().toInt(), &evtLoop, SLOT(quit())); // xxxx ms d'attente
        //evtLoop.exec();


      //out_control << "11" << Qt::endl;


        // fermeture du port RS232
        ecriture_logs("Fermeture du port COM");
        fermeture_port();

        // fermeture du fichier de controle
       //file_controle.close();

}





//////////////////////////////////////////////////////////
/// Ferme proprement le port
///
void TPG300::fermeture_port(void)
{

    // fermeture du port RS232
    if(port != NULL)
    {
        port->flush(); // on vide le cache
        port->close(); // on ferme le port
         delete port;
         port = NULL;
         fenetre_parametres.port_local = NULL;
    }

    return;

}








