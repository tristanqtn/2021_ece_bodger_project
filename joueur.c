#include "header.h"

/**< FONCTION DE RECUPERATION DU NOMBRE DE JOUEURS >******************************************************/
int nombreJoueur()
{
    //Déclarations
    int nbJoueur;
    int sortie = 0;
    BITMAP * fond;

    fond = chargerImage("data/images/menu/nombreJoueurs.bmp");    //chargement de l'image du sous menu jouer

    //Affichage ajusté de l'image
    show_mouse(NULL);
    stretch_blit(fond, screen, 0, 0, fond->w, fond->h, 0, 0, SCREEN_W, SCREEN_H);

    show_mouse(screen);

    while (sortie == 0)
    {

        //tant que un bouton n'est pas pressé ou que ECHAP n'est pas presée
        if(appuieBouton(687,407, 1187, 637)== 1)
        {
            //bouton 3j appuyé
            sortie = 1;
            nbJoueur = 3;

        }
        else if(appuieBouton(1293,407, 1793, 637)== 1)
        {
            //bouton 4j appuyé
            sortie = 1;
            nbJoueur = 4;
        }
        else if(appuieBouton(82,407, 579, 637)== 1)
        {
            //bouton 2j appuyé
            sortie = 1;
            nbJoueur = 2;
        }
        else if(appuieBouton(356,695,857,927)== 1)
        {
            //bouton 6j appuyé
            sortie = 1;
            nbJoueur = 5;
        }
        else if(appuieBouton(1063,695,1565,927)== 1)
        {
            //bouton 6j appuyé
            sortie = 1;
            nbJoueur = 6;
        }
    }
    destroy_bitmap(fond);
    return nbJoueur;            //retourne le nombre de joueurs
}
/*********************************************************************************************************/


/**< ORCHESTRE LA CREATION DES PROFILS DES JOUEURS : liste chainée circulaire >***************************/
t_identite * listeProfils(int nbJoueurs)
{
    int i;

    t_identite * courant = NULL;                //Création de deux pointeurs de parcours
    t_identite * precedent = NULL;
    t_identite * ancre = NULL;                  //premier joueur saisi

    for(i=0; i<nbJoueurs; i++)                  //creation de n joueurs
    {
        courant = creationIdentite(i, ancre);          //appel fonction de creation

        if(i==0)
        {
            ancre = courant;                    //si premier saisi, il devien l'ancre
        }
        else
        {
            precedent->suivant = courant;       //sinon on fait pointer le precedent
        }

        precedent = courant;                    //incrémentation des pointeurs de parcours
    }

    courant->suivant = ancre;                   //liste chainée circulaire : le dernier pointe sur le premire

    return ancre;
}
/*********************************************************************************************************/




