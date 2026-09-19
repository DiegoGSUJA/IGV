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

/// Vistas can�nicas
enum Vista
{  PLANTA   ///< Vista de planta (c�mara en el eje Y)
   , ALZADO   ///< Vista de alzado (c�mara en el eje Z)
   , PERFIL   ///< Vista de perfil (c�mara en el eje X)
   , OTRA   ///< Otro tipo de vista
};

/**
 * Los objetos de esta clase encapsulan la interfaz y el estado de la aplicaci�n
 */
class igvInterfaz
{  private:
      // Atributos
      int ancho_ventana = 0; ///< Ancho de la ventana de visualizaci�n
      int alto_ventana = 0;  ///< Alto de la ventana de visualizaci�n

      igvEscena3D escena; ///< Escena que se visualiza en la ventana definida por igvInterfaz
      igvCamara camara; ///< C�mara que se utiliza para visualizar la escena

      // Valores de la vista panor�mica
      igvPunto3D p0 = { 0, 0, 0 } ///< Posici�n de la c�mara
                 , r = { 0, 0, 0 } ///< Punto de referencia para las vistas
                 , V = { 0, 0, 0 } ///< Vector que indica la vertical en la vista
                 ;


      Vista vista_actual = OTRA; //Vista activa actualmente

      bool multivista = false; //Indica si esta en modo 4 vistas

      bool modoCamara = false; //Teclas afectan a camara o a objeto, lo usamos dado a que nos quedamos sin teclas comunes

      // Aplicaci�n del patr�n Singleton
      static igvInterfaz* _instancia;   ///< Puntero al objeto �nico de la clase
      /// Constructor por defecto
      igvInterfaz() = default;

   public:
      static igvInterfaz& getInstancia ();

      /// Destructor
      ~igvInterfaz () = default;

      // M�todos est�ticos
      // callbacks de eventos
      static void keyboardFunc ( unsigned char key, int x, int y ); // m�todo para control de eventos del teclado
      static void reshapeFunc ( int w, int h ); // m�todo que define la camara de vision y el viewport
                                                // se llama autom�ticamente cuando se cambia el tama�o de la ventana
      static void displayFunc (); // m�todo para visualizar la escena


      // M�todos
      // crea el mundo que se visualiza en la ventana
      void crear_mundo ();

      // inicializa todos los par�metros para crear una ventana de visualizaci�n
      void configura_entorno ( int argc, char **argv // par�metros del main
                               , int _ancho_ventana, int _alto_ventana // ancho y alto de la ventana de visualizaci�n
                               , int _pos_X, int _pos_Y // posici�n inicial de la ventana de visualizaci�n
                               , std::string _titulo // t�tulo de la ventana de visualizaci�n
                             );

      void inicializa_callbacks (); // inicializa todos los callbacks

      void inicia_bucle_visualizacion (); // visualiza la escena y espera a eventos sobre la interfaz

      // m�todos get_ y set_ de acceso a los atributos
      int get_ancho_ventana ();

      int get_alto_ventana ();

      void set_ancho_ventana ( int _ancho_ventana );

      void set_alto_ventana ( int _alto_ventana );
};

#endif   // __IGVINTERFAZ
