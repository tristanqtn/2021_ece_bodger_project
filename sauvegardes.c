#include "header.h"


/**< SAUVEGARDE DU GAGNANT D'UNE PARTIE >*****************************************************************/
void sauvegardeGagnant (t_partie * maPartie)
{
    //Déclarations
    int i;
    FILE * fichier;
    t_identite * monJoueur;
    fichier = chargerFichier("data/fichiers/historiqueVictoires.txt", "a");//ouverture


    monJoueur = maPartie->ancreJoueur;

    for(i=0; i<maPartie->nbJoueur; i++)
    {
        fprintf(fichier, "%s \t %s \t %d \n", monJoueur->pseudo, monJoueur->nomAvatar, monJoueur->score);//ecriture des infos
        monJoueur = monJoueur->suivant;

    }
    fclose(fichier);
}
/*********************************************************************************************************/

/**< LECTURE ET AFFICHAGE DES SAUVEGARDES >***************************************************************/
void lireHistoParties (SAMPLE * musique)
{
    //Déclarations
    int sortie = 0, posliste =1;

    BITMAP * fond;

    //chargement des data

    fond = chargerImage("data/images/menu/historique_victoires.bmp");                //image


    //Affichage ajusté de l'image
    stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);
    show_mouse(screen);

    chargerAfficherSauv(posliste); //on charge la liste

    while(!key[KEY_ESC] && sortie == 0)
    {
        if(mouse_b == 1)
        {
            if(appuieBouton(1638, 541, 1798, 611) == 1)
            {//fleche gauche
                posliste++;
                chargerAfficherSauv(posliste);

            }
            if (appuieBouton(132, 548, 292, 614) == 1)
            {//fleche droite
                posliste--;
                if(posliste == 0)
                {
                    posliste=1;
                }
                chargerAfficherSauv(posliste);
            }
            if (appuieBouton(1733, 834, 1899, 1061) == 1)
            {//exit
                sortie = 1;
                show_mouse(NULL);
            }

        }
    }
    destroy_bitmap(fond);
}
/*********************************************************************************************************/


/**< AFFICHAGE DES SAUVEGARDES >**************************************************************************/
void chargerAfficherSauv (int posListe)
{
    //Déclarations
    int i, j;
    int posX = 460;
    int posY [4] = {420, 575, 732, 885};

    char nom[4][20];
    char avatar[4][10];
    int score [4];

    FILE * fichier = chargerFichier("data/fichiers/historiqueVictoires.txt", "r");  //fichier de sauvegarde

    //lecture des histo
    for(j=0; j<posListe; j++)
    {
        for(i=0; i<4; i++)
        {
            score[i] = 0;
        }

        for(i=0; i<4; i++)
        {
            fscanf(fichier, "%s", nom[i]);
            fscanf(fichier, "%s", avatar[i]);
            fscanf(fichier, "%d", &score[i]);
        }
    }

    //affichage
    for(i=0; i<4; i++)
    {
        if(score[i] != 0)
            textprintf_ex(screen, font, posX, posY[i], makecol(0, 0, 0), makecol(169, 215, 142), "%s avec %s a actuellement %d points ", nom[i], avatar[i], score[i]);    //affichage pos Y souris
        else
           textout_ex(screen, font, "Pas de partie anterieurs sauvegardee                                          ", posX, posY[i], makecol(0, 0, 0), makecol(169, 209, 142));
    }
    fclose(fichier);
}
/*********************************************************************************************************/