/**< CREATION D'UNE IDENTITE >****************************************************************************/
t_identite * creationIdentite(int numJoueur, t_identite * ancre)    //ici on choisi de recuperer un ancienr profil ou d'en creer un nouveau
{
    ///Déclarations
    t_identite * nouveau = NULL;
    BITMAP *  nouveauJoueur;
    t_identite * courant = NULL;
    int nbJoueurs, i;
    int recommencer =0;
    int sortie = 0;
    FONT * pixelated28;        //POLICE


    pixelated28 = chargerPolice("data/police/pixelated_28.pcx");   //chargement de la police d'écriture
    nouveauJoueur = chargerImage("data/images/menu/creationJoueur_nouveauOuPas.bmp");    //chargement de l'interface de choix de nouveau joueur


    show_mouse(NULL);
    stretch_blit(nouveauJoueur, screen, 0, 0, nouveauJoueur->w, nouveauJoueur->h, 0, 0, SCREEN_W, SCREEN_H );    //affichage du fond
    show_mouse(screen);

    textprintf_ex(screen, pixelated28, 340, 550, makecol(59, 56, 56), makecol(97, 97, 97), "MYSTERE ASSOCIE %d", numJoueur + 1);  //nième saisie de profil
    courant = ancre;
    nbJoueurs = recupNBJoueur();//recuperation nb de joueurs

    if(nbJoueurs>4)
    {//si le nombre de joueurs sauvegardés excede 4 on reviens à 4
        nbJoueurs= 4;
    }

    while(sortie ==0)
    {//sortie possible  si un profil a été créer
            if(appuieBouton(1017,271,1802,576)== 1)   //Bouton ancien profil
            {
                if(numJoueur<=nbJoueurs)
                {
                sortie =1;
                show_mouse(NULL);
                do
                {//tant que le profil choisi est dejà utilisé
                    nouveau = recupID(nbJoueurs);//recupID
                    nouveau->avatar = recupAvatar(nouveau);//recup avatar
                    for(i=0; i<nbJoueurs; i++)
                    {//parcours nbJoueurs
                        if(courant != NULL)
                        {//si la liste n'est pas vide
                            if(strcmp(courant->nomAvatar, nouveau->nomAvatar) == 0 || strcmp(courant->pseudo, nouveau->pseudo) == 0)
                            {//si l'avatar ou le pseudo est déjà utilisé
                                i=nbJoueurs;//sortie du for
                                recommencer = 1;//on recommence
                                courant = ancre;//on réinitialise le parcours
                                rectfill(screen, 0, 0, SCREEN_W, 20, makecol(180, 0, 0));
                                textout_ex(screen, font, "Ce profil ou cet avatar est deja utilise par un joueur",800, 10, makecol(255, 255, 255),-1);   //affichage d'un message de confirmation
                                sleep(2);
                            }
                            else
                            {
                                recommencer = 0;//on recommence pas
                                if(courant->suivant != NULL)//si le pt suivant est pas null
                                {
                                    courant = courant->suivant;//parcours
                                }
                            }
                        }

                    }
                }while(recommencer == 1);

            }
        }
        else if(appuieBouton(1017,672,1802,976)== 1)    //Bouton nouveau joueur
        {
            show_mouse(NULL);
            nouveau = allocInitIdentite();
            saisieNom(nouveau, numJoueur, ancre );              //appel fonction saisie nouveau joueur
            sortie =1;
        }
    }

    destroy_bitmap(nouveauJoueur);                      //destruction de l'image
    destroy_font(pixelated28);

    return nouveau;
}
/*********************************************************************************************************/

