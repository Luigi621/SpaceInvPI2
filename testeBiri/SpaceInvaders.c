#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

const float FPS = 60.0;
const int screen_w = 640;
const int screen_h = 480;
const int tamanho_jogador = 32;
const int tamanho_inimigo = 32;

typedef struct {
    float x, y;
    float vel_jogador;
} Player;

typedef struct {
    float x, y;
    bool active;
} Tiro;

typedef struct {
    double x;
    double y;
    bool active;
} Inimigo;

int main() {
    
    al_init();
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();

    // Criando display e timer
    ALLEGRO_DISPLAY* display = al_create_display(screen_w, screen_h);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    // Objetos
    Player jogador = { screen_w / 2 - tamanho_jogador / 2, screen_h - tamanho_jogador, 5.0 };
    Tiro bala = { jogador.x, jogador.y, false };
    Inimigo inimigos[5];
    bool andar_esquerda = false;
    bool andar_direita = false;

    // Inicializando os inimigos
    for (int i = 0; i < 5; i++) {
        inimigos[i].x = i * 100 + 50.0;
        inimigos[i].y = 50.0;
        inimigos[i].active = true;
    }

    // Registrar eventos
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);
    bool redraw = true;
    bool Sair_jogo = false;

    // Laço principal
    while (!Sair_jogo) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        //Condição para checar a tecla apertada
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                andar_esquerda = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                andar_direita = true;
                break;
            case ALLEGRO_KEY_A:
                andar_esquerda = true;
                break;
            case ALLEGRO_KEY_D:
                andar_direita = true;
                break;
            case ALLEGRO_KEY_SPACE:
                if (!bala.active) {
                    bala.x = jogador.x + tamanho_jogador / 2;
                    bala.y = jogador.y;
                    bala.active = true;
                }
                break;
            }
}       
        //Checa se a tecla foi solta
        else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
            switch (ev.keyboard.keycode) {
            case ALLEGRO_KEY_LEFT:
                andar_esquerda = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                andar_direita = false;
                break;
            case ALLEGRO_KEY_A:
                andar_esquerda = false;
                break;
            case ALLEGRO_KEY_D:
                andar_direita = false;
                break;
            }
}

        if (ev.type == ALLEGRO_EVENT_TIMER) {

            // Atualizar posição do jogador com base em teclas pressionadas
            if (andar_esquerda == true) {
                jogador.x -= jogador.vel_jogador;
            }
            if (andar_direita == true) {
                jogador.x += jogador.vel_jogador;
            }

            // Atualizar posição do tiro
            if (bala.active) {
                bala.y -= 5.0;
                if (bala.y < 0) {
                    bala.active = false;
                }
            }

            // Verificar se o tiro pegou em algum inimigo
            for (int i = 0; i < 5; i++) {
                if (inimigos[i].active && bala.active &&
                    bala.x < inimigos[i].x + tamanho_inimigo &&
                    bala.x + 5 > inimigos[i].x &&
                    bala.y < inimigos[i].y + tamanho_inimigo &&
                    bala.y + 10 > inimigos[i].y) {
                    inimigos[i].active = false;
                    bala.active = false;
                }
            }

            redraw = true;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            Sair_jogo = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Atirando
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE && !bala.active) {
                bala.x = jogador.x + tamanho_jogador / 2;
                bala.y = jogador.y;
                bala.active = true;
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            // Desenhar tudo
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // Desenhar player
            al_draw_filled_rectangle(jogador.x, jogador.y, jogador.x + tamanho_jogador, jogador.y + tamanho_jogador, al_map_rgb(0, 255, 0));

            // Desenhar tiro
            if (bala.active) {
                al_draw_filled_rectangle(bala.x, bala.y, bala.x + 5, bala.y + 10, al_map_rgb(255, 255, 255));
            }

            // Desenhar inimigos
            for (int i = 0; i < 5; i++) {
                if (inimigos[i].active) {
                    al_draw_filled_rectangle(inimigos[i].x, inimigos[i].y, inimigos[i].x + tamanho_inimigo, inimigos[i].y + tamanho_inimigo, al_map_rgb(255, 0, 0));
                }
            }

            al_flip_display();
        }
    }

    // Finalizando o Allegro
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
