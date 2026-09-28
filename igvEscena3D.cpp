#include <cstdlib>
#include <stdio.h>

#include "igvEscena3D.h"


igvEscena3D::igvEscena3D ()
{  GLfloat identidad[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

   for ( int obj = 0; obj < 3; obj++ )
   {  for ( int i = 0; i < 16; i++ )
   {  R1[obj][i] = identidad[i];
      S1[obj][i] = identidad[i];
      T1[obj][i] = identidad[i];
   }
   }
}


/**
 * M�todo para pintar los ejes coordenados llamando a funciones de OpenGL
 */
void igvEscena3D::pintar_ejes()
{  GLfloat rojo[] = { 1, 0, 0, 1.0 };
   GLfloat verde[] = { 0, 1, 0, 1.0 };
   GLfloat azul[] = { 0, 0, 1, 1.0 };

   glMaterialfv ( GL_FRONT, GL_EMISSION, rojo );
   glBegin ( GL_LINES );
   glVertex3f ( 1000, 0, 0 );
   glVertex3f ( -1000, 0, 0 );
   glEnd ();

   glMaterialfv ( GL_FRONT, GL_EMISSION, verde );
   glBegin ( GL_LINES );
   glVertex3f ( 0, 1000, 0 );
   glVertex3f ( 0, -1000, 0 );
   glEnd ();

   glMaterialfv ( GL_FRONT, GL_EMISSION, azul );
   glBegin ( GL_LINES );
   glVertex3f ( 0, 0, 1000 );
   glVertex3f ( 0, 0, -1000 );
   glEnd ();
}

/**
 * M�todo para pintar un tubo utilizando cu�dricas GLU
 */
void igvEscena3D::pintar_tubo ()
{  GLUquadricObj *tubo;

   tubo = gluNewQuadric ();
   gluQuadricDrawStyle ( tubo, GLU_FILL );
   gluQuadricNormals ( tubo, GLU_SMOOTH ); // NUEVO: genera normales para iluminación correcta

   glPushMatrix ();
   glTranslatef ( 0, 0, -0.5 );
   gluCylinder ( tubo, 0.25, 0.25, 1, 20, 20 );
   glPopMatrix ();

   gluDeleteQuadric ( tubo );
}

// M�todos p�blicos

/**
 * M�todo con las llamadas OpenGL para visualizar la escena
 */
void igvEscena3D::visualizar ( void )
{  // crear luces
   GLfloat luz0[] = { 10, 8, 9, 1 };
   glLightfv ( GL_LIGHT0, GL_POSITION, luz0 );
   glEnable ( GL_LIGHT0 );

   glPushMatrix (); // guarda la matriz de modelado

   // se pintan los ejes
   if ( ejes )
   {  pintar_ejes ();
   }

   // ---- Objeto 1: corona de picos ----
   GLfloat color_corona[] = { 0.7, 0.15, 0.15, 1.0 }; // rojizo, ajustable
   glMaterialfv ( GL_FRONT, GL_DIFFUSE, color_corona );

   glPushMatrix ();
   glMultMatrixf ( T1[0] ); // traslación acumulada del objeto 1
   glMultMatrixf ( S1[0] ); // escalado acumulado
   glMultMatrixf ( R1[0] ); // rotación acumulada
   pintar_corona ();
   glPopMatrix ();

   // ---- Objeto 2: escalera ----
   GLfloat color_escalera[] = { 0.15, 0.4, 0.15, 1.0 }; // verde apagado
   glMaterialfv ( GL_FRONT, GL_DIFFUSE, color_escalera );

   glPushMatrix ();
   glMultMatrixf ( T1[1] ); // traslación acumulada del objeto 2
   glMultMatrixf ( S1[1] ); // escalado acumulado
   glMultMatrixf ( R1[1] ); // rotación acumulada
   pintar_escalera ();
   glPopMatrix ();

   // ---- Objeto 3: tubería ----
   GLfloat color_tuberia[] = { 0.1, 0.1, 0.6, 1.0 }; // azul
      glMaterialfv ( GL_FRONT, GL_DIFFUSE, color_tuberia );

   glPushMatrix ();
      glMultMatrixf ( T1[2] );
      glMultMatrixf ( S1[2] );
      glMultMatrixf ( R1[2] );
      pintar_tuberia ();
   glPopMatrix ();

   glPopMatrix (); // restaura la matriz de modelado
}

/**
 * M�todo para consultar si hay que dibujar los ejes o no
 * @retval true Si hay que dibujar los ejes
 * @retval false Si no hay que dibujar los ejes
 */
bool igvEscena3D::get_ejes ()
{  return ejes;
}

/**
 * M�todo para activar o desactivar el dibujado de los ejes
 * @param _ejes Indica si hay que dibujar los ejes (true) o no (false)
 * @post El estado del objeto cambia en lo que respecta al dibujado de ejes,
 *       de acuerdo al valor pasado como par�metro
 */
void igvEscena3D::set_ejes ( bool _ejes )
{  ejes = _ejes;
}




//METODOS RST

void  igvEscena3D::set_objetoSeleccionado ( int _obj ) {
   objetoSeleccionado = _obj - 1;
}

void igvEscena3D::rotar_objeto(double angulo, double ex, double ey, double ez) {
   glPushMatrix();
      glLoadIdentity();
      glMultMatrixf(R1[objetoSeleccionado]); //Recupera la rotacion
      glRotatef(angulo,ex,ey,ez); //Aplica el nuevo incremento
      glGetFloatv((GL_MODELVIEW_MATRIX), R1[objetoSeleccionado]); //Guarda el nuevo
   glPopMatrix();
}

void igvEscena3D::trasladar_objeto(double dx, double dy, double dz) {
   glPushMatrix ();
      glLoadIdentity ();
      glMultMatrixf ( T1[objetoSeleccionado] );
      glTranslatef ( dx, dy, dz );
      glGetFloatv ( GL_MODELVIEW_MATRIX, T1[objetoSeleccionado] );
   glPopMatrix ();
}

void igvEscena3D::escalar_objeto ( double factor )
{  glPushMatrix ();
   glLoadIdentity ();
   glMultMatrixf ( S1[objetoSeleccionado] );
   glScalef ( factor, factor, factor );
   glGetFloatv ( GL_MODELVIEW_MATRIX, S1[objetoSeleccionado] );
   glPopMatrix ();
}

void igvEscena3D::pintar_pico ()
{  const GLfloat L = 0.5, H = 1.0;

   glBegin ( GL_TRIANGLES );
   glNormal3f ( 0, 0.45, 0.89 ); // normal aprox. cara frontal (+Z)
   glVertex3f ( -L, 0, L );  glVertex3f ( L, 0, L );  glVertex3f ( 0, H, 0 );

   glNormal3f ( 0.89, 0.45, 0 ); // cara derecha (+X)
   glVertex3f ( L, 0, L );   glVertex3f ( L, 0, -L ); glVertex3f ( 0, H, 0 );

   glNormal3f ( 0, 0.45, -0.89 ); // cara trasera (-Z)
   glVertex3f ( L, 0, -L );  glVertex3f ( -L, 0, -L ); glVertex3f ( 0, H, 0 );

   glNormal3f ( -0.89, 0.45, 0 ); // cara izquierda (-X)
   glVertex3f ( -L, 0, -L ); glVertex3f ( -L, 0, L );  glVertex3f ( 0, H, 0 );
   glEnd ();

   glBegin ( GL_QUADS );
   glNormal3f ( 0, -1, 0 ); // base mirando hacia abajo
   glVertex3f ( -L, 0, L );  glVertex3f ( L, 0, L );
   glVertex3f ( L, 0, -L );  glVertex3f ( -L, 0, -L );
   glEnd ();
}

void igvEscena3D::pintar_corona() {
   const int NUM_PICOS = 6;
   const GLfloat RADIO = 1;

   for ( int i = 0; i < NUM_PICOS; i++ )
   {  glPushMatrix ();
      glRotatef ( i * ( 360.0 / NUM_PICOS ), 0, 1, 0 ); // reparte los picos alrededor del eje Y
      glTranslatef ( RADIO, 0, 0 );                     // los aleja del centro
      pintar_pico ();
      glPopMatrix ();
   }
}

void igvEscena3D::pintar_escalon() {
   glPushMatrix ();
      glScalef ( 1.0, 0.3, 0.6 ); // ancho, alto (fino), profundidad
      glutSolidCube ( 1 );
   glPopMatrix ();
}

void igvEscena3D::pintar_escalera ()
{  const GLfloat ALTURA_ESCALON = 0.3;
   const GLfloat PROFUNDIDAD_ESCALON = 0.6;

   // centra la escalera en el origen antes de dibujar
   glPushMatrix ();
      glTranslatef ( 0, -ALTURA_ESCALON * numEscalones / 2.0
                  , -PROFUNDIDAD_ESCALON * numEscalones / 2.0 );

      for ( int i = 0; i < numEscalones; i++ )
      {  pintar_escalon ();
         glTranslatef ( 0, ALTURA_ESCALON, PROFUNDIDAD_ESCALON );
      }
   glPopMatrix ();
}

void igvEscena3D::aumentar_escalones ()
{  numEscalones++;
}

void igvEscena3D::disminuir_escalones ()
{  if ( numEscalones > 1 ) numEscalones--;
}

void igvEscena3D::pintar_tuberia ()
{  glPushMatrix ();
      glRotatef ( 35, 1, 0, 0 );
      glScalef ( 1, 1, 3.0 );
      pintar_tubo ();
   glPopMatrix ();

   glPushMatrix ();
      glRotatef ( -35, 1, 0, 0 );
      glScalef ( 1, 1, 3.0 );
      pintar_tubo ();
   glPopMatrix ();
}



