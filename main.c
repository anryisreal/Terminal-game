#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAP_SIZE 5

#define DEFAULT_PLAYER_HP 20
#define DEFAULT_PLAYER_DAMAGE 2

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
    if (game == NULL) {
        return -1;
    }
    move(&(game->player), 2, 2);
    run(game);

    free_game(game);
}

/*
// Refactor
void fill_matrix(int** arr, int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            arr[i][j] =  0;
        }
    }
    arr[MAPN - 1][0] = 1;
    arr[0][0] = 1;
    arr[1][1] = 2;
}




void battle_mob(struct entity player, struct entity mob, char* scanfight, int fnum, int snum, int** map, int hp, struct entity supermob) {
    if(fnum == 0 && snum == 0 && map[0][0] == 1) {
        player.health_points = hp;
        printf("You want a battle?\n");
        gets(scanfight);
        if(strcmp(scanfight, "fight") == 0) {
            while (mob.health_points > 0) {
                if (player.health_points <= 0) {
                    printf("You died");
                    break;
                }
                player.health_points -= mob.damage;
                hp -= mob.damage;
                printf("Mob damaged you on %d dmg, your hp = %d\n", mob.damage, player.health_points);
                //sleep(2);
                mob.health_points -= player.damage;
                hp -= mob.damage;
                printf("You damaged mob on %d dmg, mob hp = %d\n", player.damage, mob.health_points);
                //sleep(2);
            }
            if (mob.health_points == 0) {
                printf("\nYou killed mob, congratulations!!!\n");
                map[0][0] = 0;
                int chancem = rand() % 80 + 1;
                printf("%d", chancem);
                if (chancem == 50 || chancem == 10 || chancem == 15 || chancem == 20) {
                    if (strcmp(player.chance->helmet.armor, "Helmet") == 0) {
                        printf("You give loot from the mob, it's Helmet, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Helmet");
                        player.chance->helmet.armor = mob.chance->helmet.armor;
                        player.health_points += mob.chance->helmet.health_points_armor;
                        hp += mob.chance->helmet.health_points_armor;
                    }
                }
                if (chancem == 6 || chancem == 11 || chancem == 32) {
                    if (strcmp(player.chance->breastp.armor, "Breastplace") == 0) {
                        printf("You give loot from the mob, it's Breastplace, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Breastplace");
                        player.chance->breastp.armor = mob.chance->breastp.armor;
                        player.health_points += mob.chance->breastp.health_points_armor;
                        hp += mob.chance->breastp.health_points_armor;
                    }
                }
                if (chancem == 7 || chancem == 12 || chancem == 16 || chancem == 21) {
                    if (strcmp(player.chance->pants.armor, "Pants") == 0) {
                        printf("You give loot from the mob, it's Pants, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Pants");
                        player.chance->breastp.armor = mob.chance->breastp.armor;
                        player.health_points += mob.chance->breastp.health_points_armor;
                        hp += mob.chance->breastp.health_points_armor;
                    }
                }
                if (chancem == 8 || chancem == 13 || chancem == 17 || chancem == 22 || chancem == 79) {
                    if (strcmp(player.chance->shoes.armor, "Shoes") == 0) {
                        printf("You give loot from the mob, it's Shoes, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Shoes");
                        player.chance->shoes.armor = mob.chance->shoes.armor;
                        player.health_points += mob.chance->shoes.health_points_armor;
                        hp += mob.chance->shoes.health_points_armor;
                    }
                }
            }
        }
    }
    if(fnum == 1 && snum == 1 && map[1][1] == 2) {
        player.health_points = hp;
        printf("You want a battle?\n");
        printf("If you want a battle say: fight\n");
        gets(scanfight);
        if(strcmp(scanfight, "fight") == 0) {
            while (supermob.health_points > 0) {
                if (player.health_points <= 0) {
                    printf("You died");
                    break;
                }
                player.health_points -= supermob.damage;
                hp -= supermob.damage;
                printf("Mob damaged you on %d dmg, your hp = %d\n", supermob.damage, player.health_points);
                sleep(2);
                supermob.health_points -= player.damage;
                hp -= supermob.damage;
                printf("You damaged mob on %d dmg, mob hp = %d\n", player.damage, supermob.health_points);
                sleep(2);
            }
            if (supermob.health_points == 0) {
                printf("\nYou killed supermob, congratulations!!!\n");
                map[1][1] = 0;
                int chancem = rand() % 100 + 1;
                a <= [1 : 100]
                a % 6 == 0 (P = 1 / 6)
                printf("%d", chancem);
                if (chancem == 5 || chancem == 10 || chancem == 15 || chancem == 20) {
                    if (strcmp(player.chance->helmet.armor, "Helmet") == 0) {
                        printf("You give loot from the mob, it's Helmet, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Helmet");
                        player.chance->helmet.armor = supermob.chance->helmet.armor;
                        player.health_points += supermob.chance->helmet.health_points_armor;
                        hp += supermob.chance->helmet.health_points_armor;
                    }
                }
                if (chancem == 6 || chancem == 11 || chancem == 32) {
                    if (strcmp(player.chance->breastp.armor, "Breastplace") == 0) {
                        printf("You give loot from the mob, it's Breastplace, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Breastplace");
                        player.chance->breastp.armor = supermob.chance->breastp.armor;
                        player.health_points += supermob.chance->breastp.health_points_armor;
                        hp += supermob.chance->breastp.health_points_armor;
                    }
                }
                if (chancem == 7 || chancem == 12 || chancem == 16 || chancem == 21) {
                    if (strcmp(player.chance->pants.armor, "Pants") == 0) {
                        printf("You give loot from the mob, it's Pants, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Pants");
                        player.chance->breastp.armor = supermob.chance->breastp.armor;
                        player.health_points += supermob.chance->breastp.health_points_armor;
                        hp += supermob.chance->breastp.health_points_armor;
                    }
                }
                if (chancem == 8 || chancem == 13 || chancem == 17 || chancem == 22 || chancem == 27) {
                    if (strcmp(player.chance->shoes.armor, "Shoes") == 0) {
                        printf("You give loot from the mob, it's Shoes, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Shoes");
                        player.chance->shoes.armor = supermob.chance->shoes.armor;
                        player.health_points += supermob.chance->shoes.health_points_armor;
                        hp += supermob.chance->shoes.health_points_armor;
                    }
                }
            }
        }
    }
}

void move(char* scan, int** map, struct entity player, struct entity mob, int fnum, int snum, int hp, struct entity supermob) {
    int i = 0;
    for(; ;) {
        battle_mob(player, mob, scan, fnum, snum, map, hp, supermob);
        i++;
        i--;
        gets(scan);
        if (strcmp(scan, "right") == 0) {
            if(snum + 1 >= MAPN) {
                printf("Sorry, You are going over the edge\n");
            }
            else {
                snum += 1;
            }
        }
        if (strcmp(scan, "left") == 0) {
            if(snum - 1 < 0) {
                printf("Sorry, You are going over the edge\n");
            }
            else {
                snum -= 1;
            }
        }
        if (strcmp(scan, "forward") == 0) {
            if(fnum - 1 < 0) {
                printf("Sorry, You are going over the edge\n");
            }
            else {
                fnum -= 1;
            }
        }
        if (strcmp(scan, "back") == 0) {
            if(fnum + 1 >= MAPN) {
                printf("Sorry, You are going over the edge\n");
            }
            else {
                fnum += 1;
            }
        }
    }
}

int main() {
    srand(time(NULL));
    int hp = 100;
   
    struct armor helmet = {
            15,
            "Helmet"
    };
    struct armor breastp = {
            35,
            "Breastplace"
    };
    struct armor pants = {
            25,
            "Pants"
    };
    struct armor shoes = {
            10,
            "Shoes"
    };
        struct armor p_helmet = {
            0,
            ""
    };
    struct armor p_breastp = {
            0,
            ""
    };
    struct armor p_pants = {
            0,
            ""
    };
    struct armor p_shoes = {
            0,
            ""
    };
    struct mobloot p_chance = {
        p_helmet,
        p_breastp,
        p_pants,
        p_shoes
    };
    struct mobloot chance = {
            helmet,
            breastp,
            pants,
            shoes
    };
        struct entity player = {
            hp,
            2,
            &p_chance,
            0,
            MAPN - 1
    };
    struct entity mob = {
            30,
            3,
            &chance,
            0,
            0
    };
    struct entity supermob = {
            50,
            5,
            &chance,
            1,
            1
    };
    int fnum = MAPN - 1, snum = 0;
    int** map = create_matrix(MAPN, MAPN);
    if (map == NULL) {
        return -1;
    }
    fill_matrix(map, MAPN, MAPN);
    show_matrix(map, MAPN, MAPN);
    char scan[20];
    move(scan, map, player, mob, fnum, snum, hp, supermob);
    return 0;
}*/
