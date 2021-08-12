#include "header.h"


/**< GESTION DU TOUR D'UN JOUEUR >************************************************************************/
int tourJoueur (t_partie * maPartie, t_identite * monJoueur, BITMAP * doublebuffer, BITMAP * buffer)
{
    ///Déclarations
    int i, action =0;
    int resultat = 0;
    int sortie = 0;
    char phrase[100];
    int nbDefausse = 0;
    int placement = 0;
    int passer =0;


    ///TRI MAIN JOUEUR
    triMain(monJoueur);
    affichageDebutTour(doublebuffer, buffer, maPartie, monJoueur);

    while(action == 0 && sortie == 0)  //Début de la boucle pour le tour complet d'un joueur
    {
        //tant que le joueur n'a pas fait d'actions

        for(i=0; i<6; i++)
        {
            strcpy(phrase, "0");
            if(monJoueur->profilJoueur->mainJoueur[i] != NULL)
            {
                //parcours de toutes les cartes de la main

                if(appuieCarte(monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX, monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY, monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX+CARTE_PIOCHE_W, monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY+CARTE_PIOCHE_H) == 1)
                {
                    //si on clique sur une carte de la main

                    dragAndDrop(i, maPartie, monJoueur); //drag and drop de la carte


                    if(testPositionCarte(monJoueur->profilJoueur->mainJoueur[i], DEFAUSSE_X, DEFAUSSE_X+CARTE_PIOCHE_W, DEFAUSSE_Y, DEFAUSSE_Y+CARTE_PIOCHE_H)==1)
                    {                                                                   // si la carte est defaussee
                        nbDefausse ++; //le joueur a defauuser une carte
                        placement = 1;          //on affirme qu'une carte a bien été défaussée
                        action = utiliserDefause(nbDefausse, i, monJoueur);
                        affichageTour(doublebuffer, buffer, maPartie, monJoueur);

                        rectfill(screen, 232, 0, SCREEN_W, 20, makecol(0, 0, 180));
                        textprintf_ex(screen, font, 280, 10, makecol(255, 255, 255),-1, "Vous pouvez defausser jusqu'a 3 cartes : %d/3", nbDefausse);   //affichage d'un message de confirmation
                    }
                    if(nbDefausse == 0)                                             // si aucune carte n'a ete defausée  //a chaque fois on appelle le sous programme concerne par le type de carte
                    {
                        if(monJoueur->profilJoueur->mainJoueur[i]->estCeObjet == 1)             //la carte est un objet
                        {
                            action = utiliserObjet(maPartie, monJoueur, phrase, i);
                        }
                        else if(monJoueur->profilJoueur->mainJoueur[i]->estCeChemin== 1)        //la carte est un chemin
                        {
                            action = placerChemin(maPartie, monJoueur, phrase, i);
                        }
                        else if(monJoueur->profilJoueur->mainJoueur[i]->estCeEboulement == 1)       //la carte est une carte eboulement
                        {
                            action = utiliserEboulement(maPartie, monJoueur, phrase, i);
                        }
                        else if(monJoueur->profilJoueur->mainJoueur[i]->estCeMap == 1)              //la carte est une map
                        {
                            action = utiliserCarte(doublebuffer, buffer, maPartie, monJoueur, i);
                        }
                    }


                    if(action == 0  && placement == 0)      // si aucune carte n'a ete placee : càd elle n'a pas ete detecte a un bon endroit pour son type ou elle n'a pas le droit d'y etre
                    {
                        retourPosInit(monJoueur, maPartie, i, phrase);              //elle retourne a sa position initiale et on affiche avec phrase un message d'erreur
                    }
                    else                // si elle a bien été placée
                    {
                        placement = 0;  //on reinitialise la variable placement

                        monJoueur->profilJoueur->mainJoueur[i] = piocherCarte(monJoueur->profilJoueur->mainJoueur[i],maPartie, i);    //on remplace la carte placce par une carte de la pioche
                        i = 7;          //on sort de la boucle for
                    }
                }
            }
        }
        if(appuieBouton(1638, 1036, 1912, 1070) == 1)           //bouton pour passer son tour
        {
            //si le joueur passe son tour
            action =1;
            passer =1;
        }

        if(key[KEY_P])                      //action de sauvegarde rapide
        {
            while(key[KEY_P])
            {rest(5);//tempo
            }
            sauvegardeTout(maPartie);
            rectfill(screen, 232, 0, SCREEN_W, 20, makecol(0, 180, 0));
            textout_ex(screen, font, "sauvegarde effectuee",280, 10, makecol(255, 255, 255),-1);   //affichage d'un message de confirmation
        }

        if(key[KEY_ESC])            //menu de jeu , 3 possibilites : sauvegarder, menu , reprendre la partie
        {
            while(key[KEY_ESC])
            {rest(5);//tempo
            }
            clear_bitmap(screen);
            show_mouse(NULL);
            pause(doublebuffer, buffer, maPartie, monJoueur, &sortie, &resultat);
        }
    }


    if(resultat != 1)
    {
        affichageDebutTour(doublebuffer, buffer, maPartie, monJoueur);    // si on ne quitte pas la partie
    }

    verifPossibiliteJeu(maPartie, monJoueur, passer);

    rest(100);

    return resultat;
}
/*********************************************************************************************************/

