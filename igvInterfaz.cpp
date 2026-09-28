#include <cstdlib>
#include <stdio.h>
#include <cmath>
#include "igvInterfaz.h"

igvInterfaz* igvInterfaz::_instancia = nullptr;

igvInterfaz& igvInterfaz::getInstancia ()
{  if ( !_instancia )
   {  _instancia = new igvInterfaz;
   }
   return *_instancia;
}

void igvInterfaz::crear_mundo ()
{  p0 = igvPunto3D ( 3.0, 2.0, 4 );
   r = igvPunto3D ( 0, 0, 0 );
   V = igvPunto3D ( 0, 1.0, 0 );

   _instancia->camara.set ( IGV_PARALELA, p0, r, V, -1 * 3, 1 * 3, -1 * 3, 1 * 3, 1, 200 );
   // camara.set(...) ya sincroniza internamente radioOrbita/acimut/elevacion

   double d = sqrt ( p0[X]*p0[X] + p0[Y]*p0[Y] + p0[Z]*p0[Z] ); // misma distancia que la vista principal
   _instancia->camaraCenital.set ( IGV_PARALELA
                                  , igvPunto3D ( 0, d, 0 ), igvPunto3D ( 0, 0, 0 ), igvPunto3D ( 0, 0, -1 )
                                  , -1 * 3, 1 * 3, -1 * 3, 1 * 3, 1, 200 );

}

void igvInterfaz::configura_entorno ( int argc, char **argv, int _ancho_ventana
                                      , int _alto_ventana, int _pos_X, int _pos_Y
                                      , std::string _titulo )
{  ancho_ventana = _ancho_ventana;
   alto_ventana = _alto_ventana;

   glutInit ( &argc, argv );
   glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
   glutInitWindowSize ( _ancho_ventana, _alto_ventana );
   glutInitWindowPosition ( _pos_X, _pos_Y );
   glutCreateWindow ( _titulo.c_str () );

   glEnable ( GL_DEPTH_TEST );
   glClearColor ( 1.0, 1.0, 1.0, 0.0 );

   glEnable ( GL_LIGHTING );
   glEnable ( GL_NORMALIZE );

   crear_mundo ();
}

void igvInterfaz::inicia_bucle_visualizacion ()
{  glutMainLoop ();
}

void igvInterfaz::keyboardFunc ( unsigned char key, int x, int y )
{  switch ( key )
   {  case 'p':
      case 'P':
         _instancia->camara.cambiarProyeccion ();
         break;

      case 'v':
      case 'V':
      {  double d = sqrt ( _instancia->p0[X] * _instancia->p0[X]
                          + _instancia->p0[Y] * _instancia->p0[Y]
                          + _instancia->p0[Z] * _instancia->p0[Z] );

         switch ( _instancia->vista_actual )
         {  case OTRA:
               _instancia->vista_actual = PLANTA;
               _instancia->camara.set ( igvPunto3D ( 0, d, 0 ), igvPunto3D ( 0, 0, 0 ), igvPunto3D ( 0, 0, -1 ) );
               break;
            case PLANTA:
               _instancia->vista_actual = ALZADO;
               _instancia->camara.set ( igvPunto3D ( 0, 0, d ), igvPunto3D ( 0, 0, 0 ), igvPunto3D ( 0, 1, 0 ) );
               break;
            case ALZADO:
               _instancia->vista_actual = PERFIL;
               _instancia->camara.set ( igvPunto3D ( d, 0, 0 ), igvPunto3D ( 0, 0, 0 ), igvPunto3D ( 0, 1, 0 ) );
               break;
            case PERFIL:
               _instancia->vista_actual = OTRA;
               _instancia->camara.set ( _instancia->p0, _instancia->r, _instancia->V );
               break;
         }
         break;
      }

      case '+':
         _instancia->camara.zoom ( 0.05 );
         break;
      case '-':
         _instancia->camara.zoom ( -0.05 );
         break;
      case 'n':
         _instancia->camara.incrementarZnear ( 0.2 );
         break;
      case 'N':
         _instancia->camara.incrementarZnear ( -0.2 );
         break;

      // ---- NUEVO: recorte del volumen de visión (plano front/back) ----
      case 'f': // mueve el plano front (cercano) hacia delante, cierra el volumen
         _instancia->camara.incrementarZnear ( 0.2 );
         break;
      case 'F': // mueve el plano front (cercano) hacia atrás, abre el volumen
         _instancia->camara.incrementarZnear ( -0.2 );
         break;
      case 'b': // mueve el plano back (lejano) hacia delante, cierra el volumen
         _instancia->camara.incrementarZfar ( -0.2 );
         break;
      case 'B': // mueve el plano back (lejano) hacia atrás, abre el volumen
         _instancia->camara.incrementarZfar ( 0.2 );
         break;

      case '4':
         _instancia->multivista = ! _instancia->multivista;
         break;
      case 'e':
         _instancia->escena.set_ejes ( !_instancia->escena.get_ejes () );
         break;

      case '1': case '2': case '3':
         _instancia->escena.set_objetoSeleccionado ( key - '0' );
         break;

      case 'u':
         _instancia->escena.trasladar_objeto ( 0, 0.1, 0 );
         break;
      case 'U':
         _instancia->escena.trasladar_objeto ( 0, -0.1, 0 );
         break;

      case 'x':
         _instancia->escena.rotar_objeto ( 5, 1, 0, 0 );
         break;
      case 'X':
         _instancia->escena.rotar_objeto ( -5, 1, 0, 0 );
         break;

      case 'y':
         if ( _instancia->modoCamara )
            _instancia->camara.rotarEjeY ( 5 );
         else
            _instancia->escena.rotar_objeto ( 5, 0, 1, 0 );
         break;
      case 'Y':
         if ( _instancia->modoCamara )
            _instancia->camara.rotarEjeY ( -5 );
         else
            _instancia->escena.rotar_objeto ( -5, 0, 1, 0 );
         break;

      case 'z':
         _instancia->escena.rotar_objeto ( 5, 0, 0, 1 );
         break;
      case 'Z':
         _instancia->escena.rotar_objeto ( -5, 0, 0, 1 );
         break;

      case 's':
         _instancia->escena.escalar_objeto ( 1.05 );
         break;
      case 'S':
         _instancia->escena.escalar_objeto ( 1.0 / 1.05 );
         break;

      case 'c': case 'C':
         _instancia->modoCamara = ! _instancia->modoCamara;
         break;

      case 27:
         exit ( 1 );
         break;
   }
   glutPostRedisplay ();
}