/**< AFFICHAGE DES SAUVEGARDES >**************************************************************************/
t_identite * chargerID ()
{
    //declarations
    int posListe =1;
    int i, k, sortie =0, persoARetourner, choix =0;
    char inutile [1000];
    int posX = 460;
    int posY [4] = {420, 575, 732, 885};

    t_identite ** tempo;

    tempo = (t_identite **) malloc (sizeof(t_identite*) * 100); //alloc tab de joueurs
    FILE * fichier = chargerFichier("data/fichiers/historiqueVictoires.txt", "r");  //fichier de sauvegarde

    if(tempo == NULL)
    {//procédure d'erreur
        sortieErreur("Problème d'allocation de profil de joueur");
    }

        for(i=0; i<100; i++)
        {
            tempo[i] = allocInitIdentite(); //alloc identité
            tempo[i]->score = 0;
        }

        i=0;

        do
        {//pour le nombre de joueurs deja sauvé


            //recup données
            fscanf(fichier, "%s", tempo[i]->pseudo);
            fscanf(fichier, "%s", tempo[i]->nomAvatar);
            fscanf(fichier, "%d", &tempo[i]->score);
            fgets(inutile, 1000, fichier);
            i++;
        }while(!feof(fichier));

   fclose(fichier);//fermeture lecture
   fichier = chargerFichier("data/fichiers/historiqueVictoires.txt", "w"); //ouverture ecriture



    while(choix ==0)
    {//tant qu'un choix a pas ete fait
        for(k=0; k<posListe; k++)
        {//affichage
            for(i=0; i<4; i++)
            {
            if(tempo[(posListe-1)*4+i]->score != 0)
            {
                textprintf_ex(screen, font, posX, posY[i], makecol(0, 0, 0), makecol(169, 215, 142), "Pseudo : %s  Avatar : %s a actuellement %d points ", tempo[(posListe-1)*4+i]->pseudo, tempo[(posListe-1)*4+i]->nomAvatar, tempo[(posListe-1)*4+i]->score);    //affichage pos Y souris
            }
            else
                textout_ex(screen, font, "Pas de profils anterieurs sauvegardes                                          ", posX, posY[i], makecol(0, 0, 0), makecol(169, 209, 142));
            }

        }
        show_mouse(screen);
        sortie =0;
        while(sortie == 0 && choix == 0)
        {//tant qu'on a pas selectionner un joueur
            if(appuieBouton(420, 390, 1500, 455) == 1)
            {//joueur1
                persoARetourner =0;
                choix = 1;
            }
            if(appuieBouton(420, 546, 1500, 611) == 1)
            {//joueur2
                persoARetourner =1;
                choix  = 1;
            }
            if(tempo[2]->score != 0)
            {
                if(appuieBouton(420, 700, 1500, 767) == 1)
                {//joueur3
                    persoARetourner =2;
                    choix  = 1;
                }
            }
            if(tempo[3]->score != 0)
            {//si 4 joueurs sauvés
                if(appuieBouton(420, 858, 1500, 923) == 1)
                {//joueur4
                    persoARetourner =3;
                    choix  = 1;
                }
            }
            if(appuieBouton(1638, 541, 1798, 611) == 1)
            {//fleche droite
                posListe++;
                sortie = 1;
            }
            if (appuieBouton(132, 548, 292, 614) == 1)
            {//fleche gauche
                posListe--;
                sortie =1;
                if(posListe == 0)
                {
                    posListe=1;
                }
            }
        }
    }

    //suppression du profil selectionne
    tempo[persoARetourner+(posListe-1)*4]->score=0;

    for(i=0; i<100; i++)
    {//réecriture des autres profils
        if(tempo[i]->score!=0)
        {
            fprintf(fichier, "%s \t %s \t %d \n", tempo[i]->pseudo, tempo[i]->nomAvatar, tempo[i]->score);

    }
    }

    fclose(fichier);//fermeture fichier
    return tempo[persoARetourner+(posListe-1)*4];//rendu du profil
}//le profil du joueur sera sauvegardé en fin de prochaine partie
/*********************************************************************************************************/


/**< RECUPARATION D'UN IDENTITE DE JOUEUR PRECEDENT POUR NOUVELLE PARTIE >*********************************/
t_identite * recupID()
{//est different di recupJoueurs() car c'est pour unr nouvelle partie
    ///Déclarations
    BITMAP * fond;
    t_identite * rendu;

    fond = chargerImage("data/images/menu/anciensProfils.bmp");//chargement bitmap
    stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

    rendu = chargerID();

    //destructions et fermeture
    destroy_bitmap(fond);

    //rendu de l'identite
    return (rendu);
}
/*********************************************************************************************************/



/**< SAUVEGARDE DE TOUT LES ELEMENTS DE JEU >*************************************************************/
void sauvegardeTout (t_partie * maPartie)
{
    sauvegardeNBJoueur(maPartie);//sauvegarde du nb de joueurs
    sauvegardeJoueurs(maPartie);//sauvegarde des profils de joueurs
    sauvegardePioche(maPartie->ancrePioche);//sauvegarde de la pioche
    sauvegardePlateau(maPartie->plateau);//sauvegarde du plateau
}
/*********************************************************************************************************/



/**< SAUVEGARDE DU NB DE JOUEURS >************************************************************************/
void sauvegardeNBJoueur (t_partie * maPartie)
{
    ///Déclarations
    FILE * fichier;

    fichier = fopen("data/fichiers/nbJoueurs.txt", "w"); //ouverture du fichier

    fprintf(fichier, "%d", maPartie->nbJoueur); //ecriture du nombre de joueurs
    fclose(fichier);//fermeture du fichier
}
/*********************************************************************************************************/



/**< RECUPERATION DU NB DE JOUEURS >**********************************************************************/
int recupNBJoueur ()
{
    ///Déclarations
    FILE * fichier;
    int nbJoueurs;

    fichier = fopen("data/fichiers/nbJoueurs.txt", "r");//ouverture du fichier

    fscanf(fichier, "%d", &nbJoueurs);//lecture de la valeur

    fclose(fichier);//fermeture du fichier
    return nbJoueurs;//rendu du nb de joueurs
}
/*********************************************************************************************************/



