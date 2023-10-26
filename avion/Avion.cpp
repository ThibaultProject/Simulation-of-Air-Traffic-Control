#include "Avion.h"

/* constructeur et destructeur */
Avion::Avion()
{
    /* rien a mettre */
}

Avion::Avion(std::ifstream &file, int i_id)
{
    /* charge l'avion */
    v_load( file );

    mi_id = i_id;
    m_img=m_alleg.load_img("images/avion.bmp");
}

Avion::~Avion()
{
    /* rien a mettre */
}

/* methodes */

/* sauvegarde les caractéristiques de l'avion */
//void Avion::v_save( std::ofstream &file )
//{
//    file << ms_type << std::endl;       /* type de l'avion */
//    file << mi_carburant << std::endl;       /* carburant */
//    file << mi_consom << std::endl;     /* consommation (litre pour 100km) */
//    file << mi_capa << std::endl;       /* capacite en carburant */
//    file << mi_pose[0] << std::endl;    /* position de l'avion en x */
//    file << mi_pose[1] << std::endl;    /* position de l'avion en y */
//}

/* charge les caractéristiques de l'avion */
void Avion::v_load( std::ifstream &file )
{
    file >> ms_type;                /* charge le type */
    file >> mi_carburant;                /* charge le carburant */
    file >> mi_consom;             /* charge la consommation */
    file >> mi_capa;             /* charge la capacite */
    file >> mi_pose[0];             /* charge la capacite */
    file >> mi_pose[1];             /* charge la capacite */
}

/* affiche les caractéristiques de l'avion et sa localisation */
void Avion::v_display_all( BITMAP *&page, Souris &souris, int i_x, int i_y, bool b_droit, int i_current, int i_curseur )
{
    /* Declaration des ressources */
    int i_page = i_current - i_curseur;

   // mf_angle+=10;
   // if(mf_angle>=360)mf_angle=0;
    /* rectangle rouge de localisation */
    rotate_sprite(page, m_img, mi_pose[0]-3, mi_pose[1]-3, ftofix(mf_angle*128.0/180.0));
    //m_alleg.myrectfill( page, mi_pose[0], mi_pose[1], mi_pose[0]+10, mi_pose[1]+10, 0, 255, 0 );
    souris.b_verifclick( 0, mi_pose[0], mi_pose[1], mi_pose[0]+10, mi_pose[1]+10 );     /* bruitage si on survole le rectangle */

    /* si on a demande d'afficher tous les aeroports */
    if( b_droit && ( 0 <= i_page ) && ( 2 >= i_page ) )
    {
        /* on affiche les caracteristiques de l'aeroport en bas de l'ecran */
        v_display( page, i_x+250*i_page, i_y );
    }

    /* si la souris est en collision avec le rectangle rouge de l'avion */
    if( m_alleg.collimouse(mi_pose[0], mi_pose[1], mi_pose[0]+10, mi_pose[1]+10) )
    {
        /* on affiche les caracteristiques de l'aeroport sous le rectangle rouge */
        v_display( page, mi_pose[0], mi_pose[1]+10 );
    }
}

