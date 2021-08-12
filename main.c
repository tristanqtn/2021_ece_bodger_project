#include "header.h"

/**< MAIN >***********************************************************************************************/
int main(int argc, char * argv[])
{
    ///DECLARATIONS
    int jeux =1, sortie =0, victoire = 0, gameover =0;

    t_partie * maPartie = NULL;
    t_identite * courant = NULL;
    t_identite * copie = NULL;

    BITMAP * doublebuffer;
    BITMAP * buffer;
    SAMPLE * musique, * generique;



    ///SETUP
    setupAllegro();                                     //initialisation allegro
    srand(time(NULL));                                  //initialisation de l'aléatoire



    musique = chargerMusique("data/sons/musique.wav");  //chargement musique fond
    generique = chargerMusique("data/sons/generique.wav");//chargement generique menu
    doublebuffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");
    buffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");


    play_sample(generique, 180, 128, 1000, 0);//joueur la musique

    while(jeux ==1)
    {
        jeux = affichageMenu(musique);  //affichage du menu et recuperation de la volontée de jouer

        sortie = 0;                     //initialisation de la sortie
        gameover = 0;
        victoire = 0;

        play_sample(musique, 100, 128, 1000, 1);//musique de fond

        if(jeux== 1)
        {
            ///INITIALISATION DE LA PARTIE
            maPartie = menuPartie();                        //récupération des informations d'une partie

            animationRole(maPartie);                        //affichage des roles de la partie

            maPartie->tour = 0;                             //initialisation du compteur de tours
            maPartie->compteurPassageTour = 0;

            clear_bitmap(screen);                           //nettoyage de l'ecran

            courant = maPartie->ancreJoueur;                //initialisation du parcours


            while(sortie == 0 && victoire == 0 && gameover == 0)
            {///BOUCLE DE JEUX
                sortie = tourJoueur(maPartie, courant, doublebuffer, buffer); //tour du joueur

                if(verifArrivee(maPartie) == 1 && sortie != 1)
                {//si un joueur ralie l'arrivée
                    victoire = animTestArrivee(maPartie, courant, doublebuffer, buffer);
                    copie = courant;//stockage du gagnet de la partie
                }

                if(maPartie->compteurPassageTour == maPartie->nbJoueur*2)
                {//les joueurs ont passés 2x leurs tour avec une pioche vide ->game over
                    gameover =1;
                }
                if(key[KEY_G])
                {
                    gameover =1;
                }

                courant = courant->suivant;//parcours de la liste de joueurs
                maPartie->tour++;//compteur de tours
            }

            if(victoire == 1)
            {///ANIMATIONS DE VICTOIRE
                victoire = 0;
                sortie = 0;
                attributionOr(maPartie, copie);

                if(copie->profilJoueur->role == 1)
                {
                    victoireSaboteur(maPartie, copie);
                }
                else
                {
                    ///Victoire mystere associé
                    victoireAssocies(maPartie, copie);
                }
                sauvegardeGagnant(maPartie);
            }
            else if(gameover == 1)
            {
                gameOver();
            }
        }
    }

    destroy_bitmap(doublebuffer);
    destroy_bitmap(buffer);
    destroy_sample(generique);
    destroy_sample(musique);

    free(maPartie);
    free(copie);
    free(courant);


    allegro_exit();                         //fermeture d'allegro

    return 0;
}
END_OF_MAIN();
/*********************************************************************************************************/
