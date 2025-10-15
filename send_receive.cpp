
// partie 1 = system code
// 1 seul octet
// 04 = EOT Pret a recevoir
// 05 - ENQ envoi d'une requete
// 06 - ACK validation
// 15 - NAK non validation

// partie 2 = text block composé de 3 sous parties
// ne peut pas dépasser 253 octets

// partie 2-1 = Text length
// correspond à 1 octet

// partie 2-2 = Text
// ne peut pas dépasser 250 octets

// partie 2-3 = Sum Check Data
// est composé de 2 bytes
// S1 : controle des données au dessus de la position
// S0 : controle des donnee en dessous de la position







/////////////////////////////////////////////
// Recevoir
int TPG300::receive_rs232(int nombre_a_lire)
{

  if(port == NULL)
  {
      qDebug("<debug> (receive_rs232) Le port n'existe pas");
      return -1;
  }

  // on regarde s'il y a quelque chose a recuperer
  if( port->bytesAvailable() == 0)
  {
      qDebug("<debug> (receive_rs232) (receive_rs232) Aucune donnee a recuperer");
      return -2; // on quitte s'il n'y a rien a recuperer
  }

  // on laise un delai d'attente de 10ms
  QEventLoop evtLoop;
  QTimer::singleShot(10, &evtLoop, SLOT(quit())); // 10 ms d'attente
  evtLoop.exec();

  // lecture d'une ligne
  int nombre_lu = 0;
  char tableau[260];
  int i=0;
  for (i=0; i<255; i++) // on fait que le tableau contienne uniquement la valeur vide
    tableau[i] = '\0';

  if( nombre_a_lire == 0 )
     nombre_lu = port->read(tableau, 254); // on stockage dans tableau avec une limite de 250 caracteres
  else
    nombre_lu = port->read(tableau, nombre_a_lire); // on stockage dans tableau avec une limite de 250 caracteres

  qDebug("<debug> (receive_rs232) Nombre de caracteres lus : %d", nombre_lu);


  if (nombre_lu == 0)
  {
      qDebug("<debug> (receive_rs232) Erreur, aucun caractere lu");
      return -3;
  }

  //affichageHTML("Premier octet : [" + QString::number(tableau[0]) + "]"  , "rouge");

  // si on a recu un seul caractere
  if (nombre_lu == 1)
  {
      if (tableau[0] == 0x0004)
          commande_rs232 = "Ready to Receive [EOT]";
       else if (tableau[0] == 0x0005)
          commande_rs232 = "Send Request [ENQ]";
       else if (tableau[0] == 0x0006)
          commande_rs232 = "Acknowledge [ACK]";
       else if (tableau[0] == 0x0015)
       {
          commande_rs232 = "Negative Acknowledge [NAK]";
          qDebug("<debug> (receive_rs232) Erreur, reception d'un NAK");
          return -4;
      }
       else
       {
          commande_rs232 = "Error";
          qDebug("<debug> (receive_rs232) Erreur, reception d'une erreur indeterminee");
          return -4;
      }

    return 1;
  }


  if (nombre_lu > 1)
  {
    int nombre_caracteres = tableau[0];
    char tableau2[500];
    for (int i=0 ; i<nombre_caracteres ; i++)  // va de 0 a "nombre_caracteres-1"
    {
     tableau2[i] = tableau[i+1];
    }
    tableau2[nombre_caracteres] = 0x0000;
    commande_rs232 = QString(tableau2);
    return nombre_lu;
  }

  return 0;
}




/////////////////////////////////////////////
// Envoyer
int TPG300::send_rs232_short(QString cmd_short)
{

  if(port == NULL)
  {
      qDebug("<debug> (send_rs232_short) Le port n'existe pas");
      return -1;
  }

  char tableau[260]; // creation d'un tableau
  int i=0;
  for (i=0; i<255; i++) // on fait que le tableau contienne uniquement la valeur vide
    tableau[i] = '\0';

  if (cmd_short == "EOT")
    tableau[0] = 0x0004;
  else if (cmd_short == "ENQ")
    tableau[0] = 0x0005;
  else if (cmd_short == "ACK")
    tableau[0] = 0x0006;
  else if (cmd_short == "NAK")
    tableau[0] = 0x0015;
  else
    return -1;

  int nb_caracteres_envoyes=0;
  nb_caracteres_envoyes = port->write(tableau, 1); // on transmet la commande RS232
  port->flush(); // On demande a ce que tout soit envoye

  if(nb_caracteres_envoyes <= 0)
  {
      qDebug("<debug> (send_rs232_short) Erreur sur le nombre de caracteres envoyes");
      qDebug("<debug> (send_rs232_short) caracteres envoyes : %d", nb_caracteres_envoyes);
      return -2;
  }

  return 0;
}