/**< UTILISATION DE LA DEFAUSSE >************************************************************************/
int utiliserDefause(int nbDefausse, int i, t_identite * monJoueur)
{
    int action = 0;

    monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX = DEFAUSSE_X;  //on place bien la carte sur la defausse
    monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY = DEFAUSSE_Y;

    if(nbDefausse == 3)   // 3 cartes sont defaussees : fin du tour
    {
        action = 1;
    }
    return action ;
}
/*********************************************************************************************************/


/**< UTILISATION D'UN EBOULEMENT >************************************************************************/
int utiliserEboulement(t_partie * maPartie, t_identite * monJoueur, char phrase [100], int i)
{
    int j, k, action = 0;
    //si le joueur veut detruire un chemin
    for(j = 0; j<PLATEAU_COLONNE; j++)
    {
        for(k=0; k<PLATEAU_LIGNE; k++)
        {
            if((maPartie->plateau[j][k] != NULL) )
            {
                if(j ==1 && k== 4 )
                {
                    strcpy(phrase, "Impossible d'exploser la case de depart");
                }
                else if( j == 9 && (k== 2 || k==4 || k== 6))
                {
                    strcpy(phrase, "Impossible d'exploser cette carte");
                }

                else if(testPositionCarte(monJoueur->profilJoueur->mainJoueur[i], (CARTE_DEPART_X + (j-1)*CASE_PLATEAU_W),(CARTE_DEPART_X + j*CASE_PLATEAU_W), (CARTE_DEPART_Y + (k-4)*CASE_PLATEAU_H), (CARTE_DEPART_Y + (k-3)*CASE_PLATEAU_H))==1)
                {
                    //maPartie->plateau[j][k]->carteChemin->estCeImpasse = 1;
                    //maPartie->plateau[j][k]->illustration = recupChemin(maPartie->plateau[j][k], maPartie);

                    maPartie->plateau[j][k] = NULL;
                    animEboulement(maPartie, monJoueur, i);
                    action = 1;

                    j = PLATEAU_COLONNE + 1;
                    k = PLATEAU_LIGNE + 1;
                }
            }
        }
    }

    return action;
}
/*********************************************************************************************************/



