#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

#include <string>

#include "igvEscena3D.h"
#include "igvCamara.h"

enum Vista
{  PLANTA
   , ALZADO
   , PERFIL
   , OTRA
};

class igvInterfaz
{  private:
    int ancho_ventana = 0;
    int alto_ventana = 0;

    igvEscena3D escena;
    igvCamara camara; ///< Cámara principal (ya la tenías)
    igvCamara camaraCenital; ///< NUEVO: cámara fija con vista desde arriba, para el recuadro superpuesto

    igvPunto3D p0 = { 0, 0, 0 }
    , r = { 0, 0, 0 }
    , V = { 0, 0, 0 }
    ;

    Vista vista_actual = OTRA;
    bool multivista = false;
    bool modoCamara = false;

    static igvInterfaz* _instancia;
    igvInterfaz() = default;

public:
    static igvInterfaz& getInstancia ();
    ~igvInterfaz () = default;

    static void keyboardFunc ( unsigned char key, int x, int y );
    static void reshapeFunc ( int w, int h );
    static void displayFunc ();
    static void specialFunc ( int key, int x, int y );

    void crear_mundo ();

    void configura_entorno ( int argc, char **argv
                             , int _ancho_ventana, int _alto_ventana
                             , int _pos_X, int _pos_Y
                             , std::string _titulo
                           );

    void inicializa_callbacks ();
    void inicia_bucle_visualizacion ();

    int get_ancho_ventana ();
    int get_alto_ventana ();
    void set_ancho_ventana ( int _ancho_ventana );
    void set_alto_ventana ( int _alto_ventana );
};

#endif   // __IGVINTERFAZ