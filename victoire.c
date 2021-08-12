#include "header.h"

/**< VFERIFICATION DE LA CONNEXION A ARRIVEE >************************************************************/
int animTestArrivee (t_partie * maPartie, t_identite * monJoueur, BITMAP * doublebuffer, BITMAP * buffer)
{
    ///Déclarations
    int sortie = 0;
    int victoire = 0;
    BITMAP * arrivee;
    BITMAP * arriveeFausse;
    SAMPLE * sammy;
    t_coord coordCourant;

    //initilisation de la position dans le tab
    coordCourant.posX = 1;
    coordCourant.posY = 4;
    arrivee = chargerImage("data/images/plateau/arrivee_1.bmp");            //on affiche l'image correspondante
    arriveeFausse = chargerImage("data/images/plateau/arrivee_0.bmp");            //on affiche l'image correspondante
    sammy = chargerMusique("data/sons/verite.wav");

    play_sample(sammy, 180, 128, 1000, 0);

    do{//tant qu'une case arrivee n'est pas atteinte
        if(key[KEY_UP])
        {//fleche haut
            while(key[KEY_UP])
            {//...
            }
            if(maPartie->plateau[coordCourant.posX][coordCourant.posY-1] != NULL)
            {//si case au dessus non vide
                if(maPartie->plateau[coordCourant.posX][coordCourant.posY-1]->carteChemin->estCeImpasse != 1 || (coordCourant.posX == 9 && (coordCourant.posY-1 == 2 || coordCourant.posY-1 == 4 || coordCourant.posY-1 == 6)))
                {//si pas impasse
                    if(maPartie->plateau[coordCourant.posX][coordCourant.posY]->carteChemin->haut == 1 && maPartie->plateau[coordCourant.posX][coordCourant.posY-1]->carteChemin->bas == 1)
                    {//si les entrées permettent la connexion
                        coordCourant.posY--;
                    }
                }
            }
        }
        if(key[KEY_DOWN])
        {//fleche bas
            while(key[KEY_DOWN])
            {//...
            }
            if(maPartie->plateau[coordCourant.posX][coordCourant.posY+1] != NULL)
            {//si case en dessous non vide
                if(maPartie->plateau[coordCourant.posX][coordCourant.posY+1]->carteChemin->estCeImpasse != 1 || (coordCourant.posX == 9 && (coordCourant.posY+1 == 2 || coordCourant.posY+1 == 4 || coordCourant.posY+1 == 6)))
                {//si pas impasse
                    if(maPartie->plateau[coordCourant.posX][coordCourant.posY]->carteChemin->bas == 1 && maPartie->plateau[coordCourant.posX][coordCourant.posY+1]->carteChemin->haut == 1)
                    {//si les entrées permettent la connexion
                        coordCourant.posY++;
                    }
                }
            }
        }
        if(key[KEY_LEFT])
        {//fleche gauche
            while(key[KEY_LEFT])
            {//...
            }
            if(maPartie->plateau[coordCourant.posX-1][coordCourant.posY] != NULL)
            {//si case a gauche non vide
                if(maPartie->plateau[coordCourant.posX-1][coordCourant.posY]->carteChemin->estCeImpasse != 1 || (coordCourant.posX-1 == 9 && (coordCourant.posY == 2 || coordCourant.posY == 4 || coordCourant.posY == 6)))
                {//si pas impasse
                    if(maPartie->plateau[coordCourant.posX][coordCourant.posY]->carteChemin->gauche == 1 && maPartie->plateau[coordCourant.posX-1][coordCourant.posY]->carteChemin->droite == 1)
                    {//si les entrées permettent la connexion
                        coordCourant.posX--;
                    }
                }
            }
        }
        if(key[KEY_RIGHT])
        {//fleche droite
            while(key[KEY_RIGHT])
            {//...
            }
            if(maPartie->plateau[coordCourant.posX+1][coordCourant.posY] != NULL)
            {//si case a droite non vide
                if(maPartie->plateau[coordCourant.posX+1][coordCourant.posY]->carteChemin->estCeImpasse != 1 || (coordCourant.posX+1 == 9 && (coordCourant.posY == 2 || coordCourant.posY == 4 || coordCourant.posY == 6)))
                {//si pas impasse
                    if(maPartie->plateau[coordCourant.posX][coordCourant.posY]->carteChemin->droite == 1 && maPartie->plateau[coordCourant.posX+1][coordCourant.posY]->carteChemin->gauche == 1)
                    {//si les entrées permettent la connexion
                        coordCourant.posX++;
                    }
                }
            }
        }
        animDeplacement(maPartie, monJoueur, coordCourant);
        if(coordCourant.posX== 9 &&(coordCourant.posY==2 || coordCourant.posY==4 || coordCourant.posY==6))
        {
            sortie = 1;
        }

        //animation de deplacement du boug


    }while(sortie == 0 && !key[KEY_O]);


    if((sortie == 1) && maPartie->plateau[coordCourant.posX][coordCourant.posY]->carteChemin->estCeImpasse == 0)
    {//si sortie du while
        victoire = 1;
        maPartie->plateau[coordCourant.posX][coordCourant.posY]->illustration = arrivee;
        affichageDebutTour(doublebuffer, buffer, maPartie, monJoueur);
        sleep(4);

    }
    else
    {
        if(coordCourant.posX== 9 &&(coordCourant.posY==2 || coordCourant.posY==4 || coordCourant.posY==6))
        {
           maPartie->plateau[coordCourant.posX][coordCourant.posY]->illustration = arriveeFausse;
        }
    }

    destroy_sample(sammy);
    //rendu bool victoire
    return victoire;

}
/*********************************************************************************************************/

