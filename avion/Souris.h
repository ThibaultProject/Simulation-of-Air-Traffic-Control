#ifndef SOURIS_H_INCLUDED
#define SOURIS_H_INCLUDED

#include "Myallegro.h"

class Souris{
private:
    //attributs
    Myallegro m_alleg;           //les fonctions allegro
    BITMAP *m_img_souris[2]; //images de la souris
    int mi_front[2];             //front montant ? (passage de non appui gauche à appui gauche)
    int m_old[4]={0,0,0,0}; //ancien rectangle en collision
    SAMPLE *m_sound[2];     /* effets sonores */

public:
    //constructeur et destructeur
    Souris();
    ~Souris() throw();

    //methodes
    void v_detruire();//detruit les elements
    void v_init_souris();//initialise la souris
    void v_gere_souris(int i_type);//gere les appuis boutons de la souris
    void v_display(BITMAP *&page);//affiche la souris
    bool b_verifclick(int sound, int x1, int y1, int x2, int y2);//permet la condition de click ou de relachement de click sur un rectangle

    //accesseurs
    //getter
    BITMAP *getImgMouse(int i_x);
    int i_getFront(int i_place) const;
};

#endif // SOURIS_H_INCLUDED