/**< SAUVEGARDE DE LA PIOCHE >****************************************************************************/
void sauvegardePioche(t_cartePioche * ancre)
{
    ///Déclarations
    t_cartePioche * ptCourant;
    FILE * fichier;

    fichier = chargerFichier("data/fichiers/sauvegardePioche.txt", "w");//ouverture du fichier

    ptCourant = ancre;//initialisation du parcours
    if(ancre != NULL)
    {
        while(ptCourant->suivante != NULL)
        {//tant que l'on atteint pas la fin de la pioche
            if(ptCourant->typeDeCarte->estCeMap ==1 ||ptCourant->typeDeCarte->estCeEboulement==1)
            {//sauvegarde spécifique si c'est une carte "MAP" ou un eboulement
                fprintf(fichier, "%d\t%d\t%d\t%d\n", ptCourant->typeDeCarte->estCeChemin, ptCourant->typeDeCarte->estCeEboulement, ptCourant->typeDeCarte->estCeMap, ptCourant->typeDeCarte->estCeObjet);
            }
            else
            {//sinon sauvegarde "classique"
                fprintf(fichier, "%d\t%d\t%d\t%d\t", ptCourant->typeDeCarte->estCeChemin, ptCourant->typeDeCarte->estCeEboulement, ptCourant->typeDeCarte->estCeMap, ptCourant->typeDeCarte->estCeObjet);
            }
            if(ptCourant->typeDeCarte->estCeChemin == 1)
            {//si c'est un chemin, sauvegarde des entrées
                fprintf(fichier, "%d\t%d\t%d\t%d\t%d\n", ptCourant->typeDeCarte->carteChemin->estCeImpasse, ptCourant->typeDeCarte->carteChemin->haut, ptCourant->typeDeCarte->carteChemin->bas, ptCourant->typeDeCarte->carteChemin->gauche, ptCourant->typeDeCarte->carteChemin->droite);
            }
            else if(ptCourant->typeDeCarte->estCeObjet == 1)
            {//si c'est un objet sauvegarde du type d'onjet
                fprintf(fichier, "%d\t%d\t%d\t%d\n", ptCourant->typeDeCarte->carteObjet->estCeReparation, ptCourant->typeDeCarte->carteObjet->chariot, ptCourant->typeDeCarte->carteObjet->lampe, ptCourant->typeDeCarte->carteObjet->pioche);
            }
            ptCourant = ptCourant->suivante;//parcours
        }
        //dernière répétition pour la dernière carte de la pioche (meme fonctionnement)
        if(ptCourant->typeDeCarte->estCeMap ==1 ||ptCourant->typeDeCarte->estCeEboulement==1)
        {
            fprintf(fichier, "%d\t%d\t%d\t%d\n", ptCourant->typeDeCarte->estCeChemin, ptCourant->typeDeCarte->estCeEboulement, ptCourant->typeDeCarte->estCeMap, ptCourant->typeDeCarte->estCeObjet);
        }
        else
        {
            fprintf(fichier, "%d\t%d\t%d\t%d\t", ptCourant->typeDeCarte->estCeChemin, ptCourant->typeDeCarte->estCeEboulement, ptCourant->typeDeCarte->estCeMap, ptCourant->typeDeCarte->estCeObjet);
        }
        if(ptCourant->typeDeCarte->estCeChemin == 1)
        {
            fprintf(fichier, "%d\t%d\t%d\t%d\t%d\n", ptCourant->typeDeCarte->carteChemin->estCeImpasse, ptCourant->typeDeCarte->carteChemin->haut, ptCourant->typeDeCarte->carteChemin->bas, ptCourant->typeDeCarte->carteChemin->gauche, ptCourant->typeDeCarte->carteChemin->droite);
        }
        else if(ptCourant->typeDeCarte->estCeObjet == 1)
        {
            fprintf(fichier, "%d\t%d\t%d\t%d\n", ptCourant->typeDeCarte->carteObjet->estCeReparation, ptCourant->typeDeCarte->carteObjet->chariot, ptCourant->typeDeCarte->carteObjet->lampe, ptCourant->typeDeCarte->carteObjet->pioche);
        }
    }
    fclose(fichier); //fermeture du fichier

}
/*********************************************************************************************************/