/* affiche les caractéristiques de l'aeroport */
void Avion::v_display( BITMAP *&page, int i_x, int i_y )
{
    /* Declaration des ressources */
    std::string s_txt = "";
    std::string s_txt2 = "";

    /* si l'avion ne fuite pas */
    if( false == b_get_fuite() )
    {
        s_txt = "Reservoir operationnel !";   /* message de l'etat actuel du reservoir */
    }
    /* sinon l'avion fuite */
    else
    {
        s_txt = "Danger : fuite reservoir !";
    }

    /* si l'avion est en vie */
    if( true == b_get_en_vie() )
    {
        s_txt2 = "Avion en vie !";   /* message de l'etat actuel du reservoir */
    }
    /* sinon l'avion a crashe... */
    else
    {
        s_txt2 = "Crash de l'avion...";
    }

    m_alleg.txtlangue( page, "Type : " + ms_type + " (" + m_alleg.s_int_to_str(mi_id) + ")", i_x, i_y );
    m_alleg.txtlangue( page, "Position : " + m_alleg.s_int_to_str(mi_pose[0]) + ", " + m_alleg.s_int_to_str(mi_pose[1]) , i_x, i_y+15 );
    m_alleg.txtlangue( page, "Consommation : " + m_alleg.s_int_to_str(mi_consom) + "L/100km" , i_x, i_y+30 );
    m_alleg.txtlangue( page, "Carburant : " + m_alleg.s_int_to_str(mi_carburant) + "L", i_x, i_y+45 );
    m_alleg.txtlangue( page, "Capa max (fuel) : " + m_alleg.s_int_to_str(mi_capa) + "L", i_x, i_y+60 );
    m_alleg.txtlangue( page, "Vitesse : " + m_alleg.s_int_to_str(mi_speed) + "km/UT", i_x, i_y+75 );
    m_alleg.txtlangue( page, "Altitude : " + m_alleg.s_int_to_str(mi_altitude) + "km", i_x, i_y+90 );
    m_alleg.txtlangue( page, s_txt, i_x, i_y+105 );
    m_alleg.txtlangue( page, s_txt2, i_x, i_y+120 );
}

/* affiche sur console un avion */
void Avion::v_console()
{
    /* Declaration des ressources */
    std::string s_txt = "";
    std::string s_txt2 = "";

    /* si l'avion ne fuite pas */
    if( false == b_get_fuite() )
    {
        s_txt = "Reservoir operationnel !";   /* message de l'etat actuel du reservoir */
    }
    /* sinon l'avion fuite */
    else
    {
        s_txt = "Danger : fuite reservoir !";
    }

    /* si l'avion est en vie */
    if( true == b_get_en_vie() )
    {
        s_txt2 = "Avion en vie !";   /* message de l'etat actuel du reservoir */
    }
    /* sinon l'avion a crashe... */
    else
    {
        s_txt2 = "Crash de l'avion...";
    }

    std::cout << "Type : " + ms_type + " (" + m_alleg.s_int_to_str(mi_id) + ")" << std::endl;
    std::cout << "Position : " + m_alleg.s_int_to_str(mi_pose[0]) + ", " + m_alleg.s_int_to_str(mi_pose[1]) << std::endl;
    std::cout << "Consommation : " + m_alleg.s_int_to_str(mi_consom) + "L/100km" << std::endl;
    std::cout << "Carburant : " + m_alleg.s_int_to_str(mi_carburant) + "L" << std::endl;
    std::cout << "Capa max (fuel) : " + m_alleg.s_int_to_str(mi_capa) + "L" << std::endl;
    std::cout << "Vitesse : " + m_alleg.s_int_to_str(mi_speed) + "km/UT" << std::endl;
    std::cout << "Altitude : " + m_alleg.s_int_to_str(mi_altitude) + "km" << std::endl;
    std::cout << s_txt << std::endl;
    std::cout << s_txt2 << std::endl << std::endl;
}

/* l'avion va-t-il subir une fuite reservoir ? */
bool Avion::b_rand_fuite()
{
    /* Declaration des ressources */
    bool b_deb_f = false;
    int i_val = 1;

    /* si l'avion ne fuite pas */
    if( false == b_get_fuite() )
    {
        /* on lance l'alea */
        i_val = rand()%20;

        /* l'avion fuite si on obtient 0 */
        if( 0 == i_val )
        {
            /* l'avion fuite */
            v_set_fuite( true );

            /* debut de fuite : il faut faire une escale d'urgence */
            b_deb_f = true;
        }
    }

    return b_deb_f;
}

/// Test l'affectation d'une fréquence
/// retourne vrai si la fréquence n'est pas en conflit avec une station adjacente
/// faux sinon
bool Avion::b_testFrequence(int i_freq)
{
    for (auto s : m_adj)
        if ( s->mi_color && s->mi_color == i_freq )
            return false;

    return true;
}

/* vide les adjacences de l'avion */
void Avion::v_vide_adj()
{
    while( 0 == m_adj.empty() )
    {
        m_adj.pop_back();
    }
}

