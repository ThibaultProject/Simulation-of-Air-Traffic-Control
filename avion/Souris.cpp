#include "Souris.h"

//constructeur et destructeur
//constructeur par efaut
Souris::Souris(){}

//destructeur
Souris::~Souris() throw(){}

//methodes
//detruit les elements
void Souris::v_detruire()
{
    //desalloue les images
    for(uint8_t ui8_j = 0; (ui8_j < 2); ui8_j++)
    {
        m_alleg.mydestroy_bitmap( m_img_souris[ui8_j] );
    }

    //detruit les sons
    for(int i=0; i<2; i++)
    {
        m_alleg.mydestroy_sample(m_sound[i]);
    }
}

//initialise la souris
void Souris::v_init_souris()
{
    //Déclaration des ressources
    char chemin2[2][50]={"images/souris/standard1.bmp", "images/souris/standard2.bmp"};
    char chemin[2][50]={"music/bruits/clavier.WAV","music/bruits/collision.WAV"};

    //on charge les différentes images de la souris
    for( uint8_t ui8_j=0; (ui8_j < 2); ui8_j++)
    {
        m_img_souris[ui8_j]=m_alleg.load_img(chemin2[ui8_j]);
    }

    //charge tous les effets sonores
    for(int i=0; i<2; i++)
    {
        m_alleg.load_one_sound(m_sound[i],chemin[i]);//charge le son
    }
}

//gere les appuis boutons de la souris
void Souris::v_gere_souris(int i_type)
{
    //si on fait click gauche ou droite (en fonction de la demande)
    if(mouse_b & i_type)
    {
        //si on n'avait pas fait click gauche, on détecte un front montant
        if(0 == mi_front[i_type-1])
        {
            mi_front[i_type-1]=1;
        }
        //sinon on avait déjà un front montant
        else
        {
            mi_front[i_type-1]=2;
        }
    }
    //sinon on ne fait pas click
    else
    {
        //si on avait fait click gauche, on détecte un front desendant
        if(2 == mi_front[i_type-1])
        {
            mi_front[i_type-1]=3;
        }
        //sinon on avait déjà un front desendant
        else
        {
            mi_front[i_type-1]=0;
        }
    }
}

//affiche la souris
void Souris::v_display(BITMAP *&page)
{
    //l'affichage de la souris dépend du choix de l'image de la souris et du click gauche de la souris
    m_alleg.transparence(page, getImgMouse(mouse_b & 1), m_alleg.getMouseX(), m_alleg.getMouseY());
}

//permet la condition de click ou de relachement de click sur un rectangle
bool Souris::b_verifclick(int sound, int x1, int y1, int x2, int y2)
{
    //Déclaration des ressources
    bool b_valid;

    //si collision de la souris avec le rectangle lors d'un front montant ou descendant, c'est validé !
    if((1 == m_alleg.collimouse(x1, y1, x2, y2)) && (i_getFront(0) == 1))
    {
        if(sound)//si on est autorise de lancer le son
        {
            m_alleg.myplay_sample(m_sound[0], 255, 128, 1000, 0);//son qui confirme le click gauche sur un rectangle
        }
        b_valid=1;//click valide
    }
    //non valide
    else
    {
        //si on n'a pas de collision avec l'ancien rectangle de collision
        if((1 == m_alleg.collimouse(x1, y1, x2, y2)) && (0 == m_alleg.collimouse(m_old[0], m_old[1], m_old[2], m_old[3])))
        {
            m_alleg.myplay_sample(m_sound[1], 255, 128, 1000, 0);//son qui confirme la collision avec un rectangle

            //sauvegarde du nouveau rectangle de collision
            m_old[0]=x1;
            m_old[1]=y1;
            m_old[2]=x2;
            m_old[3]=y2;
        }
        else if((0 == m_alleg.collimouse(m_old[0], m_old[1], m_old[2], m_old[3])))
        {
            //reinitialise l'ancien rectangle
            for(int i=0; i<4; i++)
                m_old[i]=0;
        }

        b_valid=0;//click non valide
    }

    return b_valid;//retourne la valeur de "click gauche valide ? 1 oui 0 non"
}


//accesseurs
//getter
BITMAP *Souris::getImgMouse(int i_x)
{
    ///bien compris dans l'intervalle ?

    try{
        if(i_x<0 || i_x>2)//case hors du tableau ?
            throw std::string("la case est hors du tableau !");
        return m_img_souris[i_x];//valide
    }
    catch(std::string e){
        std::cerr << e << std::endl;//message erreur
        return 0;
    }
}

int Souris::i_getFront(int i_place) const
{
    return mi_front[i_place];
}
