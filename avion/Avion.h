#ifndef AVION_H_INCLUDED
#define AVION_H_INCLUDED

#include "Myallegro.h"
#include "Souris.h"

/* classe d'un avion */
class Avion
{
/* attributs prives */
private:
    Myallegro m_alleg;
    std::string ms_type = "";   /* type de l'avion */
    int mi_carburant = 0;          /* carburant en possession */
    int mi_consom = 0;          /* consommation (litre pour 100km) */
    int mi_capa = 0 ;           /* capacite en carburant */
    int mi_pose[2] = {0, 0};    /* position de l'avion */
    int mi_speed = 0;           /* vitesse de l'avion */
    int mi_altitude = 0;        /* altitude */
    int mi_id = 0;              /* id de l'avion */
    BITMAP *m_img = NULL;       /* l'image de l'avion */
    float mf_angle = 0.0;       /* angle de rotation de l'avion */
    bool mb_sur_piste = 0;      /* sur piste de decollage ou d'atterrissage ? */
    bool mb_fuite = false;      /* l'avion fuite-t-il ? */
    bool mb_en_vie = true;      /* l'avion existe-t-il toujours ? */
    std::vector<const Avion*> m_adj;    /* liste d'adjacence */
    std::vector< std::pair< int, int > > m_aretes;  /* liste des aroprots a parcourir */
    int mi_color = 0;                   /* coloration de l'avion */
    int mi_alt_to_do = 0;                   /* altitude a avoir */

/* interface */
public:
    /* constructeur et destructeur */
    Avion();
    Avion(std::ifstream &file, int i_id);
    ~Avion();

    /* methodes */

    /* sauvegarde les caractéristiques de l'avion */
    //void v_save( std::ofstream &file );
    /* charge les caractéristiques de l'avion */
    void v_load( std::ifstream &file );
    /* affiche les caractéristiques de l'avion et sa localisation */
    void v_display_all( BITMAP *&page, Souris &souris, int i_x, int i_y, bool b_droit, int i_current, int i_curseur );
    /* affiche les caractéristiques de l'avion */
    void v_display( BITMAP *&page, int i_x, int i_y );
    /* affiche sur console un avion */
    void v_console();
    /* l'avion va-t-il subir une fuite reservoir ? */
    bool b_rand_fuite();
    /* test l'affectation d'une couleur */
    bool b_testFrequence(int i_freq);
    /* vide les adjacences de l'avion */
    void v_vide_adj();
    /* ajoute une adjacence */
    void v_ajout_adj( Avion *avion );
    /* ajoute une arete */
    void v_ajout_arete_av( std::pair <int, int> arete );
    /* vide la liste d'aretes de l'avion */
    void v_vide_arete();

    /* getter */
    std::string s_get_type() const;     /* getter du type */
    int i_get_consom() const;           /* getter sur consommation */
    int i_get_capa() const;             /* getter sur la capacite en carburant */
    int i_get_carburant() const;        /* getter sur le carburant en possession  */
    int i_get_poseX() const;            /* getter sur la position en x */
    int i_get_poseY() const;            /* getter sur la position en y */
    int i_get_speed() const;            /* getter sur la vitesse */
    int i_get_id() const;               /* getter sur l'id */
    int i_get_altitude() const;               /* getter sur l'altitude */
    float f_get_angle() const;          /* getter sur l'angle de rotation */
    bool b_get_sur_piste() const;
    bool b_get_fuite() const;
    bool b_get_en_vie() const;
    int i_get_degre() const;
    int i_get_color() const;
    int i_get_alt_to_do() const;
    std::vector< std::pair< int, int > > get_aretes() const;
    BITMAP *get_img() const;

    /* setter */
    void v_set_type( std::string s_type );  /* setter sur le type */
    void v_set_consom( int i_consom );     /* setter sur la consommation */
    void v_set_capa( int i_capa );         /* setter sur la capacite en carburant */
    void v_set_carburant( int i_carb );    /* setter sur le carburant en possession */
    void v_set_poseX( int i_x );           /* setter sur la position en x */
    void v_set_poseY( int i_y );           /* setter sur la position en y */
    void v_set_speed( int i_speed );       /* setter sur la vitesse */
    void v_set_altitude( int i_value );       /* setter sur l'altitude */
    void v_set_angle( float f_angle );       /* setter sur la angle */
    void v_set_sur_piste( bool b_value );
    void v_set_fuite( bool b_value );
    void v_set_en_vie( bool b_value );
    void v_set_color( int i_value );
    void v_set_alt_to_do( int i_value );
};

#endif // AVION_H_INCLUDED
