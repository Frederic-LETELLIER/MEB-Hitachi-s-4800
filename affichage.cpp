
/////////////////////////////////////////////
// Affichage Thread
/////////////////////////////////////////////
void TPG300::affichageHTML_thread(QString ligne, QString couleur)
{

    // Pour connaitre l'identifiant du Thread
    qDebug() << "Current thread (affichageHTML):" << QThread::currentThread();



    affichageHTML(ligne, couleur);
    return;
}



/////////////////////////////////////////////
// Affichage
/////////////////////////////////////////////
void TPG300::affichageHTML(QString ligne, QString couleur)
{

  QString ligne_html;

  // 4 couleurs sont prevues : Rouge, Bleu, Vert et Noir
  if( couleur.contains("rouge") )
     ligne_html = "<font color=\"Red\">" + ligne + "</font><br>";
  else if( couleur.contains("bleu") )
       ligne_html = "<font color=\"Blue\">" + ligne + "</font><br>";
  else if( couleur.contains("vert") )
       ligne_html = "<font color=\"Green\">" + ligne + "</font><br>";
  else if( couleur.contains("noir") )
       ligne_html = "<font color=\"Black\">" + ligne + "</font><br>";
  else if( couleur.contains("rose") )
      ligne_html = "<font color=\"Pink\">" + ligne + "</font><br>";
  else if( couleur.contains("orange") )
      ligne_html = "<font color=\"Orange\">" + ligne + "</font><br>";
  else  // si aucune couleur n'est definie, la couleur par defaut est le noir
       ligne_html = "<font color=\"Black\">" + ligne + "</font><br>";


  // on regarde si c'est un envoi
  if(ligne.contains("Commande à éxécuter : "))
  {
    ligne_html = ligne.replace("Commande à éxécuter : ","");
    ligne_html = "<strong><font color=\"Blue\">" + ligne_html + "</font></strong>";
    ligne_html = "<font color=\"Black\">Commande à éxécuter : </font>" + ligne_html + "<br>";
  }


  // on regarde si c'est une reception
  if(ligne.contains("Reception : "))
  {
      ligne_html = ligne.replace("Reception : ","");
      ligne_html = "<strong><font color=\"Green\">" + ligne_html + "</font></strong>";
      ligne_html = "<font color=\"Black\">Reception : </font>" + ligne_html + "<br>";
  }


  // on positionne le curseur a la fin du texte
  QTextCursor cursor = ui.textBrowser->textCursor();
  cursor.movePosition(QTextCursor::End);
  ui.textBrowser->setTextCursor(cursor);

  // on met le texte
  ui.textBrowser->insertHtml(ligne_html);

  // on positionne le curseur a la fin du texte
  //cursor.movePosition(QTextCursor::End);
  //ui.textBrowser->setTextCursor(cursor);

  // on demande que le curseur soit visible (fait un scroll si necessaire)
  ui.textBrowser->ensureCursorVisible();

  // rafraichissement de l'affichage
//  ui.textBrowser->repaint(); // je retire car cela bug avec le multithread

  return;
}



/////////////////////////////////////////////
// Affichage Thread
/////////////////////////////////////////////
void TPG300::affichage_thread(QString ligne)
{

    // Pour connaitre l'identifiant du Thread
    qDebug() << "Current thread (affichage):" << QThread::currentThread();

    affichage(ligne);
    return;
}


/////////////////////////////////////////////
// Affichage
/////////////////////////////////////////////
void TPG300::affichage(QString ligne)
{

    // on positionne le curseur a la fin du texte
    QTextCursor cursor = ui.textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui.textBrowser->setTextCursor(cursor);

//  ui.textBrowser->append(ligne + "\n");
  ui.textBrowser->append(ligne);

  // on positionne le curseur a la fin du texte
  //cursor.movePosition(QTextCursor::End);
  //ui.textBrowser->setTextCursor(cursor);

  // on demande que le curseur soit visible (fait un scroll si necessaire)
  ui.textBrowser->ensureCursorVisible();


  return;
}


/////////////////////////////////////////////
// Gestion des logs
/////////////////////////////////////////////
void TPG300::ecriture_logs(QString ligne_a_ecrire1)
{

    // Le premier fichier log va avoir "la date" + "envoi"/"reception + une info
    // Le second fichier log va uniquement avoir une info

    // Ouverture du fichier logs
    QFile file1;

    // nom_fichier_log = "./logs/log_MEB_" + date.toString( "yyyy-MM-dd_HH-mm-ss") + ".txt";
    file1.setFileName(nom_fichier_log);
    file1.open(QIODevice::Append | QIODevice::Text );
    QTextStream out1(&file1);
    out1 << ligne_a_ecrire1 << Qt::endl;
    file1.close();

  return;
}



/////////////////////////////////////////////
// Affichage Thread
/////////////////////////////////////////////
void TPG300::affichage_choix_thread(int choix)
{
    // Pour connaitre l'identifiant du Thread
    qDebug() << "Current thread (affichage_choix):" << QThread::currentThread();


    if( choix == 1)
    {
        affichageHTML("------------------------<br>Fin du traitement des commandes à exécuter", "noir");
    }

}

