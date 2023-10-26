#include "Myallegro.h"

//constructeur et destructeur
//constructeur par defaut
Myallegro::Myallegro(){}

//destructeur
Myallegro::~Myallegro(){}

//cree un bitmap
BITMAP *Myallegro::mycreate_bitmap(int width, int height)
{
    return create_bitmap(width, height);
}

//effacer un bitmap
void Myallegro::myclear_to_color(BITMAP *&bitmap, int color)
{
    clear_to_color(bitmap, color);
}

//sortir d'allegro
void Myallegro::myallegro_exit()
{
    allegro_exit();
}

//sortie avec erreur
void Myallegro::myexit()
{
    exit(EXIT_FAILURE);
}

//message allegro
void Myallegro::myallegro_message(const char* message)
{
    allegro_message(message);
}

//afficher une image
void Myallegro::myblit(BITMAP *source, BITMAP *&dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height)
{
    blit(source, dest, source_x, source_y, dest_x, dest_y, width, height);
}

//pause pendant un temps donne
void Myallegro::myrest(unsigned int tyme)
{
    rest(tyme);
}

//dessine l'interieur dun rectangle
void Myallegro::myrectfill(BITMAP *&dest, int x1, int y1, int x2, int y2, int red, int green, int blue)
{
    rectfill(dest, x1, y1, x2, y2, makecol(red, green, blue));
}

//dessine les contours d'un rectangle
void Myallegro::myrect(BITMAP *&dest, int x1, int y1, int x2, int y2, int red, int green, int blue)
{
    rect(dest, x1, y1, x2, y2, makecol(red, green, blue));
}

//detruit musique
void Myallegro::mydestroy_sample(SAMPLE *&s)
{
    try{
        myplay_sample(s, 0, 0, 0, 0);
        mystop_sample(s);

        if(!s)//si la musique n'existe pas
            throw std::string("Destruction d'une musique inexistante");
        destroy_sample(s);//destruction de la musique
    }
    catch(std::string e){
        std::cerr << e << std::endl;//message erreur
    }

}

//detruit image
void Myallegro::mydestroy_bitmap(BITMAP *&s)
{
    try{
        if(!s->w)//si l'image n'existe pas
            throw std::string("Destruction d'une musique inexistante");
        destroy_bitmap(s);//destruction de l'image
    }
    catch(std::string e){
        std::cerr << e << std::endl;//message erreur
    }
}

//ecrire du texte
void Myallegro::mytextout_ex(BITMAP *&page, FONT *font, char *txtfr, int xfr, int yfr, int color)
{
    textout_ex(page, font, txtfr, xfr, yfr, color, -1);
}

//jouer une musique
void Myallegro::myplay_sample(const SAMPLE* spl, int vol, int pan, int freq, int loop)
{
    play_sample(spl, vol, pan, freq, loop);
}

//arreter une musique
void Myallegro::mystop_sample(const SAMPLE* spl)
{
    stop_sample(spl);
}

//modifie la taille d'une image
void Myallegro::mystretch_sprite(BITMAP *&arrivee, BITMAP *depart, int x, int y, int w, int h)
{
    stretch_sprite(arrivee, depart, x, y, w, h);
}

//charge une musique
void Myallegro::load_one_sound(SAMPLE *&musique, std::string chemin)
{
    //Déclaration des ressources
    char txt[100];

    myStrToChar( txt, chemin ); /* converti le str en char* */

    //charge la musique
    musique=load_wav(txt);

    ///existence de la musique ?
    if (!musique)
    {
        //blindage
        myallegro_message("musique non trouve");//message erreur
        myallegro_message(txt);//suite du message erreur : affichage du chemin ou se produit l'erreur
        myallegro_exit();//on quitte allegro
        //on sort du programme
        myexit();
    }
}

