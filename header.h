/** NOTES
*
*
**/



/**< PREPROCESSOR >***************************************************************************************/
#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
/*********************************************************************************************************/


/**< LIBRAIRIES >*****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <allegro.h>
#include <unistd.h>
#include <ctype.h>
/*********************************************************************************************************/


/**< DEFINE >*********************************************************************************************/
#define NBCARTE 67
#define NBCHEMIN 40
#define NBOBJET 17

#define NBMAIN 6

#define PLATEAU_LIGNE 9
#define PLATEAU_COLONNE 11

#define CASE_PLATEAU_W 140
#define CASE_PLATEAU_H 92

#define CARTE_PLATEAU_W 138
#define CARTE_PLATEAU_H 88

#define CARTE_PIOCHE_W 208
#define CARTE_PIOCHE_H 135

#define PIOCHE_X 15
#define PIOCHE_Y 15

#define DEFAUSSE_X 15
#define DEFAUSSE_Y 170

#define CARTE_DEPART_X 419
#define CARTE_DEPART_Y 373

#define CARTE_ARRIVEE_X_MIN 1544
#define CARTE_ARRIVEE_X_MAX 1682

#define CARTE_ARRIVEE_Y_1 189
#define CARTE_ARRIVEE_Y_2 372
#define CARTE_ARRIVEE_Y_3 558

#define CARTE_MAIN_X 600
#define CARTE_MAIN_Y 870

#define POSITION_AVATAR_1_X 210
#define POSITION_AVATAR_1_Y 810

#define POSITION_AVATAR_2_Y 1080
/*********************************************************************************************************/


/**< STRUCTURES >*****************************************************************************************/
//Structure de coordonées
typedef struct coord
{
    int posX;
    int posY;

}t_coord;

//Structure pour element d'animation
typedef struct element
{
    int vitesse;
    int distanceY;
    int phase;
    int enLAir;
    t_coord position;

} t_element;

//Structure pour images animées
typedef struct animations
{
    int vitesseX;
    int vitesseY;
    t_coord position;
    BITMAP * illustration;

} t_animation;


//Structure de chemin
typedef struct chemin
{
  int estCeImpasse;             //si ce booléen est à 1 c'est une impasse -> donc les booléens suivants devient le côté d'entrée de l'impasse sur la carte

  int gauche;                   //entrée gauche
  int droite;                   //entrée droite
  int haut;                     //entrée haut
  int bas;                      //entrée bas

}t_chemin;


//Structure d'objet
typedef struct objet
{
  int estCeReparation;

  int pioche;                      //si ce booléen est à 1, cette carte objet implique la pioche
  int lampe;                       //si ce booléen est à 1, cette carte objet implique la lampe
  int chariot;                     //si ce booléen est à 1, cette carte objet implique le wagon

}t_objet;


//Structure pour une carte de la pioche
typedef struct carte
{
  int estCeChemin;                  //si ce booléen est à 1, c'est une carte chemin
  int estCeObjet;                   //si ce booléen est à 1, c'est une carte objet cassé ou réparé
  int estCeMap;                     //booléen de carte, permet si à 1 de découvrir une des arrivée
  int estCeEboulement;              //si ce booléen est à 1, c'est une carte eboulement

  t_chemin * carteChemin;           //la struct est allouée si la carte est un chemin
  t_objet * carteObjet;             //la struct est allouée si la carte est objet

  BITMAP * illustration;            //bitmap d'illustration

  t_coord myCoord;                  //structure de coordonnées

}t_carte;


//Structure du joueur pour une partie
typedef struct joueur
{
  t_carte * mainJoueur [6];         //tableau de 6 carte de la main du joueur

  t_objet objetsJoueurs;            //structure de booléens des objets du joueur si ces booléens sont à 1 le joueur peut avancer

  int role;                         //booléen pour savoir si saboteur ou mineur
    int gain;

t_coord myCoord;

}t_joueur;


//Structure d'identité d'un joueur
typedef struct identite
{
  char nomAvatar [15];              //pseudo du joueur chaine de 15char
  char pseudo[15];                  //phrase dynamique de pseudo du joueur

  int score;                        //score du joueur

  t_joueur * profilJoueur;          //la struct est allouée à chaque début de manche et free au bout de la manche

  BITMAP * avatar;                  //bitmap d'avatar
  BITMAP * carte;

  struct identite * suivant;        //pt sur suivant

}t_identite;