/**< RECUPERATION DE LA PIOCHE >**************************************************************************/
t_cartePioche * recupPioche(t_partie *maPartie)
{
    ///Déclarations
    t_cartePioche * nouveau = NULL;
    t_cartePioche * precedent = NULL;

    FILE * fichiers;
    int estCeChemin;
    int estCeObjet;
    int estCeMap;
    int estCeEboulement;

    int impasse;
    int haut;
    int bas;
    int gauche;
    int droite;

    int reparation;
    int pioche;
    int lampe;
    int chariot;

    fichiers = chargerFichier("data/fichiers/sauvegardePioche.txt", "r"); //ouverture du fichier

    while(!feof(fichiers))
    {//tant qu'on a pas atteint la fin du fichier
        ///Lecture des carte
        fscanf(fichiers, "%d", &estCeChemin);
        fscanf(fichiers, "%d", &estCeEboulement);
        fscanf(fichiers, "%d", &estCeMap);
        fscanf(fichiers, "%d", &estCeObjet);

        ///Allocation des nouvelles cartes
        nouveau = allocInitPioche();
        nouveau->typeDeCarte = allocInitCarte();

        if(estCeChemin == 1 && estCeEboulement == 0 && estCeMap == 0 && estCeObjet == 0)
        {//procedure de remplissage de la carte pour les chemins
            //alloc du t_chemin
            nouveau->typeDeCarte->carteChemin = allocInitChemin();
            nouveau->typeDeCarte->estCeChemin = 1;

            //lecture des entrées
            fscanf(fichiers, "%d", &impasse);
            fscanf(fichiers, "%d", &haut);
            fscanf(fichiers, "%d", &bas);
            fscanf(fichiers, "%d", &gauche);
            fscanf(fichiers, "%d", &droite);

            //stockage des entrées dans la structure
            nouveau->typeDeCarte->carteChemin->estCeImpasse= impasse;
            nouveau->typeDeCarte->carteChemin->haut= haut;
            nouveau->typeDeCarte->carteChemin->bas= bas;
            nouveau->typeDeCarte->carteChemin->gauche= gauche;
            nouveau->typeDeCarte->carteChemin->droite= droite;

            nouveau->typeDeCarte->illustration = recupChemin(nouveau->typeDeCarte, maPartie);
        }
        if(estCeChemin == 0 && estCeEboulement == 1 && estCeMap == 0 && estCeObjet == 0)
        {//procedure de remplissage de la carte pour les eboulements
            nouveau->typeDeCarte->estCeEboulement= 1;
            nouveau->typeDeCarte->illustration = maPartie->mesImages.couloirExplose;
        }
        if(estCeChemin == 0 && estCeEboulement == 0 && estCeMap == 1 && estCeObjet == 0)
        {//procedure de remplissage de la carte pour les carte "MAP"
            nouveau->typeDeCarte->estCeMap= 1;
            nouveau->typeDeCarte->illustration = maPartie->mesImages.carte;
        }
        if(estCeChemin == 0 && estCeEboulement == 0 && estCeMap == 0 && estCeObjet == 1)
        {//procedure de remplissage de la carte pour les objets
            //alloc du t_objet
            nouveau->typeDeCarte->estCeObjet= 1;
            nouveau->typeDeCarte->carteObjet = allocInitObjet();

            //lecture des objets
            fscanf(fichiers, "%d", &reparation);
            fscanf(fichiers, "%d", &chariot);
            fscanf(fichiers, "%d", &lampe);
            fscanf(fichiers, "%d", &pioche);

            //stockage dans la struct
            nouveau->typeDeCarte->carteObjet->estCeReparation = reparation;
            nouveau->typeDeCarte->carteObjet->pioche = pioche;
            nouveau->typeDeCarte->carteObjet->lampe = lampe;
            nouveau->typeDeCarte->carteObjet->chariot = chariot;

            nouveau->typeDeCarte->illustration = recupObjet(nouveau->typeDeCarte, maPartie);

        }

        ///Chainage
        nouveau->suivante = precedent;
        precedent = nouveau;
    }
    fclose(fichiers); //fermeture du fichier
    return nouveau; //rendu de l'ancre
}
/*********************************************************************************************************/



/**< SAUVEGARDE DE LA MAIN D'UN JOUEUR >******************************************************************/
void sauvegardeMain (t_joueur * monJoueur, FILE * fichier)
{
    ///Déclarations
    int i;

    for(i=0; i<6; i++)
    {//parcours de la main
        if(monJoueur->mainJoueur[i] != NULL)
        {//si la carte est différent de null
            //sauvegarde de l'état de la carte
            fprintf(fichier, "%d\t%d\t%d\t%d\t", monJoueur->mainJoueur[i]->estCeChemin, monJoueur->mainJoueur[i]->estCeEboulement, monJoueur->mainJoueur[i]->estCeMap, monJoueur->mainJoueur[i]->estCeObjet);
            if(monJoueur->mainJoueur[i]->estCeChemin == 1)
            {//sauvegarde spé si chemin
                fprintf(fichier, "%d\t%d\t%d\t%d\t%d\t", monJoueur->mainJoueur[i]->carteChemin->estCeImpasse, monJoueur->mainJoueur[i]->carteChemin->haut, monJoueur->mainJoueur[i]->carteChemin->bas, monJoueur->mainJoueur[i]->carteChemin->gauche, monJoueur->mainJoueur[i]->carteChemin->droite);
            }
            if(monJoueur->mainJoueur[i]->estCeObjet == 1)
            {//sauvegarde spé si objets
                fprintf(fichier, "%d\t%d\t%d\t%d\t", monJoueur->mainJoueur[i]->carteObjet->estCeReparation, monJoueur->mainJoueur[i]->carteObjet->chariot, monJoueur->mainJoueur[i]->carteObjet->lampe, monJoueur->mainJoueur[i]->carteObjet->pioche);
            }
        }
    }
}
/*********************************************************************************************************/


