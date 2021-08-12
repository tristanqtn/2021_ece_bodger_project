#include "header.h"

/**< Début d'une partie >*******************************************************************************************************/
t_partie * debutPartie()
{
    ///Déclarations
    t_partie * newGame;                             //Création d'une structure partie
    t_identite * courant;

    int i, j, whoIsSaboteur;

    newGame = (t_partie*)malloc(sizeof(t_partie));      //allocation de la structure

    if(newGame == NULL)                                     //blindage de l'allocation
    {
        sortieErreur("Erreur d'allocation de la structure partie");
    }

    newGame->mesImages = chargerTabImage();

    newGame->ancrePioche = creationListeCarte(newGame);              //appel du sous programme de conception de la pioche

    newGame->nbJoueur = nombreJoueur();                     //Appel du sous-programme qui détermine le nombre de joueurs
    newGame->ancreJoueur = listeProfils(newGame->nbJoueur);

    courant = newGame->ancreJoueur;

    whoIsSaboteur = rand() % newGame->nbJoueur;

    for(i=0; i<newGame->nbJoueur; i++)  //Attribution des mains des joueurs et de leur rôle
    {
        if(i == whoIsSaboteur)
        {
            courant->profilJoueur->role = 1; //il est saboteur
        }
        for(j=0; j<NBMAIN; j++)
        {//remplir la main des joueurs
            courant->profilJoueur->mainJoueur[j] = piocherCarte(courant->profilJoueur->mainJoueur[j], newGame, newGame->nbJoueur);
        }
        courant = courant->suivant;
    }

    newGame->buffer = creerBuffer(SCREEN_W, SCREEN_H, "buffer"); //creation de bitmap de partie

    initialisationPlateau(newGame); //initialisation du plateau
    chargerPlateau(newGame);        //chargement di plateau
    clear_bitmap(newGame->buffer);  //nettoyage preventif du buffer de partie

    return newGame;
}
/*********************************************************************************************************/


/**< Rechargement d'une partie sauvegardée >**************************************************************/
t_partie * jouerPartieSauvegardee ()
{
    ///Déclarations
    t_partie * maPartie;

    ///Alloc de la partie sauvegardée
    maPartie = (t_partie *) malloc (sizeof(t_partie));

    if(maPartie == NULL)
    {
        sortieErreur("echec alloc partie");
    }
    else
    {
        maPartie->nbJoueur = recupNBJoueur();   //recup du nb de joueurs
        maPartie->mesImages = chargerTabImage();    //recup des images de jeu
        maPartie->ancrePioche = recupPioche(maPartie);  //recup de la pioche
        maPartie->buffer = creerBuffer(SCREEN_W, SCREEN_H, "buffer partie"); //creation du buffer de partie
        maPartie->ancreJoueur = recupJoueurs(maPartie); //recup des joueurs
        initialisationPlateau(maPartie);    //init du plateau de jeu
        recupPlateau(maPartie);//recup du plateau
    }

    return maPartie; //rendu de la partie
}
/*********************************************************************************************************/

/**< Révélation des rôles des joueurs >*******************************************************************/
void animationRole(t_partie * maPartie)
{
    int i, revelation= 0;
    t_identite * courant = maPartie->ancreJoueur;  //initialisation du pointeur courant
    //chargement des BITMAPS
    BITMAP * fond = chargerImage("data/images/menu/revelationRole.bmp");
    BITMAP * saboteur = chargerImage("data/images/menu/SaboteurCarte.bmp");
    BITMAP * associe = chargerImage("data/images/menu/AssocieCarte.bmp");
    BITMAP * buffer = creerBuffer(SCREEN_W, SCREEN_H, "buffer revelation joueurs");
    int x, y, tab[10];
    FONT * pixelated28;        //POLICE
    pixelated28 = chargerPolice("data/police/pixelated_28.pcx");   //chargement de la police d'écriture

    stretch_blit(fond, buffer, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

    x = 1500/maPartie->nbJoueur ;   // position des cartes selon le nombre de joeurs (pour que ca soit centré)
    y  = 100;


    for(i=0; i<maPartie->nbJoueur; i++)   //Chargement des images carte pour chaque joueur
    {
        tab[i] = 0;
        if(strcmp("mystery", courant->nomAvatar) == 0 )
        {
            courant->carte = chargerImage("data/images/menu/MysteryCarte.bmp");
        }
        else if(strcmp("daphne", courant->nomAvatar)== 0 )
        {
            courant->carte = chargerImage("data/images/menu/DaphneCarte.bmp");
        }
        else if(strcmp("scooby", courant->nomAvatar) == 0)
        {
            courant->carte = chargerImage("data/images/menu/ScoobyCarte.bmp");
        }
        else if(strcmp("sammy", courant->nomAvatar) == 0)
        {
            courant->carte = chargerImage("data/images/menu/SamyCarte.bmp");
        }
        else if(strcmp("vera", courant->nomAvatar) == 0)
        {
            courant->carte = chargerImage("data/images/menu/VeraCarte.bmp");
        }
        else if(strcmp("fred", courant->nomAvatar) == 0)
        {
            courant->carte = chargerImage("data/images/menu/FredCarte.bmp");
        }

        stretch_sprite(buffer, courant->carte, x*(1+i)-100, y, courant->carte->w, courant->carte->h);  //Actualisation du buffer
        textprintf_ex(buffer, pixelated28, x*(1+i)-40, y + 10, makecol(255, 255, 255), -1, "%s", courant->pseudo );   //affichage du nom du joueur sur la carte

        courant = courant->suivant;
    }
    show_mouse(NULL);
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );                           //affichage du buffer sur le screen
    show_mouse(screen);

    courant = maPartie->ancreJoueur;                                                 //reinitialisation du pointeur de structure courant

    while(revelation!= maPartie->nbJoueur && !key[KEY_ENTER])                       //Tant que tous les roles n'ont pas étés decouverts
    {
        for(i=0; i<maPartie->nbJoueur; i++)                                         //on parcourt les cartes de chaque joueurs
        {


            if(appuieBouton(x*(1+i) - 100, y, x*(1+i) - 100 + courant->carte->w, y+courant->carte->h) == 1)   //si un clique a ete detecte sur une carte
            {
                //revelation role
                if(courant->profilJoueur->role == 1)  // Si c'est un saboteur
                {
                    stretch_sprite(screen, saboteur, x*(1+i)-100, y, courant->carte->w, courant->carte->h);       //affichage sur le buffer
                }
                else // si c'est un associe
                {
                    stretch_sprite(screen, associe, x*(1+i)-100, y, courant->carte->w, courant->carte->h);       //affichage sur le buffer
                }

                                     //affichage du buffer sur le screen

                while(appuieBouton(0, 0, SCREEN_W, SCREEN_H)!= 1)    //Cette fonction permet egalement d'eviter la repetitition d'un clique de la souris à cause d'un appui trop long
                {
                    rest(5);
                }
                //reaffichage de la carte retournee sur le screen
                blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H );                           //affichage du buffer sur le screen

                if(tab[i]== 0)  //evite d'incrementer le compteur revelation si on clique plusieurs fois sur la meme carte
                {
                    tab[i]= 1;
                    revelation ++;
                }

            }
            courant = courant->suivant;  // incrémentatop,
        }
    }


}
/*********************************************************************************************************/