//Structure pour une carte de la pioche (->PILE)
typedef struct cartePioche
{
  t_carte * typeDeCarte;            //structure de type de carte

  struct cartePioche *suivante;     //pointeur sur carte suivante

}t_cartePioche;


//Strcutre pour charger toutes les images de cartes
typedef struct images
{
    BITMAP * cheminDroitHaut;
    BITMAP * cheminDroitLong;

    BITMAP * cheminX;

    BITMAP * cheminTHaut;
    BITMAP * cheminTLong;

    BITMAP * cheminL_1;
    BITMAP * cheminL_2;

    BITMAP * impasse_1_1;
    BITMAP * impasse_1_2;

    BITMAP * impasse_2_1;
    BITMAP * impasse_2_2;
    BITMAP * impasse_2_3;
    BITMAP * impasse_2_4;

    BITMAP * impasse_3_1;
    BITMAP * impasse_3_2;

    BITMAP * impasse_4;

    BITMAP * carte;
    BITMAP * couloirExplose;

    BITMAP * piege;
    BITMAP * liberation;

    BITMAP * blessure;
    BITMAP * guerison;

    BITMAP * pasLumiere;
    BITMAP * lampe;

    BITMAP * guerison_liberation;
    BITMAP * guerison_lampe;
    BITMAP * liberation_lampe;

    BITMAP * dosCartes;


}t_tabImage;


//Structure de partie
typedef struct partie
{
    t_identite * ancreJoueur;       //ancre de liste chainée de joueur

    t_cartePioche * ancrePioche;    //ancre de la pioche (pile)

    t_carte * plateau [PLATEAU_COLONNE][PLATEAU_LIGNE];

    int tour;                       //nombre de tours
    int nbJoueur;                   //nombre de joueurs
    int compteurPassageTour;

    BITMAP * buffer;

    t_tabImage mesImages;           //strcture contenant ttes les images fréquement utilisées

}t_partie;
/*********************************************************************************************************/



/**< PROTOTYPES >*****************************************************************************************/
///SETUP
void setupAllegro();

t_tabImage chargerTabImage();
///



///JOUEURS
int nombreJoueur();

t_joueur * allocInitJoueurs ();
t_joueur * initialisationJoueurs();

t_identite * allocInitIdentite();
t_identite * listeProfils(int nbJoueurs);
t_identite * creationIdentite(int numJoueur, t_identite * ancre);

BITMAP * selectionAvatar(t_identite * IDJoueur, t_identite * ancre, int numJoueur);

void saisieNom(t_identite * nouveau, int numJoueur, t_identite * ancre);


///



///PARTIE
t_partie * debutPartie();
t_partie * jouerPartieSauvegardee ();

void animationRole(t_partie * maPartie);
///



///ACTION JOUEUR
t_carte * piocherCarte (t_carte * maCarte, t_partie *maPartie, int nombre);

void triMain (t_identite * monJoueur);
void affichageTour (BITMAP * doublebuffer, BITMAP * buffer, t_partie * maPartie,  t_identite * monJoueur);
void affichageDebutTour (BITMAP * doublebuffer, BITMAP * buffer, t_partie * maPartie, t_identite * monJoueur);
void pause (BITMAP * doublebuffer, BITMAP * buffer, t_partie * maPartie, t_identite * monJoueur, int * sortie, int * resultat);


int reparerObjet (t_objet * monObjet, t_identite * monJoueur);
int utiliserDefause(int nbDefausse, int i, t_identite * monJoueur);
int casserObjet (t_objet * monObjet, t_identite * monJoueur, t_partie * maPartie);
int utiliserObjet(t_partie * maPartie, t_identite * monJoueur, char phrase [100], int i);
int placerChemin (t_partie * maPartie, t_identite * monJoueur, char phrase [100], int i);
int utiliserEboulement(t_partie * maPartie, t_identite * monJoueur, char phrase [100], int i);
int tourJoueur (t_partie * maPartie, t_identite * monJoueur, BITMAP * doublebuffer, BITMAP * buffer);
int utiliserCarte(BITMAP * doublebuffer, BITMAP * buffer, t_partie * maPartie, t_identite * monJoueur, int i);
///



///MENU
void wiki ();

t_partie * menuPartie();

int affichageMenu(SAMPLE * musique);
///



///PIOCHE
t_carte * allocInitCarte();

t_chemin * allocInitChemin();
t_chemin * creationPassages(int type);

t_objet * allocInitObjet();
t_objet * creationObjet(int type);