/**< RECUPERATION D'UN CARTE >****************************************************************************/
t_carte * recupCarte (FILE * fichiers, t_partie * maPartie)
{
    ///Déclarations
    t_carte * nouveau;

    int estCeChemin;
    int estCeObjet;
    int estCeMap;
    int estCeEboulement;

    int impasse;
    int haut;
    int bas;
    int gauche;
    int droite;

    int reparation;
    int pioche;
    int lampe;
    int chariot;

    //alloc
    nouveau = allocInitCarte();

    //lecture du type de carte
    fscanf(fichiers, "%d", &estCeChemin);
    fscanf(fichiers, "%d", &estCeEboulement);
    fscanf(fichiers, "%d", &estCeMap);
    fscanf(fichiers, "%d", &estCeObjet);

    if(estCeChemin == 1 && estCeEboulement == 0 && estCeMap == 0 && estCeObjet == 0)
    {//procedure chemin
        //alloc t_chemin
        nouveau->carteChemin = allocInitChemin();
        nouveau->estCeChemin = 1;

        //lecture des entrées
        fscanf(fichiers, "%d", &impasse);
        fscanf(fichiers, "%d", &haut);
        fscanf(fichiers, "%d", &bas);
        fscanf(fichiers, "%d", &gauche);
        fscanf(fichiers, "%d", &droite);

        //stockage des entrées
        nouveau->carteChemin->estCeImpasse= impasse;
        nouveau->carteChemin->haut= haut;
        nouveau->carteChemin->bas= bas;
        nouveau->carteChemin->gauche= gauche;
        nouveau->carteChemin->droite= droite;

        //recup de l'illustration
        nouveau->illustration = recupChemin(nouveau, maPartie);
    }

    if(estCeChemin == 0 && estCeEboulement == 1 && estCeMap == 0 && estCeObjet == 0)
    {//si eboulement
        nouveau->estCeEboulement= 1;//mise du bool à1
        nouveau->illustration = maPartie->mesImages.couloirExplose;//recup illustration
    }
    if(estCeChemin == 0 && estCeEboulement == 0 && estCeMap == 1 && estCeObjet == 0)
    {//si MAP
        nouveau->estCeMap= 1;//mise du bool à 1
        nouveau->illustration = maPartie->mesImages.carte;//recup de l'illustration
    }
    if(estCeChemin == 0 && estCeEboulement == 0 && estCeMap == 0 && estCeObjet == 1)
    {//si objet
        //alloc
        nouveau->estCeObjet= 1;
        nouveau->carteObjet = allocInitObjet();

        //lecture du type d'objet
        fscanf(fichiers, "%d", &reparation);
        fscanf(fichiers, "%d", &chariot);
        fscanf(fichiers, "%d", &lampe);
        fscanf(fichiers, "%d", &pioche);

        //stockage
        nouveau->carteObjet->estCeReparation = reparation;
        nouveau->carteObjet->pioche = pioche;
        nouveau->carteObjet->lampe = lampe;
        nouveau->carteObjet->chariot = chariot;

        //illusation
        nouveau->illustration = recupObjet(nouveau, maPartie);

    }
    return nouveau; //rendu de la nouvelle carte
}
/*********************************************************************************************************/



/**< SAUVEGARDE DES JOUEURS DE LA PARTIE >****************************************************************/
void sauvegardeJoueurs(t_partie * maPartie)
{
    ///Déclarations
    t_identite * ptCourant;
    FILE * fichier;
    int i;

    fichier = chargerFichier("data/fichiers/sauvegardeJoueurs.txt", "w");//ouverture du fichier

    ptCourant = maPartie->ancreJoueur; //initialisation du parcours
    for(i=0; i<maPartie->nbJoueur; i++)
    {//pour le nombre de joueurs

    //sauvegarde des données du joueurs
        fprintf(fichier, "%s\t", ptCourant->pseudo);
        fprintf(fichier, "%s\t", ptCourant->nomAvatar);
        fprintf(fichier, "%d\t", ptCourant->score);
        fprintf(fichier, "%d\t", ptCourant->profilJoueur->role);

        //sauvegarde objets du joueur
        fprintf(fichier, "%d\t", ptCourant->profilJoueur->objetsJoueurs.chariot);
        fprintf(fichier, "%d\t", ptCourant->profilJoueur->objetsJoueurs.lampe);
        fprintf(fichier, "%d\t", ptCourant->profilJoueur->objetsJoueurs.pioche);

        //sauvegarde de la main
        sauvegardeMain(ptCourant->profilJoueur, fichier);
        fprintf(fichier, "\n");
        //parcours
        ptCourant = ptCourant->suivant;
    }
    fclose(fichier);//fermeture du fichier
}
/*********************************************************************************************************/



