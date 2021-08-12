#include "header.h"

/**< CHAGRER LE PLATEAU DE JEU >**************************************************************************/
void chargerPlateau (t_partie * maPartie)
{
    //Déclarations
    int i, j;
    BITMAP * monPlateau;
    BITMAP * fleche;
    BITMAP * dosCarte;

    //chargement des data
    monPlateau = chargerImage("data/images/plateau/plateau.bmp"); //chargement du plateau
    fleche = chargerImage("data/images/plateau/flechePlateau.bmp");  //chargement des flèches : sens de la file d'attente
    show_mouse(NULL);

    //affichage ajusté du plateau
    stretch_blit(monPlateau, maPartie->buffer, 0, 0, monPlateau->w, monPlateau->h, 0, 0, SCREEN_W, SCREEN_H);

    dosCarte = chargerImage("data/images/plateau/dos.bmp");//chargement du dos de cartes

    stretch_sprite(maPartie->buffer, dosCarte, PIOCHE_X, PIOCHE_Y, CARTE_PIOCHE_W, CARTE_PIOCHE_H); //affichage du dos de carte sur la pioche

    for(i=2; i<maPartie->nbJoueur; i++)
    {//affichage du nombre de flèches addapté au nb de joueurs
        stretch_sprite(maPartie->buffer, fleche, 100, 1220-160*i, fleche->w/2, fleche->h/2 );
    }

    for(i=0; i<PLATEAU_COLONNE; i++)
    {
        for(j=0; j<PLATEAU_LIGNE; j++)
        {
            if(maPartie->plateau[i][j] != NULL)
            {
                 stretch_sprite(maPartie->buffer, maPartie->plateau[i][j]->illustration, maPartie->plateau[i][j]->myCoord.posX, maPartie->plateau[i][j]->myCoord.posY, CARTE_PLATEAU_W, CARTE_PLATEAU_H);
            }
        }
    }
    destroy_bitmap(monPlateau);
    destroy_bitmap(fleche);
    destroy_bitmap(dosCarte);
}
/*********************************************************************************************************/



/**< INITIALISATION DU PLATEAU DE JEU >*******************************************************************/
void initialisationPlateau (t_partie * maPartie)
{
    //déclarations
    t_carte * depart;
    t_carte * arrivee [3];
    int i, j, alea;

    //initialisation de toute les cases du plateau à NULL
    for(i=0; i<PLATEAU_COLONNE; i++)
    {
        for(j=0; j<PLATEAU_LIGNE; j++)
        {
            maPartie->plateau[i][j] = NULL;
        }
    }

    //alloc d'un carte depart
    depart = allocInitCarte();  //alloc d'un t_carte
    depart->estCeChemin =1;     //c'est une chemin
    depart->carteChemin = allocInitChemin(); //alloc du t_chemin

    depart->carteChemin->bas =1;    //entrée en bas
    depart->carteChemin->haut =1;   //entrée en haut
    depart->carteChemin->gauche =1; //entrée à gauche
    depart->carteChemin->droite =1; //entrée à  droite

    depart->illustration = chargerImage("data/images/plateau/carteEntree.bmp"); //chargement du bitmap

    //init des coordonées
    depart->myCoord.posX = CARTE_DEPART_X;
    depart->myCoord.posY = CARTE_DEPART_Y;

    maPartie->plateau[1][4] = depart; //stockage du depart dans le plateau de jeu

    //création des 3 arrivées
    for(i=0; i<3; i++)
    {
        arrivee[i] = allocInitCarte(); //alloc de la carte
        arrivee[i]->carteChemin = allocInitChemin(); //alloc du chemin

        arrivee[i]->estCeChemin =1; //c'est un chemin
        arrivee[i]->carteChemin->bas =1;//entrée en bas
        arrivee[i]->carteChemin->haut =1;//entrée en haut
        arrivee[i]->carteChemin->gauche =1;//entrée à gauche
        arrivee[i]->carteChemin->droite =1;//entrée à droite

        arrivee[i]->illustration = chargerImage("data/images/plateau/carteArrivee.bmp"); //chargement du bitmap


        if(i>0)
        {//2 chemin sont des impasses donc des mauvaises arrivées
            arrivee[i]->carteChemin->estCeImpasse =1;
        }
    }

        //tirage au sort des arrivées du plateau
    for(i=0; i<3; i++)
    {
        do
        {
            alea = rand()%3;//tirage de l'aleatoire pour l'arrivée
        }
        while(arrivee[alea] == NULL);

        maPartie->plateau[9][2+2*i] = arrivee[alea];    //stockage de l'arrivée piochée

        arrivee[alea] = NULL;   //libération de l'arrivée utilisée
    }

    //coord des arrivées
    maPartie->plateau[9][2]->myCoord.posX = CARTE_DEPART_X + 8 * CASE_PLATEAU_W;
    maPartie->plateau[9][2]->myCoord.posY = CARTE_DEPART_Y - 2 * CASE_PLATEAU_H;

    maPartie->plateau[9][4]->myCoord.posX = CARTE_DEPART_X + 8 * CASE_PLATEAU_W;
    maPartie->plateau[9][4]->myCoord.posY = CARTE_DEPART_Y;

    maPartie->plateau[9][6]->myCoord.posX = CARTE_DEPART_X + 8 * CASE_PLATEAU_W;
    maPartie->plateau[9][6]->myCoord.posY = CARTE_DEPART_Y + 2 * CASE_PLATEAU_H;
}
/*********************************************************************************************************/



