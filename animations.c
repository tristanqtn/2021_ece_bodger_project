#include "header.h"

/**< ANIMATION EBOULEMENT / EXPLOSION >*******************************************************************/
void animEboulement(t_partie * maPartie, t_identite * monJoueur, int i)
{
    SAMPLE * boum = chargerMusique("data/sons/explosion.wav");
    BITMAP * explosif = chargerImage("data/images/actions/dynamite.bmp");
    BITMAP * explosion = chargerImage("data/images/actions/explosionAnimation.bmp");
    BITMAP * doublebuffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");
    BITMAP * buffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");
    BITMAP * imageTournee  = creerBuffer(explosif->h, explosif->h, "rotation dynamite");
    BITMAP * dynamite  = creerBuffer(explosif->h, explosif->h, "rotation dynamite");
    int endX, endY, size;
    int x = 220, y = 820;
    int vitesse = 10, angle = 0;

    clear_bitmap(dynamite);                                                                     //creation d'une BITMAP carre pour la rotation
    rectfill(dynamite, 0, 0, imageTournee->w, imageTournee->h, makecol(255, 0, 255));           //bitmap transparent
    blit(explosif, dynamite, 0, 0, dynamite->w / 2 - explosif->w / 2, dynamite->h / 2 - explosif->h/ 2, explosif->w, explosif->h );


    endX =  monJoueur->profilJoueur->mainJoueur[i]->myCoord.posX ;              //coordonnes de la carte a exploser
    endY = monJoueur->profilJoueur->mainJoueur[i]->myCoord.posY ;

    monJoueur->profilJoueur->mainJoueur[i] = NULL;                              //suppression de la carte de la main

    while(x != endX  - CARTE_PLATEAU_W/2|| y != endY  - CARTE_PLATEAU_H/2)
    {
        angle = (angle + 5) % 255;  //incrementation de l'angle de rotation

        //incrementation de la composante horizontale
        if(x >= endX - CARTE_PLATEAU_W/ 2 )             //condition du placement de fin
        {
            x = endX - CARTE_PLATEAU_W/2;
        }
        else
        {
            x += vitesse * 1.5;   //incrementation (plateau plus large que haut
        }

        //incrementation de la composante verticale
        if(y <= endY  - CARTE_PLATEAU_H/2 )
        {
            y = endY - CARTE_PLATEAU_H/2;               //condition du placement de fin
        }
        else
        {
            y -= vitesse;  //incrementation
        }
        clear_bitmap(doublebuffer); //nettoyage buffer partie

        //buffer de l'image tournee
        clear_bitmap(imageTournee);         //suppression
        rectfill(imageTournee, 0, 0, imageTournee->w, imageTournee->h, makecol(255, 0, 255));  //rectangle fushia --> transparence
        rotate_sprite(imageTournee, dynamite, 0, 0, itofix(angle));  //256 est un tour complet   // on tourne k'ancienne bitmap de 10°°


        blit(maPartie->buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage duplateau sauvegardé

        clear_to_color(buffer, makecol(255, 0, 255));
        afficherMainJoueur(maPartie, buffer, monJoueur);  //affichage de la main du joueur
        masked_blit(buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        masked_blit(imageTournee, doublebuffer, 0, 0, x, y, dynamite->w, dynamite->h);

        masked_blit(doublebuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage sur le screen
    }

    play_sample(boum, 255, 128, 1000, 0);//bruitage
    for(size=0; size<50; size++)
    {
         clear_bitmap(doublebuffer); //nettoyage buffer partie

        blit(maPartie->buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage duplateau sauvegardé

        clear_to_color(buffer, makecol(255, 0, 255));
        afficherMainJoueur(maPartie, buffer, monJoueur);  //affichage de la main du joueur
        masked_blit(buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        stretch_sprite(doublebuffer, explosion, endX-(size/2 ), endY - (size * 3), (explosion->w * size) /50, (explosion->h *size )/50 );

        masked_blit(doublebuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage sur le screen
    }

    destroy_bitmap(buffer);
    destroy_bitmap(doublebuffer);
    destroy_bitmap(dynamite);
    destroy_bitmap(explosion);
    destroy_bitmap(explosif);
    destroy_bitmap(imageTournee);
    destroy_sample(boum);
}
/********************************************************************************************************/



/**< ANIMATION PIEGE >***********************************************************************************/
void animPiege(t_partie * maPartie, t_identite * monJoueur)
{
    BITMAP * piege = chargerImage("data/images/actions/filet.bmp");

    BITMAP * doublebuffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");
    BITMAP * buffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");

    SAMPLE * citation = chargerMusique("data/sons/piege.wav");

    int endY;
    int x = 0, y = 0;
    int vitesse = 20;
     //coordonnes du joueur a enfermer
    endY = monJoueur->profilJoueur->myCoord.posY + 20;
                            //suppression de la carte de la main
    play_sample(citation, 180, 128, 1000, 0);
    while(y != endY)
    {
        //incrementation de la composante verticale
        if(y >= endY )
        {
            y = endY ;             //condition du placement de fin
        }
        else
        {
            y += vitesse;  //incrementation
        }

        clear_bitmap(doublebuffer); //nettoyage buffer partie
        blit(maPartie->buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage duplateau sauvegardé

        clear_to_color(buffer, makecol(255, 0, 255));
        afficherMainJoueur(maPartie, buffer, monJoueur);  //affichage de la main du joueur
        masked_blit(buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        stretch_sprite(doublebuffer, piege, x, y, piege->w/5, piege->h);

        masked_blit(doublebuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage sur le screen
    }

    sleep(3);



    destroy_bitmap(buffer);
    destroy_bitmap(doublebuffer);
    destroy_bitmap(piege);
    destroy_sample(citation);
}
/********************************************************************************************************/



/**< ANIMATION LAMPE >***********************************************************************************/
void animLumiere(t_partie * maPartie, t_identite * monJoueur)
{
    int i;
    BITMAP * eclair = chargerImage("data/images/actions/eclair.bmp");
    BITMAP * doublebuffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");
    BITMAP * buffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");
    BITMAP * fondNoir = creerBuffer(SCREEN_W, SCREEN_H, "buffer eclair ");
    SAMPLE * foudre = chargerMusique("data/sons/eclair.wav");

    clear_to_color(buffer, makecol(255, 0, 255));

    afficherMainJoueur(maPartie, buffer, monJoueur);  //affichage de la main du joueur

    play_sample(foudre, 255, 128, 1000, 0);//bruitage
    for(i=0; i<100; i++)
    {
        usleep(10);
        clear_bitmap(doublebuffer); //nettoyage buffer partie
        if(i%2 == 0)
        {

            blit(maPartie->buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage duplateau sauvegardé

            masked_blit(buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        }



        stretch_sprite(doublebuffer, eclair, 0, 0, SCREEN_W, eclair->h);
        masked_blit(doublebuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage sur le screen
    }


    destroy_bitmap(buffer);
    destroy_bitmap(doublebuffer);
    destroy_bitmap(eclair);
    destroy_bitmap(fondNoir);
    destroy_sample(foudre);
}
/********************************************************************************************************/


/**< ANIMATION BLESSURE >********************************************************************************/
void animBlessure(t_partie * maPartie, t_identite * monJoueur)
{
    BITMAP * archer = chargerImage("data/images/actions/arbalette.bmp");
    BITMAP * fleche = chargerImage("data/images/actions/fleche.bmp");
    BITMAP * flecheTournee = chargerImage("data/images/actions/fleche.bmp");

    BITMAP * doublebuffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");
    BITMAP * buffer = creerBuffer(SCREEN_W, SCREEN_H, "double buffer drag and drop");

    SAMPLE * ouch = chargerMusique("data/sons/ouch.wav");

    int endY, endX = 0;
    int x = 1750, y = 700;
    int vitesse = 15;
    int angle = 240;

    //coordonnes du joueur a enfermer
    endY = monJoueur->profilJoueur->myCoord.posY-30;

    while(y != endY || x!= endX)
    {
        if(x>= SCREEN_W/2)
        {
            y -= vitesse*0.8;
            x -= vitesse ;

            if(x>= SCREEN_W/1.5)
            {
                 angle += vitesse/10;
            }
            else
            {
                angle-= vitesse/10;
            }

            angle = angle % 255;
        }
        else
        {
            if(y >= endY)
            {
                y = endY;
            }
            else
            {
                y+= vitesse;
            }

            if(x<= endX)
            {
                x = endX;
            }
            else
            {
                x-= vitesse*1.3;
            }

             if(x >SCREEN_H/4)
            {

                angle -= vitesse/10  ;
                angle = angle % 255;
            }

        }



        clear_bitmap(flecheTournee);
        rectfill(flecheTournee, 0, 0, flecheTournee->w, flecheTournee->h, makecol(255, 0, 255));
        rotate_sprite(flecheTournee, fleche, 0, 0, itofix(angle));  //256 est un tour complet   // on tourne k'ancienne bitmap de 10°°


        clear_bitmap(doublebuffer); //nettoyage buffer partie
        blit(maPartie->buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage duplateau sauvegardé

        clear_to_color(buffer, makecol(255, 0, 255));
        afficherMainJoueur(maPartie, buffer, monJoueur);  //affichage de la main du joueur
        masked_blit(buffer, doublebuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        stretch_sprite(doublebuffer, archer, 1700, 670, archer->w/2, archer->h/2);
        stretch_sprite(doublebuffer, flecheTournee, x, y, flecheTournee->w/6, flecheTournee->h/6);

        masked_blit(doublebuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //affichage sur le screen
    }

    play_sample(ouch, 255, 128, 1000, 0);//bruitage
    sleep(1);
    destroy_sample(ouch);
    destroy_bitmap(buffer);
    destroy_bitmap(doublebuffer);
    destroy_bitmap(archer);
    destroy_bitmap(fleche);
    destroy_bitmap(flecheTournee);
}
/********************************************************************************************************/

