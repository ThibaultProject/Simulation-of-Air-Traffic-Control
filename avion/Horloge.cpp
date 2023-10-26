#include "Horloge.h"

/* constructeur et destructeur */
Horloge::Horloge()
{
    /* rien a mettre */
}

Horloge::~Horloge()
{
    /* rien a mettre */
}

/* methode */

/* initialise l'heure */
void Horloge::v_init( int i_heure, int i_min, int i_sec, int i_cpt )
{
    v_set_heure( i_heure );
    v_set_min( i_min );
    v_set_sec( i_sec );
    v_set_cpt( i_cpt );
}

/* affiche l'heure */
void Horloge::v_display( BITMAP *& page, int i_x, int i_y )
{
    /* on incremente l'horloge */
    mi_sec += mi_cpt;

    /* on passe a la prochaine seconde ? */
    if( 60 <= mi_sec )
    {
        mi_sec-=60;
        mi_min+=1;  /* on passe a la prochaine minute */

        /* on passe a la prochaine heure ? */
        if( 60 == mi_min )
        {
            mi_min-=60;
            mi_heure+=1;  /* on passe a la prochaine heure */

            if( 24 == mi_heure )    /* fin de la journee ? */
            {
                mi_heure = 0;
            }
        }
    }

    /* affiche l'heure */
    m_alleg.txtlangue( page, m_alleg.s_int_to_str(i_get_heure())+" : "+m_alleg.s_int_to_str(i_get_min()), i_x, i_y );
}

/* saisit d'une heure */
void Horloge::v_saisi()
{
    std::cout << "Veuillez saisir l'heure de depart : " ;

    /* saisie de l'heure */
    m_alleg.v_saisie_blinde( mi_heure, 0, 23 );

    std::cout << "Veuillez saisir les minutes de depart : " ;

    /* saisie des minutes */
    m_alleg.v_saisie_blinde( mi_min, 0, 59 );
}

/* accesseurs */
/* getter */
int Horloge::i_get_heure() const
{
    return mi_heure;
}

int Horloge::i_get_min() const
{
    return mi_min;
}

int Horloge::i_get_sec() const
{
    return mi_sec;
}

int Horloge::i_get_cpt() const
{
    return mi_cpt;
}

/* setter */
void Horloge::v_set_heure( int i_heure )
{
    /* compris entre 0 et 23 */
    mi_heure = i_heure%24;
}

void Horloge::v_set_min( int i_min )
{
    /* compris entre 0 et 59 */
    mi_min = i_min%60;
}

void Horloge::v_set_sec( int i_sec )
{
    /* compris entre 0 et 59 */
    mi_sec = i_sec%60;
}

void Horloge::v_set_cpt( int i_cpt )
{
    mi_cpt = i_cpt;
}
