#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <vector>
using namespace std;
//Importacion de headers propios
#include "Jugador.h"
#include "Ajustes.h"
#include "Bala.h"
//********Creacion variables del juego
#define FPS 120.0

//*******Funciones de juego
void RecibirTeclasPresionadas(ALLEGRO_EVENT evento, Jugador& jugador, std::vector<PtrBala>& Balas) {
	switch (evento.keyboard.keycode) {
		case ALLEGRO_KEY_RIGHT:
			jugador.rotacion_derecha = true;
			break;
		case ALLEGRO_KEY_LEFT:
			jugador.rotacion_izquierda = true;
			break;
		case ALLEGRO_KEY_UP:
			jugador.acelerando = true;
			break;
		case ALLEGRO_KEY_SPACE:
			CrearBala(jugador, Balas);

			break;
	}
}
void RecibirTeclasLevantadas(ALLEGRO_EVENT evento, Jugador & jugador){
	switch (evento.keyboard.keycode) {
		case ALLEGRO_KEY_RIGHT:
			jugador.rotacion_derecha = false;
			break;
		case ALLEGRO_KEY_LEFT:
			jugador.rotacion_izquierda = false;
			break;
		case ALLEGRO_KEY_UP:
			jugador.acelerando = false;
			break;
		}
}

int main() {
	if (!al_init()) {
		return -1;
	}
	//*********Inicializacion de los addons de allegro
	al_init_font_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(7);
	al_install_mouse();
	//*********Creacion de estructuras de Allegro
	ALLEGRO_DISPLAY* display = al_create_display(ANCHO_PANTALLA, ALTO_PANTALLA);
	al_set_window_title(display, "Space Invaders");
	ALLEGRO_EVENT_QUEUE* cola_eventos = al_create_event_queue();
	ALLEGRO_TIMER* timer = al_create_timer(1 / FPS);

	ALLEGRO_FONT* gameFont = nullptr;
	//ALLEGRO_SAMPLE* playerDeadSound;
	//****************************************************
	al_register_event_source(cola_eventos, al_get_display_event_source(display));
	al_register_event_source(cola_eventos, al_get_timer_event_source(timer));
	al_register_event_source(cola_eventos, al_get_keyboard_event_source());
	//***************************************************************************
	ALLEGRO_BITMAP* ImagenJugador = al_load_bitmap("Assets/Nave_Jugador.png");
	ALLEGRO_BITMAP* ImagenBala = al_load_bitmap("Assets/BALAA.png");
	//Inicializar Jugador
	Jugador jugador;
	jugador.acelerando = false;
	jugador.rotacion_derecha = false;
	jugador.rotacion_izquierda= false;
	jugador.velocidad = 0.5;
	jugador.vidas = 5;
	jugador.X = ANCHO_PANTALLA / 2;
	jugador.Y = ALTO_PANTALLA / 2;
	jugador.vivo = true;
	jugador.angulo = 0;
	jugador.dx = 0;
	jugador.dy = 0;
	//Bala************************************************
	std::vector<PtrBala> Balas;
	
	al_start_timer(timer);
	while (true) {
		ALLEGRO_EVENT Evento;
		al_wait_for_event(cola_eventos, &Evento);
		
		if (Evento.type == ALLEGRO_EVENT_KEY_DOWN) {
			RecibirTeclasPresionadas(Evento, jugador,Balas);
		}
		else if (Evento.type == ALLEGRO_EVENT_KEY_UP) {
			RecibirTeclasLevantadas(Evento, jugador);
		}
		Mover(jugador);
		ContarBorde(jugador);
		DrawJugador(jugador, ImagenJugador);
		Actualizar(Balas);
		DrawBala(Balas, ImagenBala);
		BorrarPosicion(Balas);
		
		al_flip_display();
		al_clear_to_color(al_map_rgb(0,0,0));
	}

}