/**< UTLISATION D'UNE CARTE MAP>***************************************************************************/
int utiliserCarte(BITMAP * doublebuffer, BITMAP * buffer, t_partie * maPartie, t_identite * monJoueur, int i)
{
    BITMAP * arrivee = NULL;
    int action = 0;
    //si le joueur veut joueur une carte revelation d'arrivée
    //on test un clique sur une des trois cartes
    if(testPositionCarte(monJoueur->profilJoueur->mainJoueur[i],CARTE_ARRIVEE_X_MIN, CARTE_ARRIVEE_X_MAX, CARTE_ARRIVEE_Y_1, CARTE_ARRIVEE_Y_1+CASE_PLATEAU_H) == 1)
    {
        if(maPartie->plateau[9][2]->carteChemin->estCeImpasse == 1)     //si la carte est une impasse (sans pepites)
        {
            arrivee = chargerImage("data/images/plateau/arrivee_0.bmp");        //on affiche l'image correspondante
        }
        else                                                                    //si c'est la carte avec l'arrivée
        {
            arrivee = chargerImage("data/images/plateau/arrivee_1.bmp");            //on affiche l'image correspondante
        }
        monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX = DEFAUSSE_X;
        monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY = DEFAUSSE_Y;
        affichageTour(doublebuffer, buffer, maPartie, monJoueur);
        stretch_sprite(screen, arrivee,CARTE_DEPART_X + 8 * CASE_PLATEAU_W,CARTE_DEPART_Y - 2 * CASE_PLATEAU_H, CARTE_PLATEAU_W, CARTE_PLATEAU_H);
        sleep(2);
        action =1;                                  //bouleen pour sortir de la boucle car une action a et eeffectuee
        destroy_bitmap(arrivee);
    }
    if(testPositionCarte(monJoueur->profilJoueur->mainJoueur[i],CARTE_ARRIVEE_X_MIN, CARTE_ARRIVEE_X_MAX, CARTE_ARRIVEE_Y_2, CARTE_ARRIVEE_Y_2+CASE_PLATEAU_H) == 1)
    {
        if(maPartie->plateau[9][4]->carteChemin->estCeImpasse == 1)
        {
            arrivee = chargerImage("data/images/plateau/arrivee_0.bmp");
        }
        else
        {
            arrivee = chargerImage("data/images/plateau/arrivee_1.bmp");
        }
        monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX = DEFAUSSE_X;
        monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY = DEFAUSSE_Y;
        affichageTour(doublebuffer, buffer, maPartie, monJoueur);
        stretch_sprite(screen, arrivee,CARTE_DEPART_X + 8 * CASE_PLATEAU_W,CARTE_DEPART_Y, CARTE_PLATEAU_W, CARTE_PLATEAU_H);
        sleep(2);
        action =1;
        destroy_bitmap(arrivee);

    }
    if(testPositionCarte(monJoueur->profilJoueur->mainJoueur[i],CARTE_ARRIVEE_X_MIN, CARTE_ARRIVEE_X_MAX, CARTE_ARRIVEE_Y_3, CARTE_ARRIVEE_Y_3+CASE_PLATEAU_H) == 1)
    {
        if(maPartie->plateau[9][6]->carteChemin->estCeImpasse == 1)
        {
            arrivee = chargerImage("data/images/plateau/arrivee_0.bmp");
        }
        else
        {
            arrivee = chargerImage("data/images/plateau/arrivee_1.bmp");
        }
        monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX = DEFAUSSE_X;
        monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY = DEFAUSSE_Y;
        affichageTour(doublebuffer, buffer, maPartie, monJoueur);
        stretch_sprite(screen, arrivee,CARTE_DEPART_X + 8 * CASE_PLATEAU_W,CARTE_DEPART_Y + 2 * CASE_PLATEAU_H, CARTE_PLATEAU_W, CARTE_PLATEAU_H);
        sleep(2);
        action =1;
        destroy_bitmap(arrivee);
    }

    return action;
}
/*********************************************************************************************************/