/**< AFFICHAGE DES TOURS DES JOUEURS >*******************************************************************/
void affichageJoueursPlateau(t_partie * maPartie, t_identite * monJoueur)
{
    ///Déclarations
    int i;
    BITMAP * doublebuffer;
    BITMAP * fond;      //chargement de la piece et des ronds de malus
    BITMAP * malusLumiere;
    BITMAP * malusBlessure;
    BITMAP * malusPiege;

    t_identite * parcours = monJoueur;         //structure de parcours
    FONT * pixelated72;        //POLICE

    //chargement de la data
    pixelated72 = chargerPolice("data/police/pixelated_72.pcx");   //chargement de la police
    fond = chargerImage("data/images/plateau/joueurPlateau.bmp");   //avatars
    doublebuffer = creerBuffer(fond->w, fond->h, "buffer affichage joueurs"); //buffer


    malusLumiere = chargerImage("data/images/plateau/malusLumiere.bmp");
    malusBlessure = chargerImage("data/images/plateau/malusBlessure.bmp");
    malusPiege = chargerImage("data/images/plateau/malusPiege.bmp");


    for(i=0; i<maPartie->nbJoueur; i++)
    {//affichage des profils de joueurs
        stretch_blit(fond, doublebuffer, 0, 0, doublebuffer->w, doublebuffer->h, 0, 0, doublebuffer->w, doublebuffer->h ); //fond sur le buffer
        stretch_blit(parcours->avatar, doublebuffer, 0, 0, parcours->avatar->w, parcours->avatar->h, 60, 100,(doublebuffer->w)/4, (doublebuffer->h)*0.8);   //avatar sur  le buffer
        textprintf_ex(doublebuffer, pixelated72, doublebuffer->w/3, 130, makecol(255, 255, 255), -1, "%s", parcours->pseudo);           //nom sur  le buffer
        textprintf_ex(doublebuffer, pixelated72, doublebuffer->w/2, 260, makecol(255, 255, 255), -1, "%d", parcours->score);            //score sur le buffer

        if(parcours->profilJoueur->objetsJoueurs.chariot == 0)
        {
            stretch_blit(malusBlessure, doublebuffer, 0, 0, malusBlessure->w, malusBlessure->h, 0.32*doublebuffer->w, 0.66*doublebuffer->h, (doublebuffer->w/5), (doublebuffer->w/5));
        }

        if(parcours->profilJoueur->objetsJoueurs.lampe == 0)
        {
            stretch_blit(malusLumiere, doublebuffer, 0, 0, malusLumiere->w, malusLumiere->h, 0.55*doublebuffer->w, 0.66*doublebuffer->h, (doublebuffer->w/5), (doublebuffer->w/5));
        }

        if(parcours->profilJoueur->objetsJoueurs.pioche == 0)
        {
            stretch_blit(malusPiege, doublebuffer, 0, 0, malusPiege->w, malusPiege->h, 0.77*doublebuffer->w, 0.66*doublebuffer->h, (doublebuffer->w/5), (doublebuffer->w/5));
        }


        if(i==0)
        {//affichage spécifique pour le premier joueur
            stretch_sprite(maPartie->buffer, doublebuffer, 210, 810, doublebuffer->w/2.2, doublebuffer->h/2.2);   //affichage du joueur à qui c'est la tour
            parcours->profilJoueur->myCoord.posX = POSITION_AVATAR_1_X;
            parcours->profilJoueur->myCoord.posY = POSITION_AVATAR_1_Y;
        }
        else
        {//affichage des autre joueurs
            stretch_sprite(maPartie->buffer, doublebuffer, 0, 1090-160*i, doublebuffer->w/4, doublebuffer->h/4);    //autres joueurs dans la file d'attente

            parcours->profilJoueur->myCoord.posX = 0;
            parcours->profilJoueur->myCoord.posY =POSITION_AVATAR_2_Y-160*i;

        }
        //nettoyage
        clear_bitmap(doublebuffer);
        parcours = parcours->suivant;
    }

    //destruction bitmap
    destroy_bitmap(fond);
    destroy_bitmap(doublebuffer);
    destroy_bitmap(malusBlessure);
    destroy_bitmap(malusLumiere);
    destroy_bitmap(malusPiege);
}
/*********************************************************************************************************/

