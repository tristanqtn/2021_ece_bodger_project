#include "header.h"

/**< FONCTION DE VERIFICATION DE L'ETAT DES OBJETS >******************************************************/
int verifObjets (t_joueur * leJoueur)
{
    //on récupère la structure d'un joueur
    if(leJoueur->objetsJoueurs.chariot == 1 && leJoueur->objetsJoueurs.lampe == 1 && leJoueur->objetsJoueurs.pioche == 1)
    {//si tout les objet du joueur son en bon état (donc état 1)
        return 1;                                                           //on rend 1
    }
    else
    {//si 1 seul des outils est cassé
        return 0;                                                           //on rend 0
    }
}
/*********************************************************************************************************/


/**< TEST DE LA CONNEXION DES CHEMIN >********************************************************************/
int verfiConnexionChemin(int j, int k, t_carte * plateau[PLATEAU_COLONNE][PLATEAU_LIGNE], t_carte * maCarte)
{
    ///Déclarations
    int compte = 0, validation = 0;
    int connexion =0;

    //chemin gauche
    if(plateau[j-1][k] == NULL || j == 0)
    {//si la carte est en bord de plateau gauche
        compte ++;//validation d'un des tests sur 4
    }

    else if(maCarte->carteChemin->gauche ==  plateau[j-1][k]->carteChemin->droite)
    {//si la carte à placer a une entrée gauche et la carte deja placée à une entrée droite
        compte ++;//validation d'un des tests sur 4
        if(maCarte->carteChemin->gauche == 1 )
        {
            if( j-1 == 9 && (k== 2 || k==4 || k== 6))
            {//si le chemin testé est une case arrivée, il doit y avoir une connexion avec un autre chemin obligatoirement
            }
            else
            {//validation de la connexion
                connexion ++;
            }
        }
    }

    //chemin droite
    if(plateau[j+1][k] == NULL  || j == 10)
    {//si la carte est au bord du plateau droit
        compte ++;//validation d'un des tests sur 4
    }
    else if(maCarte->carteChemin->droite ==  plateau[j+1][k]->carteChemin->gauche )
    {//si la carte à placer a une entrée droite et la carte deja placée à une entrée gauche
        compte ++;//validation d'un des tests sur 4
        if(maCarte->carteChemin->droite == 1 )
        {
             if( j+1 == 9 && (k== 2 || k==4 || k== 6))
            {//si le chemin testé est une case arrivée, il doit y avoir une connexion avec un autre chemin obligatoirement
            }
            else
            {//validation de la connexion
                connexion ++;
            }
        }
    }

    //chemin haut
    if(plateau[j][k-1] == NULL || k == 0)
    {
        compte ++;//validation d'un des tests sur 4
    }
    else if(maCarte->carteChemin->haut ==  plateau[j][k-1]->carteChemin->bas )
    {//si la carte à placer a une entree haut et la carte deja placée à une entrée bas
        compte ++;//validation d'un des tests sur 4
        if(maCarte->carteChemin->haut == 1 )
        {
            if( j == 9 && (k-1== 2 || k-1==4 || k-1== 6))
            {//si le chemin testé est une case arrivée, il doit y avoir une connexion avec un autre chemin obligatoirement
            }
            else
            {//validation de la connexion
                connexion ++;
            }
        }
    }

    //chemin bas
    if(plateau[j][k+1] == NULL || k == 8)
    {
        compte ++;//validation d'un des tests sur 4
    }
    else if(maCarte->carteChemin->bas ==  plateau[j][k+1]->carteChemin->haut )
    {//si la carte à placer a une entrée bas et la carte deja placée à une entrée haut
        compte ++;//validation d'un des tests sur 4
        if(maCarte->carteChemin->bas == 1 )
        {
             if( j == 9 && (k+1== 2 || k+1==4 || k+1== 6))
            {//si le chemin testé est une case arrivée, il doit y avoir une connexion avec un autre chemin obligatoirement
            }
            else
            {//validation de la connexion
                connexion ++;
            }
        }
    }

    if(connexion >= 1 && compte == 4)
    {//si les 4 cotés on étés testés, et que les connexion sont supérieurs à 1
        validation = 1;//bool de rendu autorisant le placement du chemin
    }

    //rendu de la validation
    return validation;
}
/*********************************************************************************************************/


