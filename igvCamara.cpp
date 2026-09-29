#include <math.h>
#define _USE_MATH_DEFINES
#include <cmath>

#include "igvCamara.h"

igvCamara::igvCamara ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r
   , igvPunto3D _V ): P0 ( _P0 ), r ( _r ), V ( _V )
                      , tipo ( _tipo )
{  inicializarOrbita ();
}

void igvCamara::set ( igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V )
{  P0 = _P0;
   r  = _r;
   V  = _V;

   inicializarOrbita (); // sincroniza radio/acimut/elevacion con la nueva posicion
}

void igvCamara::set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r
                      , igvPunto3D _V, double _xwmin, double _xwmax, double _ywmin
                      , double _ywmax, double _znear, double _zfar )
{  tipo = _tipo;

   P0 = _P0;
   r = _r;
   V = _V;

   xwmin = _xwmin;
   xwmax = _xwmax;
   ywmin = _ywmin;
   ywmax = _ywmax;
   znear = _znear;
   zfar = _zfar;

   inicializarOrbita ();
}

void igvCamara::set ( tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r
                      , igvPunto3D _V, double _angulo, double _raspecto
                      , double _znear, double _zfar )
{  tipo = _tipo;

   P0 = _P0;
   r = _r;
   V = _V;

   angulo = _angulo;
   raspecto = _raspecto;
   znear = _znear;
   zfar = _zfar;

   inicializarOrbita ();
}

void igvCamara::aplicar ()
{  glMatrixMode ( GL_PROJECTION );
   glLoadIdentity ();

   if ( tipo == IGV_PARALELA )
   {
      glOrtho ( xwmin, xwmax, ywmin, ywmax, znear, zfar );
   }
   if ( tipo == IGV_FRUSTUM )
   {
      glFrustum ( xwmin, xwmax, ywmin, ywmax, znear, zfar );
   }
   if ( tipo == IGV_PERSPECTIVA )
   {
      gluPerspective ( angulo, raspecto, znear, zfar );
   }

   glMatrixMode ( GL_MODELVIEW );
   glLoadIdentity (); //Carga la identidad
   gluLookAt ( P0[X], P0[Y], P0[Z], r[X], r[Y], r[Z], V[X], V[Y], V[Z] ); //Matriz para la camara
}

void igvCamara::zoom ( double factor )
{  double escala;

   if (factor >= 0 ) {
      escala = 1.0 - factor; //Acercar
   }else {
      escala = 1.0 / ( 1.0 + factor ); //Alejar = Inversa
   }

   if ( tipo == IGV_PERSPECTIVA || tipo == IGV_PARALELA) {
      xwmin *= escala;
      xwmax *= escala;
      ywmin *= escala;
      ywmax *= escala;
   }

   if (tipo==IGV_PERSPECTIVA) {
      angulo *= escala;
   }
}

void igvCamara::incrementarZnear ( double incremento )
{  znear += incremento;

   if ( znear < 0.01 )
   {  znear = 0.01; // evita que znear llegue a 0 o negativo (romperia la proyeccion)
   }
}

void igvCamara::cambiarProyeccion () {
   double dx = P0[X] - r[X];
   double dy = P0[Y] - r[Y];
   double dz = P0[Z] - r[Z];
   double d = sqrt ( dx*dx + dy*dy + dz*dz );

   if ( tipo == IGV_PARALELA ) {
      double semiAltura = ( ywmax - ywmin ) / 2.0;
      double semiAnchura = ( xwmax - xwmin ) / 2.0;

      angulo = 2.0 * atan ( semiAltura / d ) * 180.0 / M_PI;
      raspecto = semiAnchura / semiAltura;

      tipo = IGV_PERSPECTIVA;

   } else if ( tipo == IGV_PERSPECTIVA ) {
      double semiAltura = d * tan ( ( angulo * M_PI / 180.0 ) / 2.0 );
      double semiAnchura = semiAltura * raspecto;

      ywmax =  semiAltura;
      ywmin = -semiAltura;
      xwmax =  semiAnchura;
      xwmin = -semiAnchura;

      tipo = IGV_PARALELA;
   }
}

void igvCamara::actualizarPosicionOrbital ()
{  double x = radioOrbita * cos ( elevacion ) * sin ( acimut );
   double y = radioOrbita * sin ( elevacion );
   double z = radioOrbita * cos ( elevacion ) * cos ( acimut );

   P0 = r + igvPunto3D ( x, y, z );
}

void igvCamara::inicializarOrbita ()
{  igvPunto3D d = P0 - r;
   radioOrbita = sqrt ( d[X]*d[X] + d[Y]*d[Y] + d[Z]*d[Z] );

   if ( radioOrbita < 1e-6 ) radioOrbita = 1e-6; // evita division por cero

   elevacion = asin ( d[Y] / radioOrbita );
   acimut = atan2 ( d[X], d[Z] );
}

void igvCamara::orbitar ( double deltaAcimut, double deltaElevacion )
{  acimut += deltaAcimut;
   elevacion += deltaElevacion;

   const double LIMITE = 1.55; // ~89 grados, evita voltear la camara
   if ( elevacion > LIMITE ) elevacion = LIMITE;
   if ( elevacion < -LIMITE ) elevacion = -LIMITE;

   actualizarPosicionOrbital ();
}

void igvCamara::rotarEjeY ( double anguloGrados )
{  igvPunto3D direccion = r - P0;
   double rad = anguloGrados * M_PI / 180.0;

   double nuevoX = direccion[X] * cos(rad) + direccion[Z] * sin(rad);
   double nuevoZ = -direccion[X] * sin(rad) + direccion[Z] * cos(rad);

   r = P0 + igvPunto3D ( nuevoX, direccion[Y], nuevoZ );

   inicializarOrbita ();
}

/**
 * Incrementa/decrementa la distancia del plano lejano (back)
 * @param incremento Cantidad a sumar (o restar, si es negativa) a zfar
 */
void igvCamara::incrementarZfar ( double incremento )
{  znear += incremento;

   if ( znear < 0.01 )
   {  znear = 0.01;
   }
   if ( znear > zfar - 0.1 )
   {  znear = zfar - 0.1;
   }
}