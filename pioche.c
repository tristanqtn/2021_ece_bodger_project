#include "header.h"

/**< CREATION ET ALLOCATION D'UNE STRUCTURE CARTE>********************************************************/
//alloue dynamiquement et initialise une structure carte
t_carte * allocInitCarte()
{
    t_carte * nouveau = NULL;  //Création de la structure d'une carte

    nouveau = (t_carte * )malloc(sizeof(t_carte));   //allocation dynamique de la carte

    if(nouveau == NULL)      //Blindage pour vérifier le succès de l'allocation
    {
        sortieErreur("Problème d'allocation d'une carte");
    }

    //initialise toutes les variables
    nouveau->estCeChemin = 0;
    nouveau->estCeObjet = 0;

    nouveau->estCeMap = 0;
    nouveau->estCeEboulement = 0;

    nouveau->carteChemin = NULL;
    nouveau->carteObjet = NULL;

    return nouveau;   //Retour la carte allouée
}
/*********************************************************************************************************/



/**< CREATION ET ALLOCATION D'UNE STRUCTURE CHEMIN>*******************************************************/
//alloue dynamiquement et initialise les cartes chemin
t_chemin * allocInitChemin()
{
    t_chemin * nouveau = NULL; //création

    nouveau = (t_chemin*)malloc(sizeof(t_chemin));

    //allocation
    if(nouveau == NULL)      //Blindage pour vérifier le succès de l'allocation
    {
        sortieErreur("Problème d'allocation d'une carte chemin");
    }

    //initialisation des variables
    nouveau->haut = 0;
    nouveau->bas = 0;
    nouveau->gauche = 0;
    nouveau->droite = 0;
    nouveau->estCeImpasse = 0;

    return nouveau;
}
/*********************************************************************************************************/



/**< CREATION ET ALLOCATION D'UNE STRUCTURE OBJET>********************************************************/

//Alloue dynamiquement et initialise les cartes objets (cassés et réparés)
t_objet * allocInitObjet()
{
    t_objet * nouveau = NULL; //création

    nouveau = (t_objet *) malloc(sizeof(t_objet)); //allocation


    if(nouveau == NULL)      //Blindage pour vérifier le succès de l'allocation
    {
        sortieErreur("Problème d'allocation d'une carte chemin");
    }

    //ini_tialisation des variables
    nouveau->chariot = 0;
    nouveau->lampe = 0;
    nouveau->pioche = 0;

    nouveau->estCeReparation = 0;

    return nouveau;
}
/*********************************************************************************************************/



/**< ALLOCATION D UNE STRUCTURE DE CARTE PIOCHE : PILE>***************************************************/
//alloue dynamiquement et initialisatialise les structures de la pile de carte
t_cartePioche * allocInitPioche()
{
    t_cartePioche * nouveau = NULL; //Création

    nouveau = (t_cartePioche*)malloc(sizeof(t_cartePioche));

     if(nouveau == NULL)      //Blindage pour vérifier le succès de l'allocation
    {
        sortieErreur("Problème d'allocation d'une carte chemin");
    }

    //Initialisation des varaibles
    nouveau->typeDeCarte = NULL;
    nouveau->suivante = NULL;

    return nouveau;
}
/*********************************************************************************************************/



/**<  CREATION D'UN TAB DE CARTE POUR ENSUITE MELANGER>***************************************************/
t_cartePioche * creationListeCarte(t_partie  *game)
{
    //Déclaration des variabmes
    int i;
    t_carte * tableau[NBCARTE];
    t_cartePioche * ancre = NULL;

    //Allocation de toutes les cases d'un tableau
    for(i=0; i<NBCARTE; i++)
    {
        tableau[i] = allocInitCarte();
    }
    //Remplissage de toutes les cartes du tableau

    creationCarteChemin(tableau, game);

    creationCarteAction(tableau, game);


    //Mélange des cartes et création d'une pile
    ancre = empilement(tableau);
   // verificationPioche(ancre);



    return ancre;  //Retour de l'ancre(haut de la pile de cartes)
}
/*********************************************************************************************************/