//charge une image
BITMAP *Myallegro::load_img(std::string chemin)
{
    //Déclaration des ressources
    BITMAP *img;
    char txt[100];

    myStrToChar( txt, chemin ); /* converti le str en char* */

    //charge l'image
    img=load_bitmap(txt, NULL);

    ///existence de l'image ?
    if(NULL == img)
    {
        myallegro_message("Erreur de chargement d'une image !");//message erreur
        myallegro_message(txt);
        myallegro_exit();                                       //on quitte allegro
        myexit();                                               //on sort du programme
    }
    return img; //on retourne l'image
}

//cree la page d'affichage
void Myallegro::create_page(BITMAP *&page, int x, int y, int color)
{
    //Declaration des ressources
    Myallegro alleg;

    //creation du buffer
    page=alleg.mycreate_bitmap(x, y);

    ///existence du buffer ?
    if(NULL == page)
    {
        alleg.myallegro_message("Erreur de creation de double buffer !"); //message erreur
        alleg.myallegro_exit(); //on sort d'allegro
        alleg.myexit(); //on sort du programme
    }

    //on met tout à 0
    alleg.myclear_to_color(page, color);
}

//affiche une image avec une transparence choisie
void Myallegro::transparence(BITMAP *&page, BITMAP *const&picture, int posex, int posey)
{
    //Déclaration des ressources
    int x, y, color;

    //on parcours l'image pour l'afficher
    for(y=0; y<picture->h; y++)
    {
        for(x=0; x<picture->w; x++)
        {
            //on récolte les couleurs du pixel
            color=getpixel(picture, x, y);

            //si ce n'est pas la couleur de transparence(255, 0, 255) et qu'on dans l'ecran, on affiche
            if(((255 != getr(color)) || (0 != getg(color)) || (255 != getb(color))) &&((0 <= posex+x) && (SCREEN_W >= posex+x) && (0 <= posey+y) && (SCREEN_H >= posey+y)))
            {
                //on affiche la couleur correspondante
                putpixel(page, posex+x, posey+y, color);
            }
        }
    }
}

//converti string en char*
void Myallegro::myStrToChar(char txt[100], std::string str)
{
    //conversion : copie du str dans char*
    for(unsigned int i=0; i<str.size()+1; i++)
        txt[i]=str[i];
}

//converti char* en string
void Myallegro::myCharToStr(std::string &str, char txt[100])
{
    str+=txt;
}

//permet de convertir un entier en chaine de caractere
std::string Myallegro::s_int_to_str(int i_myint)
{
    //Déclaration des ressources
    int i_carac=1, i_max=9, i_i;
    char s_mystr[21];
    std::string s_final;

    //on cherche le nombre de caracteres necessaires pour l'entier
    while(i_max < i_myint)
    {
        //on multiplie par 10
        i_max*=10;
        //on ajoute 9
        i_max+=9;
        //1 caractere en plus nécessaire
        i_carac++;
    }

    //on ne peut afficher que 20 caracteres
    if(20 < i_carac)
    {
        //on écrit le score max
        for(i_i=0; i_i<20; i_i++)
            s_mystr[i_i]='9';
        //fin de mot
        s_mystr[20]='\0';
    }
    else
    {
        //fin de mot
        s_mystr[i_carac]='\0';

        //conversion
        for(i_i=0; i_i<i_carac; i_i++)
        {
            //on transforme en caracteres le dernier chiffre du nombre !
            s_mystr[i_carac-1-i_i]=48+i_myint%10;
            //on divise par 10 le nombre
            i_myint/=10;
        }
    }

    /* on transforme le char * en str */
    myCharToStr( s_final, s_mystr );

    return s_final;
}

