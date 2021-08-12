#include "header.h"

/**< CHARGER MUSIQUE >*************************************************************************************/
SAMPLE * chargerMusique(const char * filename)
{
    //Déclarations
    SAMPLE* musique;

    musique = load_sample(filename);                                        //chargement du fichier son

    if (musique == NULL)
    {//le pointeur de sample ne pointe pas correctement sur le fichier
        allegro_message("Erreur de chargement de la musique : %s", filename);         //message d'erreur
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    return musique;                                                         //retourne le pointeur sur fichier son
}
/*********************************************************************************************************/



/**< CHARGER D'UNE IMAGE >********************************************************************************/
BITMAP * chargerImage (const char * filename)
{
    //Déclarations
    BITMAP * image;

    image = load_bitmap(filename, NULL);                                    //chargement de l'image

    if (image == NULL)
    {//le pointeur d'image ne pointe pas correctement sur le fichier
        allegro_message("Erreur de chargement de l'image : %s", filename);                        //message d'erreur
        allegro_exit();                                                     //fermeture allegro
        exit(EXIT_FAILURE);
    }

    return image;
}
/*********************************************************************************************************/



/**< CREER UNE BITMAP >***********************************************************************************/
BITMAP * creerBuffer (int largeur, int hauteur, const char * filename)
{
    //Déclarations
    BITMAP * image;

    image = create_bitmap(largeur, hauteur);                                    //chargement de l'image

    if (image == NULL)
    {//le pointeur d'image ne pointe pas correctement sur le fichier
        allegro_message("Erreur de creation bitmap : %s", filename);                        //message d'erreur
        allegro_exit();                                                     //fermeture allegro
        exit(EXIT_FAILURE);
    }

    return image;
}
/*********************************************************************************************************/



/**< CHARGER UN FICHIER >*********************************************************************************/
FILE * chargerFichier (const char * filename, const char * typeOuverture)
{
    FILE * fichier;

    fichier = fopen(filename, typeOuverture);

    if (fichier == NULL)
    {//le pointeur sur fichier ne pointe pas correctement sur le fichier
        allegro_message("Erreur d'ouverture du fichier : %s", filename);                         //message d'erreur
        allegro_exit();                                                                         //fermeture allegro
        exit(EXIT_FAILURE);
    }

    return fichier;
}
/*********************************************************************************************************/

/**< CHARGER UNE POLICE >*********************************************************************************/
FONT * chargerPolice (const char * filename)
{
    FONT * maPolice;

    maPolice = load_font(filename, NULL, NULL);

    if(maPolice == NULL)
    {
        allegro_message("Erreur de chargement de la police : %s", filename);
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    return maPolice;
}
/*********************************************************************************************************/


/**< SAUVER UNE BITMAP >**********************************************************************************/
void saveBitmap(BITMAP * bitmapASauver, const char * filename)
{
    //Déclarations
    int verifSave;

    verifSave = save_bmp(filename, bitmapASauver, NULL);                         //sauvegarde du bitmap
    //la fct save_bitmap rend un entier différent de 0 si une erreur s'est produite

    if(verifSave != 0)
    {//en cas d'erreur dans la sauvegarde
        allegro_message("Erreur dans la sauvegarde de la bitmap : %s", filename);   //message d'erreur
        allegro_exit();                                                             //fermeture allegro
        exit(EXIT_FAILURE);                                                         //sortie forcée du programme
    }
}
/*********************************************************************************************************/


/**< PROCEDURE DE SORTIE EN CAS D'ERREUR>*****************************************************************/
void sortieErreur (const char * phrase)
{
    allegro_exit();
    allegro_message(phrase);
    exit(EXIT_FAILURE);
}
/*********************************************************************************************************/


/*********************************************************************************************************/
//Affichage des coordonées du curseur sur l'écran (uniquement utile pour le dev)
void afficherCoord ()
{
        textprintf_ex(screen, font, 400, 30, makecol(255, 255, 255), makecol(0,0,0), "Position souris X : %d ", mouse_x);    //affichage pos X souris
        textprintf_ex(screen, font, 400, 50, makecol(255, 255, 255), makecol(0,0,0), "Position souris Y : %d ", mouse_y);    //affichage pos Y souris
}
/*********************************************************************************************************/
