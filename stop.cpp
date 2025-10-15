


/////////////////////////////////////////////
// Arret de l'envoi des commandes
/////////////////////////////////////////////
void TPG300::on_pushButton_stop_clicked(void)
{


    // on vide les listes de commandes
    liste_commandes.clear();
    liste_commandes_TCP.clear();

    ui.pushButton_execution->setEnabled(false);  // on desactive le bouton pour lancer les commandes

    affichageHTML("STOP !!!", "rouge");
    affichageHTML("La liste des instructions a ete videe", "rouge");

}