/**< SAISIE NOM ET CHOIX AVATAR >*************************************************************************/
void saisieNom(t_identite * nouveau, int numJoueur, t_identite * ancre)     //ici on créé un nouveau profil de joueur
{
    BITMAP * affichagePseudo;       //buffer de saisie nom
    BITMAP * monImage;              //image de fond
    FONT * pixelated28;        //POLICE
    t_identite * courant = ancre;

    char carac,scancode, pseudo[15];
    int touche, i=0, j, erreur = 0;

    pixelated28 = chargerPolice("data/police/pixelated_28.pcx");   //chargement de la police

    affichagePseudo = creerBuffer(400,100, "Bitmap saisie pseudo");  //création du buffer de saisie de nom


    show_mouse(NULL);
    monImage = chargerImage("data/images/menu/SaisieNomJoueur.bmp");         //affichage image de fond

    blit(monImage, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    textprintf_ex(screen, pixelated28, 340, 550, makecol(59, 56, 56), makecol(97, 97, 97), "MYSTERE ASSOCIE %d", numJoueur + 1);

    show_mouse(screen);

    ///source : https://www.gamedev.net/reference/articles/article2130.asp
    do
    {
        i=0;
        courant = ancre;
        do
        {
            if(keypressed())     //si une touche est appuyée
            {
                touche   = readkey();    //on la stocke dans une variable
                carac    = touche & 0xff;  //si cest un caractere
                scancode = touche >> 8;            //si c'est une touche spéciale

                /* a character key was pressed; add it to the string */
                if(carac >= 32 && carac <= 126)    //si c'est bien un caractère
                {

                    pseudo[i] = toupper(carac);    //on le met en majuscule dans un tableau de caractères
                    pseudo[i+1] = '\0';             //on marque la fin de la chaine de caractere

                    if(i< 13)                       //si il y a encore de la place pour ecrire
                    {
                        i++;                        //on incremente la position de la derniere lettre
                    }  //blindage qui empeche que le nom soit trop long. si oui, la derniere lettre se fait ecraser : il remarque et peut soi s'arreter la soit changer

                }
                else if((scancode == KEY_BACKSPACE) && (i>0))          //si la touche supprimer est pressée
                {
                    i--;
                    pseudo[i] = '\0';             //on supprime le dernier caractère en le remplaçant par la marque de fin de chaine de caractere
                }
            }

            /* all drawing goes here */
            clear(affichagePseudo);               //efface le buffer
            rectfill(affichagePseudo, 0, 0, 400, 100, makecol(97, 97, 97));              // fait un rectangle de la coumeur du fond

            textprintf_ex(affichagePseudo, pixelated28, 10, 10, makecol(59, 56, 56), -1, "%s", pseudo);       //affiche les lettres saisies
            vline(affichagePseudo, i * 17+12, 20, 50, makecol(59, 56, 56));      //affiche un curseur
            masked_blit(affichagePseudo, screen, 0, 0, 1200, 370, 1920, 1080);            //affiche le buffer sur le screen
            clear(affichagePseudo);


        }
        while(!key[KEY_ENTER]);              //appuyer sur enter pour valider la saisie

        rectfill(affichagePseudo, 0, 0, 400, 100, makecol(97, 97, 97));               //affiche le nom final validé
        textprintf_ex(affichagePseudo, pixelated28, 10, 10, makecol(59, 56, 56), -1, "%s", pseudo);
        masked_blit(affichagePseudo, screen, 0, 0, 1200, 370, 1920, 1080);
        clear(affichagePseudo);

        for(i=0; i<numJoueur; i++)    //blindage pseudo identique a un autre joueur
        {
            if(strcmp(courant->pseudo,pseudo)== 0 || pseudo == NULL || pseudo[0] == '\0' || pseudo[0] == ' ')  //si le pseudo est identique à un pseudo precedent
            {
                erreur = 1; //Pour rester dans le while

                //affichages erreur et ecrant
                blit(monImage, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                textprintf_ex(screen, pixelated28, 340, 550, makecol(59, 56, 56), makecol(97, 97, 97), "MYSTERE ASSOCIE %d", numJoueur + 1);
                rectfill(screen, 0, 0, SCREEN_W, 20, makecol(180, 0, 0) );
                textprintf_ex(screen, font, 280, 10, makecol(255, 255, 255), -1, "Ce pseudo est deja utilise par un autre joueur, veuillez en saisir un nouveau");
                //reinitialisation de la chaine de caractere pseudo
                for(j=0; j<15; j++)
                {
                    pseudo[j] = '\0';
                }
                i = numJoueur + 1;
            }
            else
            {
                erreur = 0;  //si tout va bien erreur reste a 0 : on sort des boucles while
                courant = courant->suivant;
            }

        }

    }
    while(erreur == 1);

    strcpy(nouveau->pseudo, pseudo);                       //recopie la variable de nom dans la structure du joueur

    nouveau->avatar = selectionAvatar(nouveau, ancre, numJoueur);             //appel du sous programme du choix d'avatar
    nouveau->score = 0;                                     //initialisation du score à 0


    while(appuieBouton(348, 806, 556, 1010)== 0)     //appuyer sur le bouton OK pour sortir
    {

    }
    clear(screen);

    destroy_bitmap(affichagePseudo);
    destroy_bitmap(monImage);
    destroy_font(pixelated28);

}
/*********************************************************************************************************/


/**< SELECTION D'UN AVATAR >******************************************************************************/
BITMAP * selectionAvatar(t_identite * IDJoueur, t_identite * ancre, int numJoueur)
{
    int choix, erreur = 0;
    BITMAP * perso;
    t_identite * courant = ancre;
    int i, j;

    do
    {
        choix = 0;
        courant = ancre;
        while(choix == 0)    //ici, on attend que le joueur choisisse son avatar pour la partie
        {
            if(appuieBouton(1051, 624, 1185, 811)== 1)
            {
                rect(screen, 1051, 624, 1185, 811, makecol(0, 102, 120) );
                choix = 1;
                perso = chargerImage("data/images/avatars/Daphne_Blake.bmp");
                strcpy(IDJoueur->nomAvatar,"daphne");
            }
            else if(appuieBouton(1048, 828, 1185, 1017)== 1)
            {
                rect(screen, 1048, 828, 1185, 1017, makecol(0, 102, 120) );
                choix = 1;
                perso = chargerImage("data/images/avatars/Mystery_Machine.bmp");
                strcpy(IDJoueur->nomAvatar,"mystery");
            }
            else if(appuieBouton(1246, 764, 1384, 947)== 1)
            {
                rect(screen,1246, 764, 1384, 947, makecol(0, 102, 120) );
                choix = 1;
                perso = chargerImage("data/images/avatars/Scooby_Doo.bmp");
                strcpy(IDJoueur->nomAvatar,"scooby");
            }
            else if(appuieBouton(1429, 764, 1570, 947)== 1)
            {
                rect(screen,1429, 764, 1570, 947, makecol(0, 102, 120) );
                choix = 1;
                perso = chargerImage("data/images/avatars/Shaggy_Rogers.bmp");
                strcpy(IDJoueur->nomAvatar,"sammy");
            }
            else if(appuieBouton(1629, 624, 1768, 811)== 1)
            {
                rect(screen, 1629, 624, 1768, 811, makecol(0, 102, 120) );
                choix = 1;
                perso = chargerImage("data/images/avatars/Velma_Dinkley.bmp");
                strcpy(IDJoueur->nomAvatar,"vera");
            }
            else if(appuieBouton(1629, 835, 1768, 1017)== 1)
            {
                rect(screen, 1629, 835, 1768, 1017, makecol(0, 102, 120) );
                choix = 1;
                perso = chargerImage("data/images/avatars/Fred_Jones.bmp");
                strcpy(IDJoueur->nomAvatar,"fred");
            }
        }
        for(i=0; i<numJoueur; i++)
        {
            if(strcmp(courant->nomAvatar, IDJoueur->nomAvatar) == 0)
            {
                erreur = 1;

                //affichage message d'erreur
                rectfill(screen, 0, 0, SCREEN_W, 20, makecol(180, 0, 0) );
                textprintf_ex(screen, font, 280, 10, makecol(255, 255, 255), -1, "Cet avatar est deja utilise par un autre joueur, veuillez en saisir un nouveau");

                //reinitialisation de la chaine de caractere pseudo
                for(j=0; j<15; j++)
                {
                    IDJoueur->nomAvatar[j] = '\0';
                }
                clear_bitmap(perso);
                i = numJoueur + 1;
            }
            else
            {
                erreur = 0;   //tout va bien
                courant = courant->suivant;
            }

        }

    }while(erreur == 1);
    return perso;
}
/*********************************************************************************************************/


/**< SETUP D'UNE STRUCTURE IDENTITE >*********************************************************************/
t_identite * allocInitIdentite()
{
    t_identite * nouveau;                                       //Création de la structure

    nouveau = (t_identite *)malloc(sizeof(t_identite));         //allocation dynamique

    if(nouveau == NULL)                                         //Blindage de l'allocation dynamique
    {
        allegro_message("Probleme d'allocation de la structure identite");
        allegro_exit();
    }
    nouveau->score = 0;                                       //Initialisation des variables à NULL ou 0
    nouveau->avatar = NULL;
    nouveau->suivant = NULL;
    nouveau->carte = NULL;

    nouveau->profilJoueur = allocInitJoueurs();         //Iniitalisation de la structure joueur pour un round

    return nouveau;                                           //Retour de la structure initialisée et allouée au SP précédent
}
/*********************************************************************************************************/


/**< SETUP D'UNE STRUCTURE DE JOUEUR >********************************************************************/
t_joueur * allocInitJoueurs ()
{
    //Déclarations
    t_joueur * nouveau;
    nouveau = (t_joueur *) malloc (sizeof(t_joueur));              //allocation d'une structure de joueur (valable pour 1 manche)

    if(nouveau == NULL)
    {
        allegro_message("Probleme d'allocation");                   //controle dans la console
        allegro_exit();                                             //fermeture d'allegro car erreur fatale bloquant le jeux
        exit(EXIT_FAILURE);
    }

//Initialisation des variables associées à NULL
    nouveau->role = 0;
    nouveau->gain = 0;
    nouveau->objetsJoueurs.chariot = 1;
    nouveau->objetsJoueurs.lampe = 1;
    nouveau->objetsJoueurs.pioche = 1;
    nouveau->objetsJoueurs.estCeReparation = 0;

    return nouveau;
}
/*********************************************************************************************************/