/**< CREATION DES CARTES CHEMIN>**************************************************************************/
void creationCarteChemin(t_carte *tableau[NBCARTE], t_partie *game)
{
    int i;

    for(i=0; i<NBCHEMIN; i++)
    {
        tableau[i]->estCeChemin = 1;  //On précise dans la structure carte que c'est un chemin
    }

    //Chaque structure carte de 0 à 40 pointe sur une structure chemin que l'on créé et remplie
    //Il y a 9 possibilités de chemins différents selon les directions (9 pour les impasses et 7 pour les vrais chemins)
    //On envoie au sous programme de création de structure le type pour qu'il initie les bons bouleens direction

    for(i=0; i< 9; i++)   //d'abord les impasses
    {
        tableau[i]->carteChemin = creationPassages(i);
        tableau[i]->carteChemin->estCeImpasse = 1;
    }
    tableau[0]->illustration = game->mesImages.impasse_4;
    tableau[1]->illustration = game->mesImages.impasse_3_1;
    tableau[2]->illustration = game->mesImages.impasse_3_2;
    tableau[3]->illustration = game->mesImages.impasse_2_3;
    tableau[4]->illustration = game->mesImages.impasse_2_4;
    tableau[5]->illustration = game->mesImages.impasse_2_1;
    tableau[6]->illustration = game->mesImages.impasse_2_2;
    tableau[7]->illustration = game->mesImages.impasse_1_2;
    tableau[8]->illustration = game->mesImages.impasse_1_1;

    for(i=9; i<14; i++)   //Les carrefours à 4 branches
    {
        tableau[i]->carteChemin = creationPassages(0);
        tableau[i]->illustration = game->mesImages.cheminX;
    }
    for(i=14; i<19; i++)   //Carrefour a 3 branches 1
    {
        tableau[i]->carteChemin = creationPassages(1);
        tableau[i]->illustration = game->mesImages.cheminTHaut;
    }
    for(i=19; i<24; i++)    //Carrefour a 3 branches 2
    {
        tableau[i]->carteChemin = creationPassages(2);
        tableau[i]->illustration = game->mesImages.cheminTLong;
    }
    for(i=24; i<28; i++)   //carrefour a 2 branches en angle 1
    {
        tableau[i]->carteChemin = creationPassages(3);
        tableau[i]->illustration = game->mesImages.cheminL_1;
    }
    for(i=28; i<33; i++)    //carrefour a 2 branches en angle 2
    {
        tableau[i]->carteChemin = creationPassages(4);
        tableau[i]->illustration = game->mesImages.cheminL_2;
    }
    for(i=33; i<36; i++)    //Carrefour a deux branches tout droit 1
    {
        tableau[i]->carteChemin = creationPassages(5);
        tableau[i]->illustration = game->mesImages.cheminDroitLong;
    }
    for(i=36; i<40; i++)     //Carrefour a deux branches tout droit 2
    {
        tableau[i]->carteChemin = creationPassages(6);
        tableau[i]->illustration = game->mesImages.cheminDroitHaut;
    }

}
/*********************************************************************************************************/



/**< CARTE CHEMIN : REMPLISSAGE DES BOOLEENS>*************************************************************/
t_chemin * creationPassages(int type)
{
    t_chemin * myWay = NULL;
    myWay = allocInitChemin();    //allocation de la structure

    if(type == 0 || type ==1 || type ==2 || type ==4 || type ==6 )  //Chemin vers le haut
    {
        myWay->haut =1;
    }
    if(type == 0 || type ==2 || type ==3 || type ==6 || type ==7 )   //Chemin vers le bas
    {
        myWay->bas =1;
    }
    if(type == 0 || type ==1 || type ==2  || type ==5 || type ==8 )  //CHemin vers la gauche
    {
        myWay->gauche =1;
    }
     if(type == 0 || type ==1 || type ==3 || type ==4 || type ==5 )  //Chemin vers la droite
    {
        myWay->droite =1;
    }

    return myWay;
}
/*********************************************************************************************************/