//permet d'afficher le texte correspondant au chois de la langue
void Myallegro::txtlangue(BITMAP *&page, std::string txtfr, int xfr, int yfr, bool b_font)
{
    //Déclaration des ressources
    int color=makecol(150, 150, 150);
    char mychar[100];

    myStrToChar( mychar, txtfr );   /* converti le str en char * */

    if( b_font )/* si on demande d'afficher le font */
    {
        drawrect(page, 0, 0, xfr, yfr, xfr+9*strlen(mychar)+5, yfr+15);   /* on trace un rectangle de font */
    }
    mytextout_ex(page, font, mychar, xfr+5, yfr+5, color);//texte
}


//contour ou rectangle plein en fonction de la souris et de la demande
void Myallegro::drawrect(BITMAP *&page, bool demande, bool force, int x1, int y1, int x2, int y2)
{
    //Déclaration des ressources
    int color=230;

    //si la souris est en collision avec le rectangle ou si on force l'affichage, on affiche le rectangle plein
    if(((1 == collimouse(x1, y1, x2, y2)) && (true == demande)) || (true == force))
    {
        color=190;
    }

    myrectfill(page, x1, y1, x2, y2, color, color, color);//contour du ectangle
    myrect(page, x1, y1, x2, y2, 190, 190, 190);//interieur du rectangle
}

//permet d'observer une collision de la souris avec un rectangle 0:non 1:oui
int Myallegro::collimouse(int x1, int y1, int x2, int y2)
{
    /* Declaration des ressources */
    int i_colli = 0;  /* par defaut absence de collision */

    //collision ?
    if(( getMouseX() >= x1 ) && ( getMouseX() < x2 ) && ( getMouseY() >= y1 ) && ( getMouseY() < y2 ))
    {
        i_colli = 1;//presence de collision
    }

    return i_colli;//absence de collision
}

/* trace une ligne */
void Myallegro::myline( BITMAP *&page, int i_x1, int i_y1, int i_x2, int i_y2, int i_r, int i_v, int i_b )
{
    line(page, i_x1, i_y1, i_x2, i_y2, makecol(i_r, i_v, i_b));
}

/* on verifie qu'une chaine de caractere ne contient que des chiffres */
bool Myallegro::b_only_digit( std::string s_txt )
{
    /* Declaration des ressources */
    bool b_cdt = true;
    char c;

    /* on parcours tous les caracteres */
    for( uint8_t ui8_i = 0; ui8_i < s_txt.size(); ui8_i++ )
    {
        /* caractere a etudier */
        c = s_txt[ui8_i];

        /* caractere different d'un chiffre ? */
        if(( c != '0' ) && ( c != '1' ) && ( c != '2' ) && ( c != '3' ) && ( c != '4' ) && ( c != '5' ) && ( c != '6' ) && ( c != '7' ) && ( c != '8' ) && ( c != '9' ))
        {
            /* la chaine de caractere ne contient pas que des chiffres */
            b_cdt = false;
        }
    }

    /* on retourne le resultat */
    return b_cdt;
}

/* saisie dans un intervalle defini */
void Myallegro::v_saisie_blinde( int &i_val, int i_dep, int i_fin )
{
    /* declaration des ressources */
    std::string s_txt = "";
    bool b_cdt = false;

    /* saisie du choix */
    do{
        std::cin >> s_txt;

        /* contient que des chiffres ? */
        if( true == b_only_digit( s_txt ) )
        {
            /* conversion du texte en chiffres */
            i_val = stoi( s_txt );

            /* mauvais interval ? */
            if (( i_dep > i_val ) || ( i_fin < i_val ))
            {
                /* message erreur */
                std::cout << "Mauvais intervalle... Recommencez : ";
            }
            else
            {
                /* bon intervalle ! */
                b_cdt = true;
            }
        }
    }while( false == b_cdt );
}


//getter
int Myallegro::getMouseX() const
{
    return mouse_x;
}

int Myallegro::getMouseY() const
{
    return mouse_y;
}

int Myallegro::getSCREEN_W() const
{
    return SCREEN_W;
}

int Myallegro::getSCREEN_H() const
{
    return SCREEN_H;
}