t_cartePioche * allocInitPioche();
t_cartePioche * creationListeCarte(t_partie  *game);
t_cartePioche * empilement(t_carte * tableau[NBCARTE]);

void creationCarteChemin(t_carte *tableau[NBCARTE], t_partie *game);
void creationCarteAction(t_carte *tableau[NBCARTE], t_partie * game);
///



///PLATEAU
void chargerPlateau (t_partie * maPartie);
void initialisationPlateau (t_partie * maPartie);
void carteGrise(BITMAP *monImage, BITMAP * imageFin );
void affichageJoueursPlateau(t_partie * maPartie, t_identite * monJoueur);
void afficherMainJoueur(t_partie * maPartie, BITMAP * doublebuffer, t_identite * monJoueur);
///



///DRAG AND DROP
t_carte * rotationCarte(t_carte * maCarte);

void dragAndDrop(int carteABouger, t_partie * maPartie, t_identite * monJoueur);
void retourPosInit( t_identite * monJoueur, t_partie * maPartie, int i, char phrase[100] );
///



///VERIFICATIONS
int verifObjets (t_joueur * leJoueur);
int verifArrivee (t_partie * maPartie);
int appuieCarte(int x1, int y1, int x2, int y2);
int appuieBouton(int x1, int y1, int x2, int y2);
int testPositionCarte (t_carte *maCarte, int x1, int x2, int y1, int y2);
int verfiConnexionChemin(int j, int k, t_carte * plateau[PLATEAU_COLONNE][PLATEAU_LIGNE], t_carte * maCarte);

void verificationPioche(t_cartePioche * parcours);
void verifPossibiliteJeu(t_partie * maPartie, t_identite * monJoueur, int tourPasse);
///


///ANIMATIONS
void animPiege(t_partie * maPartie, t_identite * monJoueur);
void animLumiere(t_partie * maPartie, t_identite * monJoueur);
void animBlessure(t_partie * maPartie, t_identite * monJoueur);
void animEboulement(t_partie * maPartie, t_identite * monJoueur, int i);
///


///VICTOIRE
t_coord deplacementSaboteur (t_animation oiseau);

void gameOver();
void sauvegardeGagnant (t_partie * maPartie);
void victoireAssocies(t_partie * maPartie, t_identite * gagnant);
void attributionOr (t_partie * maPartie, t_identite * monJoueur);
void victoireSaboteur(t_partie * maPartie, t_identite * monJoueur);
void collisionChercheurs (t_animation * chercheur1, t_animation * chercheur2);
void animDeplacement (t_partie * maPartie, t_identite * monJoueur, t_coord coordCourant);

int animTestArrivee (t_partie * maPartie, t_identite * monJoueur, BITMAP * doublebuffer, BITMAP * buffer);
///



///SAUVEGARDE
t_identite * recupID();
t_identite * chargerID ();
t_identite * recupJoueurs(t_partie * maPartie);

t_cartePioche * recupPioche(t_partie *maPartie);

t_carte * recupCarte (FILE * fichiers, t_partie * maPartie);

BITMAP * recupAvatar (t_identite * monID);
BITMAP * recupObjet(t_carte * maCarte, t_partie * maPartie);
BITMAP *recupChemin (t_carte * maCarte, t_partie * maPartie);

int recupNBJoueur ();

void lireHistoParties(SAMPLE * musique);
void recupPlateau (t_partie * maPartie);
void sauvegardeTout (t_partie * maPartie);
void sauvegardeJoueurs(t_partie * maPartie);
void sauvegardePioche(t_cartePioche * ancre);
void sauvegardeNBJoueur (t_partie * maPartie);
void chargerAfficherSauv(int posListe);
void sauvegardeMain (t_joueur * monJoueur, FILE * fichier);
void sauvegardeIdentite (t_identite * monJoueur, FILE * maSauvegarde);
void sauvegardePlateau (t_carte * plateau[PLATEAU_COLONNE][PLATEAU_LIGNE]);
///


///MES FONCTIONS
FONT * chargerPolice(const char * filename);

SAMPLE * chargerMusique(const char * filename);

BITMAP * chargerImage(const char * filename);
BITMAP * creerBuffer(int largeur, int hauteur, const char * filename);

FILE * chargerFichier(const char * filename, const char * typeOuverture);

void afficherCoord();
void sortieErreur(const char * phrase);
void saveBitmap(BITMAP * bitmapASauver, const char * filename);
///


/*********************************************************************************************************/
#endif // HEADER_H_INCLUDED
