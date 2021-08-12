#include "header.h"

/**< SETUP ALLEGRO >**************************************************************************************/
void setupAllegro ()
{
    allegro_init();                                                         //initialisation allegro

    install_keyboard();                                                     //installation du clavier
    install_mouse();                                                        //installation de la souris

    set_color_depth(desktop_color_depth());                                 //palette de couleur

    if((set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1920, 1080, 0, 0))!=0)        //paramètres fenêtre
    {
        //problème avec fenêtre allegro
        allegro_exit();
        exit(EXIT_FAILURE);                                                    //fermeture allegro
        allegro_message("Pb de mode graphique");
    }

    //Initialisation du son
    if (install_sound(DIGI_AUTODETECT, MIDI_NONE, 0) != 0)
    {//si échec de l'initialisation, message d'erreur
        allegro_message("Erreur : %s", allegro_error);
    }

}
/*********************************************************************************************************/


/**< CHARGEMENT DE TOUT LES CHEMINS >*********************************************************************/
t_tabImage chargerTabImage ()
{
    ///Déclration
    t_tabImage monTabImage;


    ///Chargement de la struct de bitmaps
    monTabImage.cheminDroitHaut = chargerImage("data/images/chemins/I_1.bmp");
    monTabImage.cheminDroitLong = chargerImage("data/images/chemins/I_2.bmp");

    monTabImage.cheminL_1 = chargerImage("data/images/chemins/L_1.bmp");
    monTabImage.cheminL_2 = chargerImage("data/images/chemins/L_2.bmp");

    monTabImage.cheminTHaut = chargerImage("data/images/chemins/T_1.bmp");
    monTabImage.cheminTLong = chargerImage("data/images/chemins/T_2.bmp");

    monTabImage.cheminX = chargerImage("data/images/chemins/X.bmp");

    monTabImage.impasse_1_1 = chargerImage("data/images/chemins/impasse_1.1.bmp");
    monTabImage.impasse_1_2 = chargerImage("data/images/chemins/impasse_1.2.bmp");

    monTabImage.impasse_2_1 = chargerImage("data/images/chemins/impasse_2.1.bmp");
    monTabImage.impasse_2_2 = chargerImage("data/images/chemins/impasse_2.2.bmp");
    monTabImage.impasse_2_3 = chargerImage("data/images/chemins/impasse_2.3.bmp");
    monTabImage.impasse_2_4 = chargerImage("data/images/chemins/impasse_2.4.bmp");

    monTabImage.impasse_3_1 = chargerImage("data/images/chemins/impasse_3.1.bmp");
    monTabImage.impasse_3_2 = chargerImage("data/images/chemins/impasse_3.2.bmp");

    monTabImage.impasse_4 = chargerImage("data/images/chemins/impasse_4.bmp");

    monTabImage.carte = chargerImage("data/images/actions/carte.bmp");
    monTabImage.couloirExplose = chargerImage("data/images/actions/explosion.bmp");

    monTabImage.blessure = chargerImage("data/images/actions/blessure.bmp");
    monTabImage.guerison = chargerImage("data/images/actions/guerison.bmp");

    monTabImage.pasLumiere = chargerImage("data/images/actions/pasLumiere.bmp");
    monTabImage.lampe = chargerImage("data/images/actions/lampe.bmp");

    monTabImage.piege = chargerImage("data/images/actions/piege.bmp");
    monTabImage.liberation = chargerImage("data/images/actions/liberation.bmp");

    monTabImage.guerison_liberation = chargerImage("data/images/actions/guerison_liberation.bmp");
    monTabImage.guerison_lampe = chargerImage("data/images/actions/guerison_lampe.bmp");
    monTabImage.liberation_lampe = chargerImage("data/images/actions/liberation_lampe.bmp");


    return monTabImage;
}
/*********************************************************************************************************/