/* ajoute une adjacence */
void Avion::v_ajout_adj( Avion *avion )
{
    m_adj.push_back( avion );
}

/* ajoute une arete */
void Avion::v_ajout_arete_av( std::pair <int, int> arete )
{
    m_aretes.push_back( arete );
}

/* vide la liste d'aretes de l'avion */
void Avion::v_vide_arete()
{
    while( 0 == m_aretes.empty() )
    {
        m_aretes.pop_back();
    }
}

/* getter */

/* getter du type */
std::string Avion::s_get_type() const
{
    return ms_type;
}

/* getter sur consommation */
int Avion::i_get_consom() const
{
    return mi_consom;
}

/* getter sur la capacite en carburant */
int Avion::i_get_capa() const
{
    return mi_capa;
}

int Avion::i_get_carburant() const
{
    return mi_carburant;
}

/* getter sur la position en x */
int Avion::i_get_poseX() const
{
    return mi_pose[0];
}

/* getter sur la position en y */
int Avion::i_get_poseY() const
{
    return mi_pose[1];
}

/* getter sur la vitesse */
int Avion::i_get_speed() const
{
    return mi_speed;
}

int Avion::i_get_id() const
{
    return mi_id;
}

int Avion::i_get_altitude() const
{
    return mi_altitude;
}

float Avion::f_get_angle() const
{
    return mf_angle;
}

bool Avion::b_get_sur_piste() const
{
    return mb_sur_piste;
}

bool Avion::b_get_fuite() const
{
    return mb_fuite;
}

bool Avion::b_get_en_vie() const
{
    return mb_en_vie;
}

int Avion::i_get_degre() const
{
    return (int)m_adj.size();
}

int Avion::i_get_color() const
{
    return mi_color;
}

int Avion::i_get_alt_to_do() const
{
    return mi_alt_to_do;
}


std::vector< std::pair< int, int > > Avion::get_aretes() const
{
    return m_aretes;
}

BITMAP *Avion::get_img() const
{
    return m_img;
}

/* setter */

/* setter sur le type */
void Avion::v_set_type( std::string s_type )
{
    ms_type = s_type;
}

/* setter sur la consommation */
void Avion::v_set_consom( int i_consom )
{
    mi_consom = i_consom;
}

/* setter sur la capacite en carburant */
void Avion::v_set_capa( int i_capa )
{
    mi_capa = i_capa;
}

void Avion::v_set_carburant( int i_carb )
{
    mi_carburant = i_carb;

    /* ne peut pas etre negatif */
    if( 0 > mi_carburant )
    {
        mi_carburant = 0;
        /* l'avion n'est plus en vie */
        v_set_en_vie( false );
        /* 0 km d'altitude */
        v_set_altitude( 0 );
        /* vitesse nulle */
        v_set_speed( 0 );
    }
    /* ne peut pas depasser le max */
    else if( mi_carburant > mi_capa )
    {
        mi_carburant = mi_capa;
    }
}

/* setter sur la position en x */
void Avion::v_set_poseX( int i_x )
{
    mi_pose[0] = i_x;
}

/* setter sur la position en y */
void Avion::v_set_poseY( int i_y )
{
    mi_pose[1] = i_y;
}

/* setter sur la vitesse */
void Avion::v_set_speed( int i_speed )
{
    mi_speed = i_speed;
}

/* setter sur l'altitude */
void Avion::v_set_altitude( int i_value )
{
    mi_altitude = i_value;
}

/* setter sur la angle */
void Avion::v_set_angle( float f_angle )
{
    mf_angle = f_angle;
}

void Avion::v_set_sur_piste( bool b_value )
{
    mb_sur_piste = b_value;
}

void Avion::v_set_fuite( bool b_value )
{
    mb_fuite = b_value;
}

void Avion::v_set_en_vie( bool b_value )
{
    mb_en_vie = b_value;
}

void Avion::v_set_color( int i_value )
{
    mi_color = i_value;
}

void Avion::v_set_alt_to_do( int i_value )
{
    mi_alt_to_do = i_value;
}