/**< VERIF SI CLIQUE EFFECTUE DANS LA ZONE DONNEE >*******************************************************/
int appuieBouton(int x1, int y1, int x2, int y2)
{
    int position = 0;

    if(mouse_b == 1)
    {//si click gauche de la souris

        if(mouse_x >= x1 && mouse_x <= x2 && mouse_y >= y1 && mouse_y <= y2 )
       {//si le clique est dans la zone donnée
           position = 1;//bool rend 1
            while(mouse_b != 0)    //On attend que le clique gauche soit relaché avant d'aller plus loin pour éviter les clics involontaires quand on change de pages
            {//tant que le click reste appuyé
                rest(5);//tempo
            }
       }
       else
       {//si pas de clique dans la zone
           position = 0;//bool rend 0
       }

    }
    //rendu du booleen
    return position;

}
/*********************************************************************************************************/

/**< VERIF SI CLIQUE EFFECTUE DANS LA ZONE DONNEE >*******************************************************/
int appuieCarte(int x1, int y1, int x2, int y2)
{
    ///Déclarations
    int position = 0;

    if(mouse_b == 1)
    {//si clique
        if(mouse_x >= x1 && mouse_x <= x2 && mouse_y >= y1 && mouse_y <= y2 )
       {//si clique dans la bonne zone
           position = 1;//bool rend 1
       }//diff avec le appuieBouton : n'attend pas que le clique soit relaché;
       else
       {//si le clique n'est pas au bon endroit
           position = 0;//bool rend 0
       }

    }
    //rendu du booléen
    return position;
}
/*********************************************************************************************************/



/**< SP DE COMPTE DES CARTES DE LA PIOCHE>****************************************************************/
void verificationPioche(t_cartePioche * parcours)
{
    int chemin = 0;
    int objet = 0;
    int carte = 0;
    int eboulemnt = 0;

    while(parcours != NULL)
    {
        printf("\n chemin %d\t objet %d \t carte %d \t eboulement %d",parcours->typeDeCarte->estCeChemin, parcours->typeDeCarte->estCeObjet, parcours->typeDeCarte->estCeMap, parcours->typeDeCarte->estCeEboulement);



        parcours = parcours->suivante;
    }
    printf("Après melange : \n\n chemin %d\t objet %d \t carte %d \t eboulement %d", chemin, objet, carte, eboulemnt);
}
/*********************************************************************************************************/


/**< TEST LA POSITION D'UNE CARTE SELON 2 PT >************************************************************/
int testPositionCarte (t_carte *maCarte, int x1, int x2, int y1, int y2)
{
    ///Déclaration
    int rendu =0;

    if(x1<=maCarte->myCoord.posX && maCarte->myCoord.posX<=x2 && y1<=maCarte->myCoord.posY && maCarte->myCoord.posY<=y2)
    {//si les coords de la carte  sont comprises dans les coords données
        rendu =1;//bool rend 1, la carte est dans la zone desirée
    }

    //rendu booleen de placement
    return rendu;
}
/*********************************************************************************************************/



/**< VERIFICATION DE CONNEXION A L'ARRIVEE >**************************************************************/
int verifArrivee (t_partie * maPartie)
{
    ///Déclarations
    int j, k;
    int rendu;

    //parcours du plateau
    for(j=0; j<PLATEAU_COLONNE; j++)
    {
       for(k=0; k<PLATEAU_LIGNE; k++)
       {
           if(((k==2 || k==4 || k==6) && (j==8 || j==10)) || ((k==1 || k==3 || k==5 || k==7) && (j==9)))
           {//si la case est en contact avec une arrivée
             if(maPartie->plateau[j][k] != NULL && maPartie->plateau[j][k]->carteChemin->estCeImpasse == 0)
             {
                 rendu = 1;//verif à 1
                 //sortie du test
                 k=PLATEAU_LIGNE+1;
                 j=PLATEAU_COLONNE+1;
             }
           }
       }
    }
    //rendu de la validation
    return rendu;
}
/*********************************************************************************************************/



/**< VERIFICATION DE LA POSSIBILITE DE JOUER >************************************************************/
void verifPossibiliteJeu(t_partie * maPartie, t_identite * monJoueur, int tourPasse)
{
    if(maPartie->ancrePioche == NULL)
    {//si la pioche est vide

                if(tourPasse==1)
                {
                    maPartie->compteurPassageTour++;
                }
                else
                {
                    maPartie->compteurPassageTour++;
                }
    }
}
/*********************************************************************************************************/