/////////////////////////////////////////////
// Envoyer
int TPG300::send_rs232_long(int longueur, QString cmd_long)
{

    // commande_rs232_complet est un QString
    commande_rs232_complet = commande_rs232_complet.append("[");
    commande_rs232_complet = commande_rs232_complet.append(QString::number(longueur));
    commande_rs232_complet = commande_rs232_complet.append("] ");
    commande_rs232_complet = commande_rs232_complet.append(cmd_long);
    commande_rs232_complet = commande_rs232_complet.append(" [");
    // qDebug("<debug> (send_rs232_long) commande_rs232_complet : %s", commande_rs232_complet);
    //qDebug() << "<debug> (send_rs232_long) commande_rs232_complet : " << commande_rs232_complet;

    int somme = 0;

    char tableau[500]; // creation d'un tableau

    tableau[0] = longueur; // 1er octet = longueur de la commande

    // on remplit le tableau avec la commande et on fait la somme des caracteres
    for (int i=0; i<longueur; i++)  // on recopie la chaine de caracteres dans le tableau
    {
        tableau[i+1] = cmd_long[i].unicode(); // renvoi un const QChar
        somme = somme + cmd_long[i].unicode(); // renvoi la valeur décimale du caractère
        // '0'=48, '5'=53 et '9'=57
        // A=63, N=78 et Z=90
        // a=97, n=110 et z=122
        //qDebug("<debug> somme : %d", somme);
    }

    tableau[longueur+1] = 0;  // s1  - octet de verification
    tableau[longueur+2] = 0;  // s2  - octet de verification

    QString hexadecimal1 = 0;
    QString hexadecimal2 = 0;
    hexadecimal1.setNum(somme,16);
    if( hexadecimal1.size() == 3)
      hexadecimal1 = hexadecimal1.first(1); // on ne garde que le premier
    else if (hexadecimal1.size() == 4)
      hexadecimal1 = hexadecimal1.first(2); // on ne garde que les 2 premiers
    else
      hexadecimal1 = "0"; // vide
    hexadecimal2.setNum(somme,16);
    if( hexadecimal2.size() >=3 )
      hexadecimal2 = hexadecimal2.last(2); // on ne garde que les 2 derniers
    commande_rs232_complet = commande_rs232_complet.append(hexadecimal1);
    commande_rs232_complet = commande_rs232_complet.append("] ");
    commande_rs232_complet = commande_rs232_complet.append("[");
    commande_rs232_complet = commande_rs232_complet.append(hexadecimal2);
    commande_rs232_complet = commande_rs232_complet.append("]");

    // qDebug("<debug> (send_rs232_long) commande_rs232_complet : %s", commande_rs232_complet);
    qDebug() << "<debug> (send_rs232_long) commande_rs232_complet : " << commande_rs232_complet;

    while (somme >= 256)
    {
      if (somme >= 256)
      {
        somme = somme - 256;
        tableau[longueur+1] = tableau[longueur+1] + 1;  // s1
       }
      }
    // exemple pour MQHSa ==> somme = 400. 400-256 = 154 = 0x009A

    tableau[longueur+2] = somme ; // s2
   // tableau[longueur+2] = 0x009A ; // s2 ecriture hexa

  if(port == NULL)
  {
      qDebug("<debug> (send_rs232_long) Le port n'existe pas");
      return -1;
  }

  port->write(tableau, longueur+3); // on transmet la commande RS232
  port->flush(); // On demande a ce que tout soit envoye

   return 0;
}








/////////////////////////////////////////////
// Recevoir tout
void TPG300::receive_rs232_tout()
{

    if(port == NULL)
    {
        qDebug("<debug> (receive_rs232) Le port n'existe pas");
        return;
    }

    // on regarde s'il y a quelque chose a recuperer
    if( port->bytesAvailable() == 0)
    {
        qDebug("<debug> (receive_rs232) (receive_rs232) Aucune donnee a recuperer");
        return; // on quitte s'il n'y a rien a recuperer
    }

    // lecture d'une ligne
    char tableau[260];
    port->read(tableau, 254); // on stockage dans tableau avec une limite de 250 caracteres

    return;
}