/**< AFFICHAGE DE LA MAIN D'UN JOUEUR >*******************************************************************/
void afficherMainJoueur(t_partie * maPartie, BITMAP * doublebuffer, t_identite * monJoueur)
{
    //Déclarations
    int i;
    int nb = 0;
    BITMAP * carteModif = creerBuffer(300, 190, "Image grisee carte chemin bis");

    //nettoyage des bitmaps
    clear_bitmap(doublebuffer);
    clear_to_color(doublebuffer, makecol(255, 0, 255));

    for(i=0; i<6; i++)
    {//pour chaque carte de la main
        if(monJoueur->profilJoueur->mainJoueur[i]!= NULL)   // on verifie que la carte existe avant de lui faire n'importe quel traitement
        {
           if((monJoueur->profilJoueur->mainJoueur[i]->estCeChemin == 1) && (monJoueur->profilJoueur->objetsJoueurs.chariot == 0 || monJoueur->profilJoueur->objetsJoueurs.pioche == 0 || monJoueur->profilJoueur->objetsJoueurs.lampe == 0 ))  // si le joeuur a au moins un malus
            {
                carteGrise(monJoueur->profilJoueur->mainJoueur[i]->illustration, carteModif);           // on met la carte en noir et bleau pour lui dire qu'il ne peut pas la poser sur le plateau
                stretch_sprite(doublebuffer, carteModif, monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX, monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY, CARTE_PIOCHE_W, CARTE_PIOCHE_H); //affichage de la carte aux dimensions plateau

            }
            else
            {
                stretch_sprite(doublebuffer, monJoueur->profilJoueur->mainJoueur[i]->illustration, monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX, monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY, CARTE_PIOCHE_W, CARTE_PIOCHE_H); //affichage de la carte aux dimensions plateau
           }

            if(monJoueur->profilJoueur->mainJoueur[i]->estCeChemin == 1)
            {
                nb ++;  // on compte le nombre de cartes chemins pour ensuite pouvoir faire la separation entre les cartes actions et les cartes chemins avec une ligne
            }
        }

    }
    vline(doublebuffer, (CARTE_MAIN_X+nb*220-7), (CARTE_MAIN_Y -20),(CARTE_MAIN_Y + CARTE_PIOCHE_H), makecol(255, 255, 255) );  // ligne de separation

    destroy_bitmap(carteModif);
}
/*********************************************************************************************************/


/**< GRISER UNE CARTE QUAND ELLE NE PEUT PAS ETRE JOUEE >*************************************************/
void carteGrise(BITMAP *monImage, BITMAP * imageFin )
{
    int x,y, couleur, nouveau;

    clear_to_color(imageFin, makecol(255, 0, 255));
    for(y=0; y<monImage->h; y++)                        //on parcourt tous les pixels de l'image
    {
        for(x=0; x<monImage->w; x++)
        {

            couleur = getpixel(monImage, x, y);    // on preleve la couleur du pixel
            if(getr(couleur) == 255 && getb(couleur)==255 && getg(couleur) == 0)   // si il est fushia (couleur transparente on y touche pas
            {

            }
            else  // sinon
            {
                nouveau = (getr(couleur)+getg(couleur)+getb(couleur))/3;    // on fait la moyenne des trois couleurs pour juste avoir l'intensite lumineuse
                putpixel(imageFin, x, y, makecol(nouveau, nouveau, nouveau));    // on injecte la mm quantite de lumirer dans chaque composante de couleur pour avoir une nuance de gris
            }
        }
    }
}
/*********************************************************************************************************/