/**< CREATION DES CARTES ACTION>**************************************************************************/
void creationCarteAction(t_carte * tableau[NBCARTE], t_partie *game )
{
    int i;  //on remplie les 67 cartes

    for(i=NBCHEMIN; i<NBCHEMIN+6; i++)    //Cartes carte(pour regarder les cartes arrivées)
    {
        tableau[i]->estCeMap = 1; //Modification de l'état du booléen
        tableau[i]->illustration = game->mesImages.carte;
    }
    for(i=NBCHEMIN+6; i<NBCHEMIN +9; i++)    //Cartes explosion
    {
        tableau[i]->estCeEboulement = 1;
        tableau[i]->illustration = game->mesImages.couloirExplose;
    }
    for(i=NBCHEMIN + 9; i<NBCARTE; i++)    //Cartes objets
    {
        tableau[i]->estCeObjet = 1;  //On met le booléen à un
    }


    //On remplie toutes les cartes chemin (9 types de cartes)
    for(i=NBCHEMIN+9; i<NBCHEMIN + 12; i++)    //piege
    {
        tableau[i]->carteObjet = creationObjet(1);  //Appel d'un sous programme qui renvoie une structure objet avec les boolens remplis selon le type d'objet
        tableau[i]->illustration = game->mesImages.piege;
    }
    for(i=NBCHEMIN+12; i<NBCHEMIN + 15; i++)            //blessure
    {
        tableau[i]->carteObjet = creationObjet(2); //Envoi par paramètre du type
        tableau[i]->illustration = game->mesImages.blessure;
    }
    for(i=NBCHEMIN+15; i<NBCHEMIN + 18; i++)    //dark
    {
        tableau[i]->carteObjet = creationObjet(3);
        tableau[i]->illustration = game->mesImages.pasLumiere;
    }
    for(i=NBCHEMIN+18; i<NBCHEMIN + 20; i++)    //pas piege
    {
        tableau[i]->carteObjet = creationObjet(4);
        tableau[i]->illustration = game->mesImages.liberation;
    }
    for(i=NBCHEMIN+20; i<NBCHEMIN + 22; i++)    //guerison
    {
        tableau[i]->carteObjet = creationObjet(5);
        tableau[i]->illustration = game->mesImages.guerison;
    }
    for(i=NBCHEMIN+22; i<NBCHEMIN + 24; i++)
    {
        tableau[i]->carteObjet = creationObjet(6);
        tableau[i]->illustration = game->mesImages.lampe;
    }
    tableau[NBCHEMIN + 24]->carteObjet = creationObjet(7);  //blessure + piege
    tableau[NBCHEMIN + 24]->illustration = game->mesImages.guerison_liberation;

    tableau[NBCHEMIN + 25]->carteObjet = creationObjet(8);   //blessure + lampe
    tableau[NBCHEMIN + 25]->illustration = game->mesImages.guerison_lampe;

    tableau[NBCHEMIN + 26]->carteObjet = creationObjet(9);  //piege + lampe
    tableau[NBCHEMIN + 26]->illustration = game->mesImages.liberation_lampe;
}
/*********************************************************************************************************/



/**< CARTES ACTIONS : REMPLISSAGE DES BOOLEENS>***********************************************************/

t_objet * creationObjet(int type)
{
    t_objet * myWay = NULL; //Création de la structure objet
    myWay = allocInitObjet();    //allocation de la structure

    if( type ==1 || type ==4 || type ==7 || type ==9 )  //piege
    {
        myWay->pioche =1;
    }
    if(type == 2 || type ==5 || type ==7 || type ==8 )  //blessure
    {
        myWay->chariot = 1;
    }
    if(type == 3 || type ==6 || type ==8|| type ==9 )  //lampe
    {
        myWay->lampe =1;
    }
     if(type >=4 && type <= 9 )  //Réparation
    {
        myWay->estCeReparation =1;
    }

    return myWay;
}
/*********************************************************************************************************/



/**< CREATION ET D'UNE PILE DYNAMIQUE DE CARTES>**********************************************************/
t_cartePioche * empilement(t_carte * tableau[NBCARTE])
{
    //Déclaration des variables
    int i, alea;
    t_cartePioche * nouveau = NULL;
    t_cartePioche * precedent = NULL;

    //Pour toutes les cartes du tableau
    for(i=0; i<NBCARTE; i++)
    {
        nouveau = allocInitPioche();  //On créer une structure cate de pioche
        do
        {
            alea = rand()%NBCARTE;   //On sélectionne aléatoirement une case dans le tableau tant que celle ci est vide
        }while(tableau[alea] == NULL);

        nouveau->typeDeCarte = tableau[alea];  //on attribue une carte à notre structure carte de pioche
        tableau[alea] = NULL;  //On vid ela case du tableau dont on a utilisé la carte


        nouveau->suivante = precedent;  //la nouvelle carte pointe sur la précédente
        precedent = nouveau;    //incrémentation
    }

    return nouveau; //on retourne l'ancre
}
/*********************************************************************************************************/


