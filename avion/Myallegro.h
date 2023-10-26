#ifndef MYALLEGRO_H_INCLUDED
#define MYALLEGRO_H_INCLUDED

#include <algorithm>
#include "allegro.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

class Myallegro{
public:
    //constructeur et destructeur
    Myallegro();
    ~Myallegro();

    //methodes
    BITMAP *mycreate_bitmap(int width, int height); //creer un bitmap
    void myclear_to_color(BITMAP *&bitmap, int color);            //effacer un bitmap
    void myallegro_exit();                          //sortir d'allegro
    void myexit();                                  //sortie par erreur
    void myallegro_message(const char* message);    //afficher un message
    void myblit(BITMAP *source, BITMAP *&dest, int source_x, int source_y, int dest_x, int dest_y, int width, int height);//afficher une image
    void myrest(unsigned int tyme);//pause
    void myrectfill(BITMAP *&dest, int x1, int y1, int x2, int y2, int red, int green, int blue);//dessine l'interieur dun rectangle
    void myrect(BITMAP *&dest, int x1, int y1, int x2, int y2, int red, int green, int blue);//dessine les contours dun rectangle
    void mydestroy_sample(SAMPLE *&s);//detruit musique
    void mydestroy_bitmap(BITMAP *&s);//detruit image
    void mytextout_ex(BITMAP *&page, FONT *font, char *txtfr, int xfr, int yfr, int color);//ecrire du texte
    void myplay_sample(const SAMPLE* spl, int vol, int pan, int freq, int loop);//jouer une musique
    void mystop_sample(const SAMPLE* spl);//arreter une musique
    void mystretch_sprite(BITMAP *&arrivee, BITMAP *depart, int x, int y, int w, int h);//modifie la taille d'une image
    void load_one_sound(SAMPLE *&sa_musique, std::string chemin);//charge une musique
    BITMAP *load_img(std::string chemin);//charge une image
    void create_page(BITMAP *&page, int x, int y, int color);//cree la page d'affichage
    void transparence(BITMAP *&page, BITMAP *const&picture, int posex, int posey);//permet de modifier la luminosité d'une image
    void myStrToChar(char txt[100], std::string str);//converti string en char*
    void myCharToStr(std::string &str, char txt[100]);//converti char* en string
    std::string s_int_to_str(int i_myint);//permet de convertir un entier en str
    void txtlangue(BITMAP *&page, std::string txtfr, int xfr, int yfr, bool b_font=true);//permet d'afficher le texte correspondant au chois de la langue
    void drawrect(BITMAP *&page, bool demande, bool force, int x1, int y1, int x2, int y2);//contour ou rectangle plein en fonction de la souris
    int collimouse(int x1, int y1, int x2, int y2);//permet d'observer une collision de la souris avec un rectangle 0:non 1:oui
    /* trace une ligne */
    void myline( BITMAP *&page, int i_x1, int i_y1, int i_x2, int i_y2, int i_r, int i_v, int i_b );
    /* on verifie qu'une chaine de caractere ne contient que des chiffres */
    bool b_only_digit( std::string s_txt );
    /* saisie dans un intervalle defini */
    void v_saisie_blinde( int &i_val, int i_dep, int i_fin );

    //getter
    int getMouseX() const;
    int getMouseY() const;
    int getSCREEN_W() const;
    int getSCREEN_H() const;
};

#endif // MYALLEGRO_H_INCLUDED