/**< RECUPERATION DES JOUEURS D'UNE PARTIE >**************************************************************/
t_identite * recupJoueurs(t_partie * maPartie)
{
    ///Déclarations
    t_identite * ancre = NULL;
    t_identite * nouveau = NULL;
    t_identite * precedent = NULL;
    FILE * fichier;
    int i, j;

    fichier = chargerFichier("data/fichiers/sauvegardeJoueurs.txt", "r");//ouverture du fichier

    for(i=0; i<maPartie->nbJoueur; i++)
    {//pour le nombre de joueurs de la partie
        //alloc d'un nv joueur
        nouveau = allocInitIdentite();
        nouveau ->profilJoueur = allocInitJoueurs();

        //recuperation de son identité
        fscanf(fichier, "%s", nouveau->pseudo);
        fscanf(fichier, "%s", nouveau->nomAvatar);
        fscanf(fichier, "%d", &nouveau->score);
        fscanf(fichier, "%d", &nouveau->profilJoueur->role);
        fscanf(fichier, "%d", &nouveau->profilJoueur->objetsJoueurs.chariot);
        fscanf(fichier, "%d", &nouveau->profilJoueur->objetsJoueurs.lampe);
        fscanf(fichier, "%d", &nouveau->profilJoueur->objetsJoueurs.pioche);
        nouveau->avatar = recupAvatar(nouveau);

        for(j=0; j<6; j++)
        {//recup de sa main
            nouveau->profilJoueur->mainJoueur[j] = recupCarte(fichier, maPartie);
        }
        ///Chainage
        if(i==0)
        {
            ancre = nouveau;
        }
        else
        {
            precedent->suivant = nouveau;

        }
        precedent = nouveau;
    }

    nouveau->suivant = ancre; //liste chainée circulaire

    fclose(fichier); //fermeture du fichier
    return ancre;//rendu de l'ancre
}
/*********************************************************************************************************/




/**< SAUVEGARDE DU PLATEAU DE JEU >***********************************************************************/
void sauvegardePlateau (t_carte * plateau[PLATEAU_COLONNE][PLATEAU_LIGNE])
{
    ///déclarations
    int i, j;
    FILE * fichier;

    fichier = chargerFichier("data/fichiers/sauvegardePlateau.txt", "w");//ouverture du fichier

    //parcours du tab
    for(i=0; i<PLATEAU_COLONNE; i++)
    {//parcours des colonnes
        for(j=0; j<PLATEAU_LIGNE; j++)
        {//parcours des lignes
            if(plateau[i][j] != NULL)
            {//si la case est pleine
                //sauvegarde des entrées du chemins et impasse
                fprintf(fichier, "1\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", plateau[i][j]->carteChemin->estCeImpasse, plateau[i][j]->carteChemin->haut, plateau[i][j]->carteChemin->bas, plateau[i][j]->carteChemin->gauche, plateau[i][j]->carteChemin->droite, plateau[i][j]->myCoord.posX, plateau[i][j]->myCoord.posY);
            }//le 1 au debut indique que la case est remplie
            else
            {//si case vide on ecrit 0 sur la ligne
                fprintf(fichier, "0\n");
            }
        }
    }
    fclose(fichier);//fermeture du fichier
}
/*********************************************************************************************************/



/**< RECUPERATION DU PLATEAU DE JEU >*********************************************************************/
void recupPlateau (t_partie * maPartie)
{
    ///Déclarations
    int i, j, test;
    FILE * fichier;
    t_carte * nouveau;
    BITMAP * depart;
    BITMAP * arrivee;

    int impasse;
    int haut;
    int bas;
    int gauche;
    int droite;
    int posX;
    int posY;

    //fichiers et bitmaps
    fichier = chargerFichier("data/fichiers/sauvegardePlateau.txt", "r");
    depart = chargerImage("data/images/plateau/carteEntree.bmp");
    arrivee = chargerImage("data/images/plateau/carteArrivee.bmp");

    //parcours du fichier
    for(i=0; i<PLATEAU_COLONNE; i++)
    {
        for(j=0; j<PLATEAU_LIGNE; j++)
        {
            fscanf(fichier, "%d", &test);//lecture du 1 ou 0 en début de ligne pour savoir si case vide ou non
            if(test == 1)
            {
                //alloc
                nouveau = NULL;
                nouveau = allocInitCarte();
                nouveau->carteChemin = allocInitChemin();
                nouveau->estCeChemin = 1;

                //lecture
                fscanf(fichier, "%d", &impasse);
                fscanf(fichier, "%d", &haut);
                fscanf(fichier, "%d", &bas);
                fscanf(fichier, "%d", &gauche);
                fscanf(fichier, "%d", &droite);
                fscanf(fichier, "%d", &posX);
                fscanf(fichier, "%d", &posY);

                //stockage
                nouveau->carteChemin->estCeImpasse= impasse;
                nouveau->carteChemin->haut= haut;
                nouveau->carteChemin->bas= bas;
                nouveau->carteChemin->gauche= gauche;
                nouveau->carteChemin->droite= droite;
                nouveau->myCoord.posX = posX;
                nouveau->myCoord.posY = posY;
                //recup de l'illustration
                nouveau->illustration = recupChemin(nouveau, maPartie);

                //stockage du nv dans le lpalteau à la bonne case
                maPartie->plateau[i][j] = nouveau;
            }
            else{//si case vide, pointeur à NULL
                maPartie->plateau[i][j] =NULL;
            }
        }
    }

    //correction des illustration pour deprt et arrivée
    maPartie->plateau[1][4]->illustration = depart;
    maPartie->plateau[9][2]->illustration = arrivee;
    maPartie->plateau[9][4]->illustration = arrivee;
    maPartie->plateau[9][6]->illustration = arrivee;

    fclose(fichier);//fermeture du fichier
}
/*********************************************************************************************************/






