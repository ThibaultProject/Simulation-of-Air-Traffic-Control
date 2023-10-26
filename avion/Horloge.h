#ifndef HORLOGE_H_INCLUDED
#define HORLOGE_H_INCLUDED

#include "Myallegro.h"

/* classe de l'horloge */
class Horloge{
private:
    //attributs
    Myallegro m_alleg;           //les fonctions allegro
    int mi_heure = 0;           /* heure */
    int mi_min = 0;           /* minute */
    int mi_sec = 0;           /* seconde */
    int mi_cpt = 0;             /* increment horloge */

public:
    /* constructeur et destructeur */
    Horloge();
    ~Horloge();

    /* methode */
    /* initialise l'heure */
    void v_init( int i_heure, int i_min, int i_sec, int i_cpt );
    /* affiche l'heure */
    void v_display( BITMAP *& page, int i_x, int i_y );
    /* saisit d'une heure */
    void v_saisi();

    /* accesseurs */
    /* getter */
    int i_get_heure() const;
    int i_get_min() const;
    int i_get_sec() const;
    int i_get_cpt() const;

    /* setter */
    void v_set_heure( int i_heure );
    void v_set_min( int i_min );
    void v_set_sec( int i_sec );
    void v_set_cpt( int i_cpt );
};

#endif // HORLOGE_H_INCLUDED