/**< PLACER UNE CARTE CHEMIN >****************************************************************************/
int placerChemin (t_partie * maPartie, t_identite * monJoueur, char phrase [100], int i)
{
    //si le joueur veut placer un chemin
    int action = 0;
    int j, k;
    if(verifObjets(monJoueur->profilJoueur) == 1)  // on check qu'il n'a pas de malus sinon il ne peut pas jouer
    {
        for(j = 0; j<PLATEAU_COLONNE; j++)    //on test pour toutes les cases de tableau grace aux deux boucles for
        {
            for(k=0; k<PLATEAU_LIGNE; k++)
            {
                if(testPositionCarte(monJoueur->profilJoueur->mainJoueur[i], (CARTE_DEPART_X + (j-1)*CASE_PLATEAU_W),(CARTE_DEPART_X + j*CASE_PLATEAU_W), (CARTE_DEPART_Y + (k-4)*CASE_PLATEAU_H), (CARTE_DEPART_Y + (k-3)*CASE_PLATEAU_H))==1)
                {                                                               //si la carte est bien placee dans une case du tableau

                    if(maPartie->plateau[j][k] == NULL)       //on verifie que celle-ci est bien nulle
                    {
                        if(verfiConnexionChemin(j, k, maPartie->plateau, monJoueur->profilJoueur->mainJoueur[i]) == 1)   //on appelle un SP qui verifie que la carte est bien en lien avec une autre du plateau et que les connexions se font bien
                        {           // si toutes les conditions sont remplies
                            maPartie->plateau[j][k] = monJoueur->profilJoueur->mainJoueur[i];   //On passe l'adresse de la structure carte au plateau
                            maPartie->plateau[j][k]->myCoord.posX = (CARTE_DEPART_X + (j-1)*CASE_PLATEAU_W);  //on place bien la carte en initialisant ses coordonnées
                            maPartie->plateau[j][k]->myCoord.posY = (CARTE_DEPART_Y + (k-4)*CASE_PLATEAU_H);

                            action = 1;  //on affirme qu'une action a bien été effectuée
                        }
                        else
                        {
                            strcpy(phrase, "Le chemin n'est pas connecte aux autres chemins");   //sinon on ecrit le message d'erreur correspondant
                        }
                    }
                    else
                    {
                        strcpy(phrase, "La case est deja occupee");
                    }
                }
            }
        }
    }
    else
    {
        strcpy(phrase, "Vos malus vous empechent de poser un objet");
    }
    return action;
}
/*********************************************************************************************************/



/**< UTILISER UN OBJET >**********************************************************************************/
int utiliserObjet(t_partie * maPartie, t_identite * monJoueur, char phrase [100], int i)
{
    t_identite * ptCourant;
    int  j;
    int action =0;


    //si le joueur joue une carte objet
    ptCourant = monJoueur;

    //joueur qui joue;
    if(testPositionCarte(monJoueur->profilJoueur->mainJoueur[i], 210, 510, 810, 1080 ) == 1)
    {
        //on test si la carte est placée sur un joueur du plateau
        if(monJoueur->profilJoueur->mainJoueur[i]->carteObjet->estCeReparation == 1)
        {
            //si c'est une carte réparation
            action = reparerObjet(monJoueur->profilJoueur->mainJoueur[i]->carteObjet, monJoueur); //on répare l'objet en question
            if(action != 1)
            {
                strcpy(phrase, "Le joueur vise ne possede pas le malus correspondant");
            }
        }

    }


    for(j=1; j<maPartie->nbJoueur; j++)
    {
        //on parcours le nombre de joueurs

        ptCourant = ptCourant->suivant;//parcours de la liste

        if(testPositionCarte(monJoueur->profilJoueur->mainJoueur[i], 0, 201, 1080-j*160, 1080-(j-1)*160) == 1)
        {
            //on test si la carte est placée sur un joueur du plateau
            if(monJoueur->profilJoueur->mainJoueur[i]->carteObjet->estCeReparation == 1)
            {
                //si c'est une carte réparatio
                action = reparerObjet(monJoueur->profilJoueur->mainJoueur[i]->carteObjet, ptCourant); //on répare l'objet en question
                if(action != 1)
                {
                    strcpy(phrase, "Le joueur vise ne possede pas le malus correspondant");
                }
            }
            else
            {
                //si c'est une carte pour casser un objet
                action = casserObjet(monJoueur->profilJoueur->mainJoueur[i]->carteObjet, ptCourant, maPartie); //on casse l'objet pour le joueur
                if(action != 1)
                {
                    strcpy(phrase, "Le joueur vise possede deja un malus identique");
                }
            }
        }
    }
    ptCourant = NULL;
    return action;
}

/*********************************************************************************************************/