/**< RECUPERATION DE LA BITMAP CORRESPONDANTE AU CHEMIN >*************************************************/
BITMAP *recupChemin (t_carte * maCarte, t_partie * maPartie)
{
    BITMAP * chemin;

    ///IMPASSES
    if(maCarte->carteChemin->estCeImpasse == 1 && maCarte->carteChemin->haut == 1 && maCarte->carteChemin->bas == 1 && maCarte->carteChemin->gauche == 1 && maCarte->carteChemin->droite == 1)
    {
        chemin = maPartie->mesImages.impasse_4;
    }
    else if(maCarte->carteChemin->estCeImpasse == 1 && maCarte->carteChemin->haut == 0 && maCarte->carteChemin->bas == 1 && maCarte->carteChemin->gauche == 0 && maCarte->carteChemin->droite == 0)
    {
        chemin = maPartie->mesImages.impasse_1_2;
    }
    else if(maCarte->carteChemin->estCeImpasse == 1 && maCarte->carteChemin->haut == 0 && maCarte->carteChemin->bas == 0 && maCarte->carteChemin->gauche == 1 && maCarte->carteChemin->droite == 0)
    {
        chemin = maPartie->mesImages.impasse_1_1;
    }
    else if(maCarte->carteChemin->estCeImpasse == 1 && maCarte->carteChemin->haut == 1 && maCarte->carteChemin->bas == 1 && maCarte->carteChemin->gauche == 0 && maCarte->carteChemin->droite == 0)
    {
        chemin = maPartie->mesImages.impasse_2_2;
    }
    else if(maCarte->carteChemin->estCeImpasse == 1 && maCarte->carteChemin->haut == 0 && maCarte->carteChemin->bas == 0 && maCarte->carteChemin->gauche == 1 && maCarte->carteChemin->droite == 1)
    {
        chemin = maPartie->mesImages.impasse_2_1;
    }
    else if(maCarte->carteChemin->estCeImpasse == 1 && maCarte->carteChemin->haut == 0 && maCarte->carteChemin->bas == 1 && maCarte->carteChemin->gauche == 0 && maCarte->carteChemin->droite == 1)
    {
        chemin = maPartie->mesImages.impasse_2_3;
    }
    else if(maCarte->carteChemin->estCeImpasse == 1 && maCarte->carteChemin->haut == 1 && maCarte->carteChemin->bas == 0 && maCarte->carteChemin->gauche == 0 && maCarte->carteChemin->droite == 1)
    {
        chemin = maPartie->mesImages.impasse_2_4;
    }
    else if(maCarte->carteChemin->estCeImpasse == 1 && maCarte->carteChemin->haut == 1 && maCarte->carteChemin->bas == 0 && maCarte->carteChemin->gauche == 1 && maCarte->carteChemin->droite == 1)
    {
        chemin = maPartie->mesImages.impasse_3_1;
    }
    else if(maCarte->carteChemin->estCeImpasse == 1 && maCarte->carteChemin->haut == 1 && maCarte->carteChemin->bas == 1 && maCarte->carteChemin->gauche == 1 && maCarte->carteChemin->droite == 0)
    {
        chemin = maPartie->mesImages.impasse_3_2;
    }


    ///CHEMIN X
    else if(maCarte->carteChemin->estCeImpasse == 0 && maCarte->carteChemin->haut == 1 && maCarte->carteChemin->bas == 1 && maCarte->carteChemin->gauche == 1 && maCarte->carteChemin->droite == 1)
    {
        chemin = maPartie->mesImages.cheminX;
    }


    ///CHEMIN DROIT
    else if(maCarte->carteChemin->estCeImpasse == 0 && maCarte->carteChemin->haut == 1 && maCarte->carteChemin->bas == 1 && maCarte->carteChemin->gauche == 0 && maCarte->carteChemin->droite == 0)
    {
        chemin = maPartie->mesImages.cheminDroitHaut;
    }
    else if(maCarte->carteChemin->estCeImpasse == 0 && maCarte->carteChemin->haut == 0 && maCarte->carteChemin->bas == 0 && maCarte->carteChemin->gauche == 1 && maCarte->carteChemin->droite == 1)
    {
        chemin = maPartie->mesImages.cheminDroitLong;
    }


    ///CHEMIN L
    else if(maCarte->carteChemin->estCeImpasse == 0 && maCarte->carteChemin->haut == 0 && maCarte->carteChemin->bas == 1 && maCarte->carteChemin->gauche == 0 && maCarte->carteChemin->droite == 1)
    {
        chemin = maPartie->mesImages.cheminL_1;
    }
    else if(maCarte->carteChemin->estCeImpasse == 0 && maCarte->carteChemin->haut == 1 && maCarte->carteChemin->bas == 0 && maCarte->carteChemin->gauche == 0 && maCarte->carteChemin->droite == 1)
    {
        chemin = maPartie->mesImages.cheminL_2;
    }


    ///CHEMIN T
    else if(maCarte->carteChemin->estCeImpasse == 0 && maCarte->carteChemin->haut == 1 && maCarte->carteChemin->bas == 0 && maCarte->carteChemin->gauche == 1 && maCarte->carteChemin->droite == 1)
    {
        chemin = maPartie->mesImages.cheminTHaut;
    }
    else if(maCarte->carteChemin->estCeImpasse == 0 && maCarte->carteChemin->haut == 1 && maCarte->carteChemin->bas == 1 && maCarte->carteChemin->gauche == 1 && maCarte->carteChemin->droite == 0)
    {
        chemin = maPartie->mesImages.cheminTLong;
    }

    return chemin;
}
/*********************************************************************************************************/