/**< ANIMATION DE DEPLACEMENT DE L'AVATAR SUR LE PLATEAU >************************************************/
void animDeplacement (t_partie * maPartie, t_identite * monJoueur, t_coord coordCourant)
{
    ///Déclarations
    BITMAP * doublebuffer;
    BITMAP * buffer;

    //création des bitmaps buffer
    doublebuffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");
    buffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");

    clear_bitmap(doublebuffer); //nettoyage buffer partie

    blit(maPartie->buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage duplateau sauvegardé

    clear_to_color(buffer, makecol(255, 0, 255));
    afficherMainJoueur(maPartie, buffer, monJoueur);  //affichage de la main du joueur
    masked_blit(buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    rectfill(doublebuffer, 232, 0, SCREEN_W, 20, makecol(0, 0, 180));
    textout_ex(doublebuffer, font, "Une arrivve est raliee ! Essayez de l'atteindre avec votre avatar !",280, 10, makecol(255, 255, 255),-1);   //affichage d'un message de confirmation

    stretch_sprite(doublebuffer, monJoueur->avatar, maPartie->plateau[coordCourant.posX][coordCourant.posY]->myCoord.posX + CARTE_PLATEAU_W/3, maPartie->plateau[coordCourant.posX][coordCourant.posY]->myCoord.posY, monJoueur->avatar->w/6, monJoueur->avatar->h/6);

    masked_blit(doublebuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage sur le screen

    //destruction des bitmaps
    destroy_bitmap(doublebuffer);
    destroy_bitmap(buffer);
}
/*********************************************************************************************************/


/**< ATTRIBUTION DES PEPITES D'OR >***********************************************************************/
void attributionOr (t_partie * maPartie, t_identite * monJoueur)
{
    //déclaraitons
    t_identite * courant;
    int i;
    courant = monJoueur;

    if(courant->profilJoueur->role == 1)   //le saboteur a gagne
    {//si le joueur vainqueur est saboteur
        courant->profilJoueur->gain = 6;
        courant->score = courant->score + courant->profilJoueur->gain;
    }
    else
    {//si le joueur vainqueur est associé
        for(i=0; i<maPartie->nbJoueur; i++)
        {//parcours de la liste
            if(i == 0)
            {
                courant->profilJoueur->gain = 5;  // celui qui est arrivé a la pepite
            }
            else if(courant->profilJoueur->role == 0)
            {//si le joueur n'est pas un saboteur
                    courant->profilJoueur->gain = (rand()%3)+1;//attribution aléatoire des pépites
            }
            else
            {
                courant->profilJoueur->gain = 0;  // si c'est le saboteur il ne gagne rien
            }

            courant->score = courant->score + courant->profilJoueur->gain;
            courant = courant->suivant;//parcours
        }
    }
    courant = monJoueur;
}
/*********************************************************************************************************/

/**< VICTOIRE SABOTEUR >**********************************************************************************/
void victoireSaboteur(t_partie * maPartie, t_identite * monJoueur)
{
    BITMAP * fond;
    BITMAP * buffer;
    BITMAP * cailloux;
    BITMAP * dollar;
    FONT * scooby;

    t_animation * chercheur;
    t_animation oiseau;
    t_element pierres [50];
    t_identite * courant;

    int i = 0, j, k;
    int CST = 0;
    int compteur = 0;


    chercheur = (t_animation *) malloc (sizeof(t_animation) * maPartie->nbJoueur-1);

    scooby = chargerPolice("data/police/ScoobyDoo72.pcx");
    fond = chargerImage("data/images/victoire/scrolling_background.bmp");
    oiseau.illustration = chargerImage("data/images/victoire/saboteur.bmp");
    dollar = chargerImage("data/images/menu/dollar.bmp");

    cailloux= chargerImage("data/images/victoire/cailloux.bmp");

    buffer = create_bitmap(SCREEN_W, SCREEN_H);


    courant = maPartie->ancreJoueur;

    for(j=0; j<maPartie->nbJoueur; j++)  //tableau de structure pour les chercheurs d'or
    {
        if(courant->profilJoueur->role == 0)
        {
        chercheur[j].illustration =  courant->avatar;
        chercheur[j].position.posX = SCREEN_W/2+10*j*30;
        chercheur[j].position.posY = SCREEN_H/2+10;
        chercheur[j].vitesseY =2;
        chercheur[j].vitesseX =3;
        }
        courant = courant->suivant;

    }

    //initialisations des positions du saboteur
    oiseau.position.posX = 0;
    oiseau.position.posY = 800;

    while(!key[KEY_ESC])
    {
        //gommer le buffer
        clear_bitmap(buffer);

        ///defiller limage (vers la gauche)
        stretch_blit(fond, buffer, 0, 0,2560,720, -i, 0,SCREEN_W,SCREEN_H);
        stretch_blit(fond, buffer, 0, 0,2560,720, -i+SCREEN_W, 0,SCREEN_W,SCREEN_H);

        textout_ex(buffer, font, "Appuyez sur ECHAP pour quitter", 10, 10, makecol(255, 255, 255), -1);
        textout_ex(buffer, scooby, "VICTOIRE DU SABOTEUR ! PROF PERICLES EST FIER DE VOUS", 100, 200, makecol(255, 192, 0), -1);
        textout_ex(buffer, scooby, "VOUS AVEZ BATTU MYSTERE ASSOCIE", 400, 300, makecol(255, 192, 0), -1);

        textprintf_ex(buffer, scooby, 400, 400, makecol(255, 192, 0), -1, "Vous emportez %d", monJoueur->profilJoueur->gain   );
        stretch_sprite(buffer, dollar, 960, 410, dollar->w, dollar->h );



        oiseau.position = deplacementSaboteur(oiseau);
        draw_sprite(buffer, oiseau.illustration, oiseau.position.posX, oiseau.position.posY);

        if(key[KEY_SPACE])  // le saboteur lance des pierres de maniere parabolique
        {
            compteur ++;
            compteur = compteur % 50;
            while(key[KEY_SPACE])
            {
            }
            pierres[compteur].position.posX = oiseau.position.posX;
            pierres[compteur].position.posY = oiseau.position.posY;
            pierres[compteur].phase = 1;
            pierres[compteur].enLAir = 1;
            pierres[compteur].vitesse = rand() % 30 + 10;
            pierres[compteur].distanceY = 0;

        }
        //affichage pierre
        for(j=0; j<50; j++)  // on peut lancer jusqua 50 pierres
        {

            if(pierres[j].enLAir == 1)  // si elle a ete envoyee
            {

                if(pierres[j].position.posY >= SCREEN_H - cailloux->h/4 && pierres[j].phase == 2)  // tombe en dessous de l'ecran
                {
                    pierres[j].enLAir = 0;
                }
                else if((pierres[j].position.posY <= 0 || pierres[j].distanceY >= 20) && pierres[j].phase == 1)   //passe la premiere partie de sa trajectoire
                {
                    pierres[j].phase = 2;
                }
                else if(pierres[j].phase == 1   )  //phase montante
                {
                    pierres[j].position.posX += pierres[j].vitesse;
                    pierres[j].position.posY -= pierres[j].vitesse;
                    pierres[j].distanceY ++;
                }
                else if(pierres[j].phase == 2   )  //phase descendante
                {
                    pierres[j].position.posX += pierres[j].vitesse;
                    pierres[j].position.posY += pierres[j].vitesse;
                }
                stretch_sprite(buffer, cailloux,  pierres[j].position.posX,  pierres[j].position.posY, cailloux->w/4, cailloux->h/4);
            }
        }


        for(j=0; j<maPartie->nbJoueur-1; j++)  //deplacement de tous les chercheurs do'or de maniere aleatoire avec detection des bords
        {
            chercheur[j].position.posX += chercheur[j].vitesseX;
            chercheur[j].position.posY += chercheur[j].vitesseY;

            if(chercheur[j].position.posX<SCREEN_W/2)  //bord gauche
            {
                chercheur[j].vitesseX = -chercheur[j].vitesseX;
            }
            if(chercheur[j].position.posX+chercheur[j].illustration->w/2>SCREEN_W)  //bord droite
            {
                chercheur[j].vitesseX = -chercheur[j].vitesseX;
            }


            if(chercheur[j].position.posY<SCREEN_H/2) //bord haut
            {
                chercheur[j].vitesseY = -chercheur[j].vitesseY;
            }
            if(chercheur[j].position.posY+chercheur[j].illustration->h/2>SCREEN_H)  //bord bas
            {
                chercheur[j].vitesseY = -chercheur[j].vitesseY;
            }

           for(k=0; k<maPartie->nbJoueur-1; k++)
            {
                if(k!=j)
                {
                     collisionChercheurs(&chercheur[j], &chercheur[k]);
                }
            }

            stretch_sprite(buffer, chercheur[j].illustration, chercheur[j].position.posX, chercheur[j].position.posY, chercheur[j].illustration->w/2, chercheur[j].illustration->h/2);

        }

        //afficher sur lecran
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        i= i+2;
        CST = CST +6;
        if(i==SCREEN_W)
        {
            i=0;
        }

    }
    destroy_bitmap(fond);
    destroy_bitmap(buffer);
    destroy_bitmap(cailloux);
    destroy_bitmap(dollar);
    destroy_font(scooby);
}
/*********************************************************************************************************/

/**< VICTOIRE SABOTEUR : SP collision >*******************************************************************/
void collisionChercheurs (t_animation * chercheur1, t_animation * chercheur2)
{
    //detection de pixels en communs entre les differents personnages : si oui on inverse leur sens de deplacement
    if(((chercheur1->position.posX <= chercheur2->position.posX+chercheur2->illustration->w/2) && (chercheur1->position.posX >= chercheur2->position.posX)) && ((chercheur1->position.posY>=chercheur2->position.posY) && chercheur1->position.posY<=chercheur2->position.posY+(chercheur2->illustration->h/2)))
    {
        chercheur1->vitesseX = -chercheur1->vitesseX;
        chercheur2->vitesseX = -chercheur2->vitesseX;

        chercheur1->position.posX += chercheur1->vitesseX;
        chercheur1->position.posY += chercheur1->vitesseY;

        chercheur2->position.posX += chercheur2->vitesseX;
        chercheur2->position.posY += chercheur2->vitesseY;

    }

        if(((chercheur1->position.posX+chercheur1->illustration->w/2 >= chercheur2->position.posX) && (chercheur1->position.posX+chercheur1->illustration->w/2 <= chercheur2->position.posX + chercheur2->illustration->w/2)) && ((chercheur1->position.posY>=chercheur2->position.posY) && chercheur1->position.posY<=chercheur2->position.posY+(chercheur2->illustration->h/2)))
    {

        chercheur1->vitesseX = -chercheur1->vitesseX;
        chercheur2->vitesseX = -chercheur2->vitesseX;

        chercheur1->position.posX += chercheur1->vitesseX;
        chercheur1->position.posY += chercheur1->vitesseY;

        chercheur2->position.posX += chercheur2->vitesseX;
        chercheur2->position.posY += chercheur2->vitesseY;

    }
}
/*********************************************************************************************************/

/**< VICTOIRE SABOTEUR : deplacement saboteur >***********************************************************/
t_coord deplacementSaboteur (t_animation oiseau)
{
    int vitesse = 10;
    //pour chaque fleche on incremente puis on verifie qu'in ne sort pas du cercle
    if(key[KEY_UP])
    {
        oiseau.position.posY = oiseau.position.posY-vitesse;
        if(oiseau.position.posY <0)
        {
            oiseau.position.posY = 0;
        }
    }
    if(key[KEY_DOWN])
    {
        oiseau.position.posY = oiseau.position.posY+vitesse;
        if(oiseau.position.posY>SCREEN_H-oiseau.illustration->h)
        {
            oiseau.position.posY = SCREEN_H-oiseau.illustration->h;
        }
    }
    if(key[KEY_LEFT])
    {
        oiseau.position.posX = oiseau.position.posX-vitesse;
        if(oiseau.position.posX<0)
        {
            oiseau.position.posX = 0;
        }
    }
    if(key[KEY_RIGHT])
    {
        oiseau.position.posX = oiseau.position.posX+vitesse;
        if(oiseau.position.posX>SCREEN_W/2-oiseau.illustration->w)
        {
            oiseau.position.posX = SCREEN_W/2-oiseau.illustration->w;
        }
    }

    return oiseau.position;
}

/*********************************************************************************************************/

/**< VICTOIRE ASSOCIES >**********************************************************************************/

void victoireAssocies(t_partie * maPartie, t_identite * gagnant)
{
    //Déclarations BITMAPS
    BITMAP * fond;
    BITMAP * buffer;
    BITMAP * doubleBuffer;
   t_identite * courant;
    BITMAP * fontaine[5];
    BITMAP * perso;
    BITMAP * amas;
    BITMAP * dollar;
    FONT * ScoobyDoo72;
    SAMPLE * croque_scooby;
    courant = gagnant;

    //Déclarations compteurs
    int i = 0, j =0, tour = 10, place = -20;

    //chargement des images
    fond = chargerImage("data/images/victoire/fondVictoireAssocies.bmp");
    amas = chargerImage("data/images/victoire/amasPiece.bmp");
    dollar = chargerImage("data/images/menu/dollar.bmp");
    fontaine[0] = chargerImage("data/images/victoire/fontaine1.bmp");
    fontaine[1] = chargerImage("data/images/victoire/fontaine2.bmp");
    fontaine[2] = chargerImage("data/images/victoire/fontaine3.bmp");
    fontaine[3] = chargerImage("data/images/victoire/fontaine4.bmp");
    fontaine[4] = chargerImage("data/images/victoire/fontaine5.bmp");
    ScoobyDoo72 = chargerPolice("data/police/ScoobyDoo72.pcx");
    croque_scooby = chargerMusique("data/sons/croque_scooby.wav");
    //créations de buffers
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    doubleBuffer = create_bitmap(SCREEN_W, SCREEN_H);
    perso = create_bitmap(SCREEN_W, SCREEN_H);

    //creation de la BITMAP permanente
    stretch_blit(fond, perso, 20, 0, fond->w, fond->h, 0, 0 , SCREEN_W, SCREEN_H);  //fond
    textout_ex(buffer, font, "Appuyez sur ECHAP pour quitter", 10, 10, makecol(255, 255, 255), -1);
    textprintf_ex(perso, ScoobyDoo72, 300, 200, makecol(255, 255, 255), -1, "VICTOIRE DES MYSTERES ASSOCIES !" );
    play_sample(croque_scooby, 255, 125, 1000, 0);
    for(i=0; i<maPartie->nbJoueur; i++)  //affichage des joeurs(sauf le saboteur)
    {
        if(courant->profilJoueur->role == 0)
        {
            stretch_sprite(perso, courant->avatar, 1210 - 280*i, 550 + 30*i, courant->avatar->w/(1+0.1*i), courant->avatar->h/(1+0.1*i));
            textprintf_ex(perso, ScoobyDoo72, 1210 - 280*i, 500, makecol(255, 255, 255), -1, "%s", courant->pseudo);
            textprintf_ex(perso, ScoobyDoo72, 1300 - 280*i, 1000, makecol(255, 255, 255), -1, "%d",courant->profilJoueur->gain );
            stretch_sprite(perso, dollar,1340 - 280*i , 1010, dollar->w, dollar->h);
        }
        courant = courant->suivant;
    }


    while(!key[KEY_ESC])  //boucle d'affichage
    {
        clear_bitmap(doubleBuffer);   //effacer BITMAP BUFFER
        rectfill(doubleBuffer, 0, 0, SCREEN_W, SCREEN_H, makecol(255, 0, 255));   //fond transparent

        //incrémentation des compteurs
        j++;
        j = j%5;
        tour --;
        if(tour == -150)
        {
            tour = 10;
            place += 20;

        }

        courant = gagnant;
        for(i=0; i<maPartie->nbJoueur-1; i++)
        {
            if(courant->profilJoueur->role == 0 && courant->profilJoueur->gain * 20 >= place)
            {
                stretch_sprite(doubleBuffer, fontaine[j], 1210-280*i, 750, fontaine[j]->w/4, fontaine[j]->h/4); //affichage des fontaines
                stretch_sprite(perso, amas, (1400-280*i +(rand()%20 -10 + place)), 915+ tour, amas->w/4, amas->h/4 );   //affichage accumulations des pièces
                stretch_sprite(perso, amas, (1270-280*i +(rand()%20 -10-place)), 915+ tour, amas->w/4, amas->h/4 );
            }
             courant = courant->suivant;
        }


        //affichage sur le buffer le fond puis les crocs scooby
        blit(perso, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        stretch_sprite(buffer, doubleBuffer, 0, 0, SCREEN_W, SCREEN_H);

        //affichage de tout sur l'ecran
                        textout_ex(buffer, font, "Appuyez sur ECHAP pour quitter", 10, 10, makecol(255, 255, 255), -1);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    destroy_bitmap(fond);
    destroy_bitmap(buffer);
    destroy_bitmap(doubleBuffer);
    destroy_bitmap(perso);
    for(i=0; i<5; i++)
    {
        destroy_bitmap(fontaine[i]);
    }
    destroy_bitmap(amas);
    destroy_font(ScoobyDoo72);
    destroy_bitmap(dollar);
    destroy_sample(croque_scooby);

}
/*********************************************************************************************************/

/**< ECRAN GAME OVER >************************************************************************************/
void gameOver()
{
    //Déclarations BITMAPS
    BITMAP * fond;
    BITMAP * buffer;
    BITMAP * doubleBuffer;
    FONT * scoobyDoo72;

    fond = chargerImage("data/images/victoire/gameOverFond.bmp");
    buffer = creerBuffer(SCREEN_W, SCREEN_H, "creation d'un buffer : gameOver");
    doubleBuffer = creerBuffer(SCREEN_W/2, SCREEN_H/2, "creation d'un buffer : gameOver");

    scoobyDoo72 = chargerPolice("data/police/ScoobyDoo72.pcx");

    int  x1 = 30, y1 = 30, i=1, j=1;
    int r = 255, v = 0, b = 0, couleur;
    int variation =1, compteur = 0;


    while(!key[KEY_ESC])   //Condition de sortie
    {
        x1 = x1 + i;  //Incrémentatuin coordonnées cercle
        y1 = y1 + j;

/////COULEUR ///////////////////////////////////////////////////////////////////////////////
        switch(variation)
        {
        case 1:
            v ++;      //Incrémentation de la bonne couleur pour un effet arc en ciel
            break;

        case 2:
            r--;
            break;

        case 3:
            b++;
            break;

        case 4:
            v--;
            break;

        case 5:
            r++;
            break;

        case 6:
            b--;
            break;
        }

        compteur ++;
        if (compteur == 255)    //Un cas va de 0 à 255 (incrémentation ou décrémentation du compteur
        {
            compteur =0;
            variation ++;
        }
        if(variation == 7 )  //Réinitialisation au premier cas lorsqu'on a atteind le dernier
        {
            variation = 1;
        }


        couleur = makecol(r, v, b);             //Valeur de la couleur du cercle
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if(x1 <= 0)
        {
            i = (rand()% 7 +5);   //incrémentation entre 5 et 12 aléatoirement pour pouvoir modifier la direction et la vitesse
        }
        if(x1 >= doubleBuffer->w -380)
        {
            i = -(rand()% 7 +5);
        }

        if(y1 <= 0)
        {
            j = (rand()% 7 +5);
        }

        if(y1 >= doubleBuffer->h - 60)
        {
            j = -(rand()% 7 +5);
        }

//////////Affichages ///////////////////////////////////////////////////////////////////////////////////////////////////////
        //nettoyage du double buffer
        clear_bitmap(doubleBuffer);

        rectfill(doubleBuffer, 0, 0, doubleBuffer->w, doubleBuffer->h, makecol(255, 0, 255));

        //affichage sur le double buffer

        textprintf_ex(doubleBuffer, scoobyDoo72, x1, y1, couleur, -1, "GAME OVER");
        //Affichage sur le buffer
        stretch_blit(fond, buffer, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);
        stretch_sprite(buffer, doubleBuffer, 0, 0, SCREEN_W, SCREEN_H);
        //affichage sur l"ecran
                textout_ex(buffer, font, "Appuyez sur ECHAP pour quitter", 10, 10, makecol(255, 255, 255), -1);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    destroy_bitmap(buffer);
    destroy_bitmap(fond);
    destroy_bitmap(doubleBuffer);
    destroy_font(scoobyDoo72);
}
/*********************************************************************************************************/