void igvInterfaz::reshapeFunc ( int w, int h )
{  _instancia->set_ancho_ventana ( w );
   _instancia->set_alto_ventana ( h );
   _instancia->camara.aplicar ();
}

void igvInterfaz::displayFunc ()
{  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // ---- Vista principal a pantalla completa ----
   glViewport ( 0, 0, _instancia->get_ancho_ventana (), _instancia->get_alto_ventana () );
   _instancia->camara.aplicar ();
   _instancia->escena.visualizar ();

   // ---- Vista cenital superpuesta (solo si multivista está activo) ----
   if ( _instancia->multivista )
   {  int anchoMini = _instancia->get_ancho_ventana () / 4;
      int altoMini  = _instancia->get_alto_ventana () / 4;
      int margen = 10;

      int x0 = _instancia->get_ancho_ventana () - anchoMini - margen; // esquina superior derecha
      int y0 = _instancia->get_alto_ventana () - altoMini - margen;

      glEnable ( GL_SCISSOR_TEST );
      glScissor ( x0, y0, anchoMini, altoMini );
      glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // limpia SOLO ese recuadro

      glViewport ( x0, y0, anchoMini, altoMini );
      _instancia->camaraCenital.aplicar ();
      _instancia->escena.visualizar ();

      glDisable ( GL_SCISSOR_TEST );
   }

   glutSwapBuffers ();
}

void igvInterfaz::inicializa_callbacks ()
{  glutKeyboardFunc ( keyboardFunc );
   glutReshapeFunc ( reshapeFunc );
   glutDisplayFunc ( displayFunc );
   glutSpecialFunc ( specialFunc );
}

int igvInterfaz::get_ancho_ventana ()
{  return ancho_ventana;
}

int igvInterfaz::get_alto_ventana ()
{  return alto_ventana;
}

void igvInterfaz::set_ancho_ventana ( int _ancho_ventana )
{  ancho_ventana = _ancho_ventana;
}

void igvInterfaz::set_alto_ventana ( int _alto_ventana )
{  alto_ventana = _alto_ventana;
}

void igvInterfaz::specialFunc ( int key, int x, int y )
{  if ( _instancia->modoCamara )
   {  switch ( key )
      {  case GLUT_KEY_LEFT:
            _instancia->camara.orbitar ( -0.05, 0 );
            break;
         case GLUT_KEY_RIGHT:
            _instancia->camara.orbitar ( 0.05, 0 );
            break;
         case GLUT_KEY_UP:
            _instancia->camara.orbitar ( 0, 0.05 );
            break;
         case GLUT_KEY_DOWN:
            _instancia->camara.orbitar ( 0, -0.05 );
            break;
      }
   }
   else
   {  switch ( key )
      {  case GLUT_KEY_LEFT:
            _instancia->escena.trasladar_objeto ( -0.1, 0, 0 );
            break;
         case GLUT_KEY_RIGHT:
            _instancia->escena.trasladar_objeto ( 0.1, 0, 0 );
            break;
         case GLUT_KEY_UP:
            _instancia->escena.trasladar_objeto ( 0, 0, 0.1 );
            break;
         case GLUT_KEY_DOWN:
            _instancia->escena.trasladar_objeto ( 0, 0, -0.1 );
            break;
      }
   }
   glutPostRedisplay ();
}