#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAP_SIZE 5

#define DEFAULT_PLAYER_HP 20
#define DEFAULT_PLAYER_DAMAGE 2
#define DEFFAULT_MOB_HP 50
#define DEFFAULT_MOB_DAMAGE 1

#define CMD_BUFFER_SIZE 20


enum state {
    MOVE = 0,
    FIGHT
};

enum action {
    HIT_ENEMY = 0,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_FORWARD,
    MOVE_BACKWARD,
    UNDEF_ACTION
};

struct entity {
    int health_points;
    int damage;
    struct armor* armor_list;
    int x;
    int y;
};

struct armor {
    int health_points;
    char* armor_name;
};

struct cell {
    struct entity* mob;
};

struct game {
    struct cell** field;
    int x_size;
    int y_size;

    struct entity player;
    enum state state;
};

// struct effect {
//     int morphp;
//     int morpdamage;
// };

struct cell** create_matrix(int n, int m) {
    struct cell**arr = (struct cell**) calloc(n, sizeof(struct cell*));
    if(arr == NULL) {
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        arr[i] = (struct cell*) calloc(m, sizeof(struct cell));
        if(arr[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(arr[j]);
            }
            free(arr);
            return NULL;
        }
    }
    return arr;
}

void free_cell(struct cell cell) {
    free(cell.mob);
}

void free_matrix(struct cell** arr, int row_cnt, int col_cnt) {
    for (int i = 0; i < row_cnt; i++) {
        for (int j = 0; j < col_cnt; j++) {
            free_cell(arr[i][j]);
        }
        free(arr[i]);
    }
    free(arr);
}

void add_mob(struct game* game, struct entity* mob, int x_pos, int y_pos) {
    mob = {
        DEFFAULT_MOB_HP,
        DEFFAULT_MOB_DAMAGE,
        NULL,
        int x_pos,
        int y_pos
    };
    game->field[y_pos][x_pos].mob = mob; 
}

struct game* init_game() {
    struct game* game = (struct game*) calloc(1, sizeof(struct game));
    if (game == NULL) {
        return NULL;
    }

    struct entity player = {
        DEFAULT_PLAYER_HP,
        DEFAULT_PLAYER_DAMAGE,
        NULL,
        0, 
        0
    };
    game->x_size = MAP_SIZE;
    game->y_size = MAP_SIZE;
    game->player = player;
    game->field = create_matrix(MAP_SIZE, MAP_SIZE);
    if (game->field == NULL) {
        free(game);
        return NULL;
    }
    return game;
}

enum action get_action_from_cmd(char* cmd) {
    if (strcmp(cmd, "hit\n") == 0) {
        return HIT_ENEMY;
    }
    if (strcmp(cmd, "move left\n") == 0) {
        return MOVE_LEFT;
    }
    if (strcmp(cmd, "move right\n") == 0) {
        return MOVE_RIGHT;
    }
    if (strcmp(cmd, "move forward\n") == 0) {
        return MOVE_FORWARD;
    }
    if (strcmp(cmd, "move backward\n") == 0) {
        return MOVE_BACKWARD;
    }
    return UNDEF_ACTION;
}

int is_entity_alive(struct entity* e) {
    return e->health_points > 0;
}

int is_possible_move(struct game* game, enum action action) {
    if (action == MOVE_LEFT && game->player.x == 0) {
        return 0;
    }
    if (action == MOVE_RIGHT && game->player.x == game->x_size - 1) {
        return 0;
    }
    if (action == MOVE_BACKWARD && game->player.y == 0) {
        return 0;
    }
    if (action == MOVE_FORWARD && game->player.y == game->y_size - 1) {
        return 0;
    }
    return 1;
}

int is_possible_cmd(struct game* game, enum action action) {
    if (action == UNDEF_ACTION) {
        return 0;
    }

    switch (game->state) {
        case MOVE: {
            if (action == HIT_ENEMY) {
                return 0;
            }
            if (!is_possible_move(game, action)) {
                return 0;
            }
        }
        default: {
            return 1;
        }
    }
    return 1;
}

void show_map(struct game* game) {
    for (int i = game->y_size - 1; i >= 0; i--) {
        for (int j = 0; j < game->x_size; j++) {
            if (game->player.x == j && game->player.y == i) {
                printf("P ");
            } else if (game->field[i][j].mob != NULL) {
                printf("M ");
            } else {
                printf(". ");
            }
        }
        printf("\n\n");
    }
}

void move(struct entity* p, int x_shift, int y_shift) {
    p->x += x_shift;
    p->y += y_shift;
}

void update_state(struct game* game) {
    if (game->field[game->player.y][game->player.x] == game->field[game->mob.y][game->mob.x]) {
        game->state = FIGHT;
    }
    if (game->field[game->player.y][game->player.x] != game->field[game->mob.y][game->mob.x]) {
        game->state = MOVE;
    }
}

void run(struct game* game) {
    char cmd_buf[CMD_BUFFER_SIZE];
    char* cmd = cmd_buf;

    while (is_entity_alive(&(game->player))) {
        show_map(game);

        cmd = fgets(cmd, CMD_BUFFER_SIZE, stdin);
        if (cmd == NULL) {
            break;
        }

        enum action action = get_action_from_cmd(cmd);
        if (is_possible_cmd(game, action)) {
            if (action == MOVE_LEFT) {
                move(&(game->player), -1, 0);
            } else if (action == MOVE_RIGHT) {
                move(&(game->player), 1, 0);
            } else if (action == MOVE_FORWARD) {
                move(&(game->player), 0, 1);
            } else if (action == MOVE_BACKWARD) {
                move(&(game->player), 0, -1);
            } else {
                printf("Unimplemented command!\n");
            }
        } else {
            printf("Cmd is not possible %s\n", cmd);
        }

        // struct cell cell = game->field[game->player.y][game->player.x];
        // if (cell.mob != NULL) {
        //     game->state = FIGHT;
        // }
        // if (cell.clothes != NULL) { game->state = PICK; } etc.
        // TODO: void update_state(game);
    }
}

void free_game(struct game* game) {
    free_matrix(game->field, game->y_size, game->x_size);
    free(game);
}

int main() {
    struct game* game = init_game();
    struct entity* mob = (struct entity*) calloc(1, sizeof(struct entity));
    add_mob(game, mob, 1, 1);
    if (game == NULL) {
        return -1;
    }
    move(&(game->player), 2, 2);
    run(game);

    free_game(game);
}

/*
void fill_matrix(int** arr, int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            arr[i][j] =  0;
        }
    }
    arr[MAPN - 1][0] = 1;
    arr[0][0] = 1;
    arr[1][1] = 2;
} */
