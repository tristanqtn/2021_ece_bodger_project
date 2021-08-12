#include "header.h"

/**< AFFICHAGE DU MENU >**********************************************************************************/
int affichageMenu (SAMPLE * musique)
{
    //Déclarations
    int sortie = 0;
    int etatMusique =1;
    int rendu = 0;

    BITMAP * menuImage;
    BITMAP * soundOff;

    menuImage = chargerImage("data/images/menu/menu2.bmp");       //chargement de l'image de menu
    soundOff = chargerImage("data/images/menu/ligne_soundoff.bmp");//chargement de la musique

    show_mouse(NULL);
    //Affichage ajusté de l'image
    stretch_blit(menuImage, screen, 0,0, menuImage->w, menuImage->h, 0, 0, SCREEN_W, SCREEN_H);
    show_mouse(screen);                                 //affichage de la souris



    while (sortie == 0)
    {//tant que un bouton n'est pas pressé ou que ECHAP n'est pas presée

        if(appuieBouton(870,461,1622,728)== 1)
        {//bouton play appuyé

            show_mouse(NULL);
            destroy_bitmap(soundOff);
            destroy_bitmap(menuImage);

            sortie = 1;
            rendu = 1;
        }

        if(appuieBouton(870,763,1211,994)== 1 )
        {//bouton wiki appuyé

            wiki();
            stretch_blit(menuImage, screen, 0,0, menuImage->w, menuImage->h, 0, 0, SCREEN_W, SCREEN_H);
            show_mouse(screen);                                 //affichage de la souris

        }

        if( appuieBouton(1288,763,1622,994)== 1 )
        {//bouton historique des points appuyé
            lireHistoParties(musique);
            stretch_blit(menuImage, screen, 0,0, menuImage->w, menuImage->h, 0, 0, SCREEN_W, SCREEN_H);
            show_mouse(screen);
        }

        if( appuieBouton(1725,21,1891,248)== 1 )
        {//bouton exit appuyé
            rendu = -1;
            sortie = 1;

            destroy_bitmap(soundOff);
            destroy_bitmap(menuImage);
        }

        if(appuieBouton(28,18,165,151)== 1 )
        {//bouton musique appuyé
            if(etatMusique == 1)
            {//eteignage musique
                stop_sample(musique);
                etatMusique = 0;

                masked_blit(soundOff, screen, 0, 0, 30, 30, soundOff->w, soundOff->h);
            }
            else if(etatMusique == 0)
            {//allumage musique
                play_sample(musique, 100, 128, 1000, 1);            //lancer la musique
                etatMusique = 1;

                show_mouse(NULL);
                stretch_blit(menuImage, screen, 0, 0, menuImage->w, menuImage->h, 0, 0, SCREEN_W, SCREEN_H);
                show_mouse(screen);
            }
        }

    }

    //destroy_sample(musique);
    return rendu;
}
/*********************************************************************************************************/



/**< AFFICHAGE MENU JOUER >*******************************************************************************/
t_partie * menuPartie()
{
    //Déclarations
    BITMAP * fond;
    t_partie * maPartie =NULL;
    int sortie = 0;

    fond = chargerImage("data/images/menu/menuJouer.bmp");    //chargement de l'image du sous menu jouer

    //Affichage ajusté de l'image
    stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

    show_mouse(screen);

    while (sortie == 0)
    {//tant que un bouton n'est pas pressé ou que ECHAP n'est pas presée

        if( appuieBouton(585,249,1338,516)== 1 )
        {//bouton nouvelle partie appuyé
            sortie = 1;                             //sortie de la boucle

            show_mouse(NULL);                       //suppression de la souris sur l'écran
            destroy_bitmap(fond);

            maPartie = debutPartie();                         //appel de la fonction de sélection du nombre de joueurs
        }

        if( appuieBouton(585,635,1338,900)== 1 )
        {
            //bouton partie sauvegardee appuyé
            sortie = 1;                             //sortie de la boucle

            show_mouse(NULL);                       //suppression de la souris sur l'écran
            destroy_bitmap(fond);

            maPartie = jouerPartieSauvegardee();
       }
    }
    return maPartie;
}
/*********************************************************************************************************/


/**< AFFICHAGE DES REGLES DU JEU >************************************************************************/
void wiki ()
{
    ///Déclarations
    BITMAP * fond;

    fond = chargerImage("data/images/menu/wiki_1.bmp");//chargement
    stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H); //affichage

        while(mouse_b != 1)//attente du click
        {rest(5);
        }//tempo
        while(mouse_b ==1)//attente du relachement du clique
        {rest(5);
        }

            fond = chargerImage("data/images/menu/wiki_2.bmp");
        stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

        while(mouse_b != 1)//attente du click
        {rest(5);
        }//tempo
        while(mouse_b ==1)//attente du relachement du clique
        {rest(5);
        }

            fond = chargerImage("data/images/menu/wiki_3.bmp");
        stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

        while(mouse_b != 1)//attente du click
        {rest(5);
        }//tempo
        while(mouse_b ==1)//attente du relachement du clique
        {rest(5);
        }

            fond = chargerImage("data/images/menu/wiki_4.bmp");
        stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

        while(mouse_b != 1)//attente du click
        {rest(5);
        }//tempo
        while(mouse_b ==1)//attente du relachement du clique
        {rest(5);
        }

            fond = chargerImage("data/images/menu/wiki_5.bmp");
        stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

        while(mouse_b != 1)//attente du click
        {rest(5);
        }//tempo
        while(mouse_b ==1)//attente du relachement du clique
        {rest(5);
        }

            fond = chargerImage("data/images/menu/wiki_6.bmp");
        stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

        while(mouse_b != 1)//attente du click
        {rest(5);
        }//tempo
        while(mouse_b ==1)//attente du relachement du clique
        {rest(5);
        }
            fond = chargerImage("data/images/menu/wiki_7.bmp");
        stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

        while(mouse_b != 1)//attente du click
        {rest(5);
        }//tempo
        while(mouse_b ==1)//attente du relachement du clique
        {rest(5);
        }

            fond = chargerImage("data/images/menu/wiki_8.bmp");
        stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

        while(mouse_b != 1)//attente du click
        {rest(5);
        }//tempo
        while(mouse_b ==1)//attente du relachement du clique
        {rest(5);
        }
            fond = chargerImage("data/images/menu/wiki_9.bmp");
        stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

        while(mouse_b != 1)//attente du click
        {rest(5);
        }//tempo
        while(mouse_b ==1)//attente du relachement du clique
        {rest(5);
        }


    destroy_bitmap(fond);
}
/*********************************************************************************************************/


