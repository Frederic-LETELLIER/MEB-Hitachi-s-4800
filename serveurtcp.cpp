



/////////////////////////////////////
// Connexion au serveur TCP
/////////////////////////////////////
void TPG300::newconnectiontcp(void)
{

    affichageHTML("Un client se connecte au serveur TCP", "noir");

    // creation du socket
    QTcpSocket *socket = serveurTcp->nextPendingConnection();

    // on laisse 3 secondes au client pour transmettre des donnees (les commandes)
    socket->waitForReadyRead(3000);
    QString lignelue = "";
    lignelue = socket->readAll();

    // on verifie que la commande recue n'est pas vide
    if( lignelue.size() <= 0)
    {
        qDebug("<debug> (newconnectiontcp) Erreur, commande recue vide");
        affichageHTML("TCP - lecture : " + lignelue, "noir");
        // on envoie un KO
        socket->write("KO");
        return;
    }

    affichageHTML("TCP - lecture : " + lignelue, "noir");

    // On vérifie que le programme n'est pas déjà en train d'exécuter des instructions
   if( !liste_commandes_TCP.isEmpty() )
   {
       liste_commandes_TCP.append("lignelue");
       // on envoie un OK
       socket->write("OK");
       return;
   }
   else if( !liste_commandes.isEmpty() )
   {
       liste_commandes.append("lignelue");
       // on envoie un OK
       socket->write("OK");
       return;
   }

    // on envoie un OK pour indiquer que la commande va être exécutée
    socket->write("OK");

    affichageHTML( "Début du traitement des commandes TCP à exécuter" , "noir");

    //qDebug("<debug> liste vide : %d", liste_commandes.isEmpty());
    while (!liste_commandes_TCP.isEmpty())
    {

        // On recupere la commande a executer
        QString commande_a_executer_tcp = liste_commandes_TCP[0];

        // on supprime la commande de la liste
        liste_commandes_TCP.removeAt(0);

        // on transmet la commande pour traitement
        traitement_commandes(commande_a_executer_tcp);

    }

    affichageHTML("------------------------", "noir");
    affichageHTML( "Fin du traitement des commandes TCP à exécuter" , "noir");





    // fermeture du socket
    socket->close();


    return;
}


