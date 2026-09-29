#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

/**
 * Los objetos de esta clase representan escenas 3D para su visualizaci�n
 */
class igvEscena3D
{  private:
      // Atributos
      bool ejes = true;   ///< Indica si hay que dibujar los _ejes coordenados o no


      GLfloat R1[3][16], S1[3][16], T1[3][16]; //Rotacion, escalado y translacion

      int objetoSeleccionado = 0; //0,1,2

      int numEscalones = 5;

      void pintar_escalon();

      void pintar_escalera();


   public:
      // Constructores por defecto y destructor
      /// Constructor por defecto
      igvEscena3D ();
      /// Destructor
      ~igvEscena3D () = default;

      // M�todos
      // m�todo con las llamadas OpenGL para visualizar la escena
      void visualizar ();

      bool get_ejes ();

      void set_ejes ( bool _ejes );

      //Tocar el objeto seleccionado y sus matrices
      void set_objetoSeleccionado ( int _obj );
      void trasladar_objeto ( double dx, double dy, double dz );
      void rotar_objeto ( double angulo, double ex, double ey, double ez );
      void escalar_objeto ( double factor );

   private:
      void pintar_ejes ();


      //OBJETO 1
      void pintar_pico();
      void pintar_corona();

      //OBJETO 2
      void aumentar_escalones();
      void disminuir_escalones();

      //OBJETO 3
      void pintar_tubo ();
      void pintar_tuberia();

      //OBJETO 4
      void pintar_chuche();
};

#endif   // __IGVESCENA3D
