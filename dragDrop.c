#include "header.h"

///APO : A COMMENTER
/**< DRAG AND DROP D'UNE CARTE >**************************************************************************/
void dragAndDrop(int carteABouger, t_partie * maPartie, t_identite * monJoueur)
{
    BITMAP * doublebuffer;
    BITMAP * buffer;
    t_carte * carteTournee = NULL;
    char phrase [100];

    doublebuffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");
    buffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");

    int ecartX, ecartY;
    int startX, startY;

    startX = monJoueur->profilJoueur->mainJoueur[carteABouger]->myCoord.posX;    //on garde en memoire la position initiale de la carte
    startY = monJoueur->profilJoueur->mainJoueur[carteABouger]->myCoord.posY;
    ecartX = mouse_x - startX;                                                      // on calcule l'ecart entre les coordonnes de la souris et les coordonnes de la carte
    ecartY = mouse_y - startY;

    show_mouse(screen);

    while(mouse_b == 1)    // tant que le clique droit est appuyé
    {
        if(key[KEY_R] && (monJoueur->profilJoueur->mainJoueur[carteABouger]->estCeChemin == 1))    // si la carte R est pressée et que c'est un chemin
        {
            while(key[KEY_R])                                                                       //on peut la tourner de 180°
            {
                rest(5);                                                     // pour eviter les multiples cliques  involontaire on met une boucle while
            }

            carteTournee = monJoueur->profilJoueur->mainJoueur[carteABouger];    // on utilise une structure carte intermediaire pour garder en memoire la carte
            monJoueur->profilJoueur->mainJoueur[carteABouger] = NULL;               //on supprime le pointeur vers la carte en question tu tableau de la main du joueur
            monJoueur->profilJoueur->mainJoueur[carteABouger] = rotationCarte(carteTournee);    //on le remplace par le pointeur vers la carte tournee

        }

        monJoueur->profilJoueur->mainJoueur[carteABouger]->myCoord.posX = mouse_x-ecartX;   // on actualise les coordonnes de la carte en temps reel
        monJoueur->profilJoueur->mainJoueur[carteABouger]->myCoord.posY = mouse_y-ecartY;

        affichageTour(doublebuffer, buffer, maPartie, monJoueur);     // on affiche le plateau avec la position de la carte qui evolue
        rest(10);
        if(key[KEY_SPACE])
        {
            while(key[KEY_SPACE])                                                                       //on peut la tourner de 180°
            {
                rest(5);                                                     // pour eviter les multiples cliques  involontaire on met une boucle while
            }
            strcpy(phrase, "annulation du drag and drop");
            retourPosInit(monJoueur, maPartie, carteABouger, phrase);
        }
    }

    carteTournee = NULL;
    destroy_bitmap(doublebuffer);
    destroy_bitmap(buffer);
}
/*********************************************************************************************************/

/**< Rotation de la carte chemin>*************************************************************************/
t_carte * rotationCarte(t_carte * maCarte)
{
    BITMAP * carteTournee;
    t_carte * nouvelleCarte = NULL;

    carteTournee = creerBuffer(maCarte->illustration->w, maCarte->illustration->h, "la carte tournee");

    nouvelleCarte = allocInitCarte();
    nouvelleCarte->carteChemin = allocInitChemin();
    nouvelleCarte->estCeChemin = 1;                     // on creer et alloiue une nouvelle carte chemin

    //recopiage du mode impasse ou non
    nouvelleCarte->carteChemin->estCeImpasse = maCarte->carteChemin->estCeImpasse;
    clear_to_color(carteTournee, makecol(255, 0, 255));
    //inversion etat haut en bas
    if(maCarte->carteChemin->haut == 1)    // on indique ses chemins en mettant l'oppose de l'ancienne carte
    {
        nouvelleCarte->carteChemin->bas = 1;
    }
    else
    {
        nouvelleCarte->carteChemin->bas = 0;
    }

    //inversion etat bas en haur
    if(maCarte->carteChemin->bas == 1)
    {
        nouvelleCarte->carteChemin->haut = 1;
    }
    else
    {
        nouvelleCarte->carteChemin->haut = 0;
    }
    //inversion etat gauche devient droite
    if(maCarte->carteChemin->gauche == 1)
    {
        nouvelleCarte->carteChemin->droite = 1;
    }
    else
    {
        nouvelleCarte->carteChemin->droite = 0;
    }

    //inversion etat droite devient gauche
    if(maCarte->carteChemin->droite == 1)
    {
        nouvelleCarte->carteChemin->gauche = 1;
    }
    else
    {
        nouvelleCarte->carteChemin->gauche = 0;
    }


    rotate_sprite(carteTournee, maCarte->illustration, 0, 0, itofix(128));  //256 est un tour complet   // on tourne k'ancienne bitmap de 10°°

    nouvelleCarte->illustration = carteTournee;
    free(maCarte);

    return nouvelleCarte;    // on rend l'adresse de la carte
}
/*********************************************************************************************************/


/**< Retour à la position de base de la carte >***********************************************************/
void retourPosInit( t_identite * monJoueur, t_partie * maPartie, int i, char phrase[100] )
{
    int vitesse = 20;
    int startX, startY;
    BITMAP * doublebuffer;
    BITMAP * buffer;

    startX = CARTE_MAIN_X+i*220;    // on initialise les variables qui sont la position finale de la carte (notre objectif)
    startY= CARTE_MAIN_Y;

    doublebuffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");
    buffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");


    while(monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX != startX || monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY != startY)   // tant qu'on a pas atteind les bons coordonnees
    {
// pour la composante horizontale
        if(monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX > startX-vitesse && monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX < startX+vitesse )
        {
            monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX = startX; // si la position de la carte est tres proche de la position finale (distance inferieur a la prochaine incrementation de pixels)
        }
        else if(monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX > startX )   // si la carte est a droite de sa position finale
        {
            monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX -= vitesse;
        }
        else if(monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX < startX )   // si la carte est a gauche de sa position finale
        {
            monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX += vitesse;
        }
// pour la composante verticale
        if(monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY > startY-vitesse && monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY < startY+vitesse )
        {
            monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY = startY;  // si la position de la carte est tres proche de la position finale (distance inferieur a la prochaine incrementation de pixels)
        }
        else if(monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY > startY)   // si la carte est en dessous de sa position finale
        {
            monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY -= vitesse;
        }
        else if(monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY < startY)  // si la carte est au dessus de sa position finale
        {
            monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY += vitesse;
        }

        clear_bitmap(doublebuffer); //nettoyage buffer partie

        blit(maPartie->buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage duplateau sauvegardé

        clear_to_color(buffer, makecol(255, 0, 255));
        afficherMainJoueur(maPartie, buffer, monJoueur);  //affichage de la main du joueur
        masked_blit(buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        //Affichage du type d'erreur
        if(strcmp(phrase, "0") != 0)
        {
            rectfill(doublebuffer, 232, 0, SCREEN_W, 20, makecol(180, 0, 0) );
            textprintf_ex(doublebuffer, font, 280, 10, makecol(255, 255, 255), -1, "%s", phrase);
        }


        masked_blit(doublebuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage sur le screen
    }

    destroy_bitmap(doublebuffer);
    destroy_bitmap(buffer);

}
/*********************************************************************************************************/