/**< AFFICHAGE DE LA PARTIE EN DEBUT DE TOUR >************************************************************/
void affichageDebutTour (BITMAP * doublebuffer, BITMAP * buffer, t_partie * maPartie, t_identite * monJoueur)
{
    //nettoyage du buffer de partie
    show_mouse(NULL);
    clear_bitmap(maPartie->buffer);

    chargerPlateau(maPartie);//affichage plateau

    affichageJoueursPlateau(maPartie, monJoueur); //affichage des joueurs

    clear_bitmap(doublebuffer); //nettoyage buffer partie

    blit(maPartie->buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage duplateau sauvegardé

    clear_to_color(buffer, makecol(255, 0, 255));
    afficherMainJoueur(maPartie, buffer, monJoueur);  //affichage de la main du joueur
    masked_blit(buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    masked_blit(doublebuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage sur le screen

    show_mouse(screen);
}
/*********************************************************************************************************/



/**< AFFICHER LA PARTIE PENDANT LE TOUR >*****************************************************************/
void affichageTour (BITMAP * doublebuffer, BITMAP * buffer, t_partie * maPartie,  t_identite * monJoueur)
{
    show_mouse(NULL);
    clear_bitmap(doublebuffer); //nettoyage buffer partie

    blit(maPartie->buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage duplateau sauvegardé

    clear_to_color(buffer, makecol(255, 0, 255));

    afficherMainJoueur(maPartie, buffer, monJoueur);  //affichage de la main du joueur
    masked_blit(buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    masked_blit(doublebuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage sur le screen

    show_mouse(screen);
}
/*********************************************************************************************************/



/**< MENU PAUSE >*****************************************************************************************/
void pause (BITMAP * doublebuffer, BITMAP * buffer, t_partie * maPartie, t_identite * monJoueur, int * sortie, int * resultat)
{
    int outPause = 0;
    BITMAP * quitter;
    BITMAP * clavier;

    quitter = chargerImage("data/images/menu/quitter.bmp");
    clavier = chargerImage("data/images/menu/clavier.bmp");

    stretch_blit(quitter, screen, 0, 0, quitter->w, quitter->h, 0, 0, SCREEN_W, SCREEN_H);

    show_mouse(screen);
        while(outPause == 0)   // tant qu'aucun choix n'a été fait
        {

            if(appuieBouton(152, 541, 890, 732) == 1)     //bouton retour à la partie
            {
                outPause = 1;       //on sort du sp
                affichageTour(doublebuffer, buffer, maPartie, monJoueur);   // on reaffiche le tour du joueur
            }

            if(appuieBouton(1030, 541, 1767, 736) == 1)   //bouton de sauvegarde
            {
                sauvegardeTout(maPartie);     // on sauvegarde et on attend que le joueur fasse un autre choix
                rectfill(screen, 0, 0, SCREEN_W, 20, makecol(0, 180, 0));
                textout_ex(screen, font, "sauvegarde effectuee",800, 10, makecol(255, 255, 255),-1);
            }
            if(appuieBouton(529, 835, 1392, 1027) == 1)   //bouton pour quitter la partie
            {
                *sortie = 1;    //on sort de toutes les boucles jusquà revenir au menu
                *resultat = 1;
                outPause = 1;
            }
            if(appuieBouton(1768, 27, 1899, 121) == 1)
            {
                wiki();
                stretch_blit(quitter, screen, 0, 0, quitter->w, quitter->h, 0, 0, SCREEN_W, SCREEN_H);
            }
            if(appuieBouton(1617, 27, 1732, 121) == 1)
            {
                stretch_blit(clavier, screen, 0, 0, quitter->w, quitter->h, 0, 0, SCREEN_W, SCREEN_H);
                while(mouse_b != 1)//attente du click
                {rest(5);
                }//tempo
                while(mouse_b ==1)//attente du relachement du clique
                {rest(5);
                }
                stretch_blit(quitter, screen, 0, 0, quitter->w, quitter->h, 0, 0, SCREEN_W, SCREEN_H);

            }
        }

    destroy_bitmap(clavier);
    destroy_bitmap(quitter);
}
/*********************************************************************************************************/



/**< TRI DE LA MAIN >*************************************************************************************/
void triMain (t_identite * monJoueur)
{
    t_carte * tempsCarte = NULL;

    int i, j;
    for(i=0; i<6; i++)   //on effecture un tri à bulle revisité puisque ce ne sont pas des nombres mais juste deux types de cartes : les cartes chemin et les autres
    {
        for(j=i+1; j<6; j++)
        {
            if(monJoueur->profilJoueur->mainJoueur[j] != NULL && monJoueur->profilJoueur->mainJoueur[i] != NULL )  //on ne peut comparer que si les deux cartes a comparer existent
            {
                if(monJoueur->profilJoueur->mainJoueur[j]->estCeChemin == 1)   // si la seconde (donc celle de droites est un carte chemin
                {
                    tempsCarte = monJoueur->profilJoueur->mainJoueur[j];   //on l'echange avec celle de gauche
                    monJoueur->profilJoueur->mainJoueur[j] = monJoueur->profilJoueur->mainJoueur[i];
                    monJoueur->profilJoueur->mainJoueur[i] = tempsCarte;
                }   //car les chemins sont à gauche et les cartes action à droite
            }
        }
    }
    for(i=0; i<6; i++)
    {
        if(monJoueur->profilJoueur->mainJoueur[i] != NULL)   //on initialise ensuite les coordonnes de chaque carte selon leur place dans le nouveau tableau trié
        {
            //initialisation des position des cartes dans la main
            monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX = CARTE_MAIN_X+i*220;
            monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY= CARTE_MAIN_Y;
        }
    }

}
/*********************************************************************************************************/

/**< DEFAUSSE D'UNE CARTE >*******************************************************************************/
t_carte * piocherCarte (t_carte * maCarte, t_partie *maPartie, int nombre)
{
    if(maPartie->ancrePioche != NULL)   // si la pioche n'est pas vide
    {
        maCarte = NULL;         //on initialise l'ancienne carte à nulle
        maCarte = maPartie->ancrePioche->typeDeCarte; //on pioche une nouvelle carte

        maPartie->ancrePioche = maPartie->ancrePioche->suivante; //on actualise la pioche

        //nouvelle coordonnées de la carte
        maCarte->myCoord.posX = CARTE_MAIN_X+nombre*220;
        maCarte->myCoord.posY = CARTE_MAIN_Y;
    }
    else   //si la pioche est nulle
    {
        maCarte = NULL;   //on met le pointeur sur la nouvelle carte à nulle
    }

    return maCarte;
}
/*********************************************************************************************************/


/**< CASSER L'OBJET D'UN JOUEUR >*************************************************************************/
int casserObjet (t_objet * monObjet, t_identite * monJoueur, t_partie * maPartie)
{
    int validation = 0;
    //pour casser un objet, il suffit de verifier que la carte est bien un carte avec le bon malus et que l'objet en question n'est pas deja cassé
    if(monObjet->chariot== 1 && monJoueur->profilJoueur->objetsJoueurs.chariot==1)
    {
        //on casse un chariot
        monJoueur->profilJoueur->objetsJoueurs.chariot = 0;
        animBlessure(maPartie, monJoueur);
        validation = 1;
    }

    if(monObjet->lampe== 1 && monJoueur->profilJoueur->objetsJoueurs.lampe==1)
    {
        //on casse une lampe
        monJoueur->profilJoueur->objetsJoueurs.lampe= 0;
        animLumiere(maPartie, monJoueur);
        validation = 1;
    }

    if(monObjet->pioche== 1  && monJoueur->profilJoueur->objetsJoueurs.pioche==1)
    {
        //on casse une pioche
        monJoueur->profilJoueur->objetsJoueurs.pioche= 0;
        animPiege(maPartie, monJoueur);
        validation = 1;
    }

    return validation;
}
/*********************************************************************************************************/



/**< REPARER L'OBJET D'UN JOUEUR >************************************************************************/
int reparerObjet (t_objet * monObjet, t_identite * monJoueur)
{
    int validation = 0;
    // pour repare un objet on verifie que on a la bonne carte de reparation et que l'objet en question est bien cassé
    if(monObjet->chariot== 1 && monJoueur->profilJoueur->objetsJoueurs.chariot==0)
    {
        //on repare un chariot
        monJoueur->profilJoueur->objetsJoueurs.chariot = 1;
        validation = 1;
    }

    if(monObjet->lampe== 1 && monJoueur->profilJoueur->objetsJoueurs.lampe==0)
    {
        //on repare une lampe
        monJoueur->profilJoueur->objetsJoueurs.lampe= 1;
        validation = 1;
    }

    if(monObjet->pioche== 1 && monJoueur->profilJoueur->objetsJoueurs.pioche==0)
    {
        //on repare une pioche
        monJoueur->profilJoueur->objetsJoueurs.pioche= 1;
        validation = 1;
    }

    return validation;
}
/*********************************************************************************************************/