/**< RECUPERATION DE LA BITMAP CORRESPONDANTE A L'OBJET >*************************************************/
BITMAP * recupObjet(t_carte * maCarte, t_partie * maPartie)
{
    BITMAP * objets;

    if(maCarte->carteObjet->estCeReparation == 1 && maCarte->carteObjet->pioche == 0 && maCarte->carteObjet->chariot== 0 && maCarte->carteObjet->lampe== 1)
    {
        objets = maPartie->mesImages.lampe;
    }

    if(maCarte->carteObjet->estCeReparation == 0 && maCarte->carteObjet->pioche == 0 && maCarte->carteObjet->chariot== 0 && maCarte->carteObjet->lampe== 1)
    {
        objets = maPartie->mesImages.pasLumiere;
    }

    if(maCarte->carteObjet->estCeReparation == 1 && maCarte->carteObjet->pioche == 0 && maCarte->carteObjet->chariot== 1 && maCarte->carteObjet->lampe== 0)
    {
        objets = maPartie->mesImages.guerison;
    }
    if(maCarte->carteObjet->estCeReparation == 0 && maCarte->carteObjet->pioche == 0 && maCarte->carteObjet->chariot== 1 && maCarte->carteObjet->lampe== 0)
    {
        objets = maPartie->mesImages.blessure;
    }

    if(maCarte->carteObjet->estCeReparation == 1 && maCarte->carteObjet->pioche == 1 && maCarte->carteObjet->chariot== 0 && maCarte->carteObjet->lampe== 0)
    {
        objets = maPartie->mesImages.liberation;
    }
    if(maCarte->carteObjet->estCeReparation == 0 && maCarte->carteObjet->pioche == 1 && maCarte->carteObjet->chariot== 0 && maCarte->carteObjet->lampe== 0)
    {
        objets = maPartie->mesImages.piege;
    }


    if(maCarte->carteObjet->estCeReparation == 1 && maCarte->carteObjet->pioche == 1 && maCarte->carteObjet->chariot== 1 && maCarte->carteObjet->lampe== 0)
    {
        objets = maPartie->mesImages.guerison_liberation;
    }
    if(maCarte->carteObjet->estCeReparation == 1 && maCarte->carteObjet->pioche == 0 && maCarte->carteObjet->chariot== 1 && maCarte->carteObjet->lampe== 1)
    {
        objets = maPartie->mesImages.guerison_lampe;
    }
    if(maCarte->carteObjet->estCeReparation == 1 && maCarte->carteObjet->pioche == 1 && maCarte->carteObjet->chariot== 0 && maCarte->carteObjet->lampe== 1)
    {
        objets = maPartie->mesImages.liberation_lampe;
    }

    return objets;
}
/*********************************************************************************************************/


/**< RECUPERATION D'UN AVATAR DE JOUEUR PRECEDENT >*******************************************************/
BITMAP * recupAvatar (t_identite * monID)
{
    BITMAP * perso  = NULL;

    if(strcmp(monID->nomAvatar, "daphne")== 0)
    {
        perso = chargerImage("data/images/avatars/Daphne_Blake.bmp");
    }
    if(strcmp(monID->nomAvatar, "mystery")== 0)
    {
        perso = chargerImage("data/images/avatars/Mystery_Machine.bmp");
    }
    if(strcmp(monID->nomAvatar, "scooby")== 0)
    {
        perso = chargerImage("data/images/avatars/Scooby_Doo.bmp");
    }
    if(strcmp(monID->nomAvatar, "sammy")== 0)
    {
        perso = chargerImage("data/images/avatars/Shaggy_Rogers.bmp");
    }
    if(strcmp(monID->nomAvatar, "vera")== 0)
    {
        perso = chargerImage("data/images/avatars/Velma_Dinkley.bmp");
    }
    if(strcmp(monID->nomAvatar, "fred")== 0)
    {
        perso = chargerImage("data/images/avatars/Fred_Jones.bmp");
    }

    return perso;
}
/*********************************************************************************************************/
