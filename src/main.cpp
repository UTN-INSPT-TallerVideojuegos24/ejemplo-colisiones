#include "sfml-utn-inspt.h"

const unsigned int ANCHO_VENT = 800;
const unsigned int ALTO_VENT = 600;
const unsigned int FRAMERATE = 60;

// Parametros de los cuadrados
const float LADO = 100.f;

// Se rota o no el cuadrado "móvil"
bool hay_rotacion = false;

// lee los eventos (clicks, posición del mouse, redimensión de la ventana, etc)
void leer_eventos(RenderWindow &ventana);

int main() {
  // Se crea la ventana de ANCHO_VENT x ALTO_VENT con el nombre "UTN-INSPT SFML"
  RenderWindow ventana =
      RenderWindow({ANCHO_VENT, ALTO_VENT}, "UTN-INSPT SFML");
  // Se establece un limite de FRAMERATE x segundo
  ventana.setFramerateLimit(FRAMERATE);

  // Se define un cuadrado de LADO de lado fijo en la mitad de la
  // pantalla, rotado a 45°
  RectangleShape fijo = RectangleShape({LADO, LADO});
  fijo.rotate(45);
  fijo.setOrigin(LADO / 2.f, LADO / 2.f);
  fijo.setPosition(ANCHO_VENT / 2.f, ALTO_VENT / 2.f);

  // Se va a dibujar los "bounds" del fijo, y como no cambian se "pre-calculan"
  RectangleShape fb = RectangleShape(fijo.getGlobalBounds().getSize());
  fb.setPosition(fijo.getGlobalBounds().getPosition());
  fb.setFillColor(Color::Transparent);
  fb.setOutlineThickness(1.f);

  // Se define otro cuadrado de igual lado, "móvil" de color verde
  RectangleShape movil = RectangleShape({LADO, LADO});
  movil.setFillColor(Color::Green);
  movil.setOrigin(LADO / 2.f, LADO / 2.f);

  // Se dibujarán los "bounds" del móvil, pero estos cambian en cada iteración
  RectangleShape mb = RectangleShape({LADO, LADO});
  mb.setFillColor(Color::Transparent);
  mb.setOutlineColor(Color::Green);
  mb.setOutlineThickness(1.f);

  // posición actual del mouse
  Vector2 posMouse = {0, 0};

  /*
   * Loop principal del juego: mientras la ventana esté abierta...
   */
  while (ventana.isOpen()) {
    /**
     *  Lectura de los eventos y teclas por parte del usuario
     */
    leer_eventos(ventana);
    posMouse = Mouse::getPosition(ventana);
    /*
     *  Lógica del juego
     */
    // Se rota el cuadrado "móvil" si está presionado el clic izquierdo
    if (hay_rotacion) {
      movil.setRotation(45);
    } else {
      movil.setRotation(0);
    }
    movil.setPosition(posMouse.x, posMouse.y);
    // Detección naive de colisiones con "intersects()" (malo malo malo)
    if (movil.getGlobalBounds().intersects(fijo.getGlobalBounds())) {
      fijo.setFillColor(Color::Red);
      fb.setOutlineColor(Color::Red);
    } else {
      fijo.setFillColor(Color::White);
      fb.setOutlineColor(Color::White);
    }
    // para dibujar los "bounds" del móvil, se calculan en cada iteración:
    mb.setSize(movil.getGlobalBounds().getSize());
    mb.setPosition(movil.getGlobalBounds().getPosition());
    /*
     *  Actualizar el dibujo de la ventana
     */
    // Siempre se borra la pantalla anterior
    ventana.clear();
    // Se dibujan los elementos:
    ventana.draw(fb);    // Se dibujan los "bounds" del fijo
    ventana.draw(fijo);  // Se dibuja el cuadrado fijo
    ventana.draw(mb);    // Se dibujan los "bounds" del móvil
    ventana.draw(movil); // el móvil siempre por arriba de todo
    // Se muestra la pantalla con los elemnetos dibujados sobre ella:
    ventana.display();
  }
}

void leer_eventos(RenderWindow &ventana) {
  // Se "leen" los eventos y se guardan en event.
  Event event = Event();
  // Se recorren los eventos hasta que ya no hayan más
  while (ventana.pollEvent(event)) {
    if (event.type == Event::Closed) {
      // Si el usuario tocó la [X] para cerra la ventana:
      ventana.close();
    } else if (event.type == Event::MouseButtonPressed) {
      if (event.mouseButton.button == Mouse::Left) {
        hay_rotacion = true;
      }
    } else if (event.type == Event::MouseButtonReleased) {
      if (event.mouseButton.button == Mouse::Left) {
        hay_rotacion = false;
      }
    }
  }
}
