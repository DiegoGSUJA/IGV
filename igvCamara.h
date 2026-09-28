#ifndef __IGVCAMARA
#define __IGVCAMARA

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

#include "igvPunto3D.h"

/**
 * Etiquetas para los diferentes tipos de camara
 */
enum tipoCamara
{  IGV_PARALELA   ///< Proyeccion paralela
   , IGV_FRUSTUM   ///< Proyeccion en perspectiva usando OpenGL
   , IGV_PERSPECTIVA   ///< Proyeccion en perspectiva usando GLU
};

/**
 * Los objetos de esta clase representan camaras de visualizacion en la aplicacion
 */
class igvCamara
{  private:
      // atributos

      // parametros de la orbita (coordenadas esfericas respecto al punto r)
      double radioOrbita = 0;
      double acimut = 0;      // angulo horizontal en radianes
      double elevacion = 0;   // angulo vertical en radianes

      tipoCamara tipo = IGV_PARALELA;  ///< Tipo de la camara

      // ventana de vision: parametros proyeccion paralela y frustum
      GLdouble xwmin = -3    ///< Coordenada X minima del frustum/proyeccion paralela
             , xwmax = 3   ///< Coordenada X maxima del frustum/proyeccion paralela
             , ywmin = -3   ///< Coordenada Y minima del frustum/proyeccion paralela
             , ywmax = 3   ///< Coordenada Y maxima del frustum/proyeccion paralela
             ;

      // ventana de vision: parametros proyeccion perspectiva
      GLdouble angulo = 60   ///< Angulo de apertura (proyeccion perspectiva)
             , raspecto = 1   ///< Razon de aspecto (proyeccion perspectiva)
             ;

      // distancias de planos cercano y lejano
      GLdouble znear = 1    ///< Distancia de la camara al plano Z near
             , zfar = 200 ///< Distancia de la camara al plano Z far
             ;

      // punto de vision
      igvPunto3D P0 = { 3, 2, 4 };   ///< Posicion de la camara

      // punto de referencia de vision
      igvPunto3D r = { 0, 0, 0 };   ///< Punto al que mira la camara

      // vector arriba
      igvPunto3D V = { 0, 1, 0 };   ///< Vector que indica la vertical

      // Metodos privados de apoyo al modo orbita
      void actualizarPosicionOrbital ();
      void inicializarOrbita ();

   public:
      // Constructores por defecto y destructor
      /// Constructor por defecto
      igvCamara () = default;

      /// Destructor
      ~igvCamara () = default;

      // Otros constructores
      igvCamara ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V );

      // Metodos
      // define la posicion de la camara
      void set ( igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V );

      // define una camara de tipo paralela o frustum
      void set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V
                 , double _xwmin, double _xwmax, double _ywmin
                 , double _ywmax, double _znear, double _zfar );

      // define una camara de tipo perspectiva
      void set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V
                 , double _angulo, double _raspecto, double _znear, double _zfar );

      void aplicar ( void ); // aplica a los objetos de la escena la transformacion
                             // de vision y la transformacion de proyeccion
                             // asociadas a los parametros de la camara
      void zoom ( double factor ); // realiza un zoom sobre la camara

      void cambiarProyeccion (void); //Alternar entre paralela/perspectiva.

      void incrementarZnear(double incremento); //Mueve el plano cercano
      void incrementarZfar(double incremento);  // NUEVO: plano back


      // NUEVO: modo camara interactivo
      void orbitar (double deltaAcimut, double deltaElevacion);

      void rotarEjeY (double anguloGrados);
};

#endif   // __IGVCAMARA