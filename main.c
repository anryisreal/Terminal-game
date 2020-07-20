#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define mapn 3

struct player {
    int health_points;
    int damage;
    struct mobloot* loot;
};

struct mob {
    int health_points;
    int damage;
    struct mobloot* chance;
};

struct armor {
    int health_points_armor;
    char* armor;
};

struct mobloot {
    struct armor helmet;
    struct armor breastp;
    struct armor pants;
    struct armor shoes;
};

struct effect {
    int morphp;
    int morpdamage;
};

int** create_matrix(int n, int m) {
    int **arr = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        arr[i] = (int *) malloc(m * sizeof(int));
    }
    return arr;
}

void fill_matrix(int** arr, int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            arr[i][j] =  0;
        }
    }
    arr[mapn - 1][0] = 1;
    arr[0][0] = 1;
    arr[1][1] = 2;
}

void show_matrix(int** arr, int n, int m) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void battle_mob(struct player player, struct mob mob, char* scanfight, int fnum, int snum, int** map, int hp, struct mob supermob) {
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
                sleep(2);
                mob.health_points -= player.damage;
                hp -= mob.damage;
                printf("You damaged mob on %d dmg, mob hp = %d\n", player.damage, mob.health_points);
                sleep(2);
            }
            if (mob.health_points == 0) {
                printf("\nYou killed mob, congratulations!!!\n");
                map[0][0] = 0;
                int chancem = rand() % 100 + 1;
                if (chancem == 5 || chancem == 10 || chancem == 15 || chancem == 20) {
                    if (strcmp(player.loot->helmet.armor, "Helmet") == 0) {
                        printf("You give loot from the mob, it's Helmet, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Helmet");
                        player.loot->helmet.armor = mob.chance->helmet.armor;
                        player.health_points += mob.chance->helmet.health_points_armor;
                        hp += mob.chance->helmet.health_points_armor;
                    }
                }
                if (chancem == 6 || chancem == 11 || chancem == 32) {
                    if (strcmp(player.loot->breastp.armor, "Breastplace") == 0) {
                        printf("You give loot from the mob, it's Breastplace, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Breastplace");
                        player.loot->breastp.armor = mob.chance->breastp.armor;
                        player.health_points += mob.chance->breastp.health_points_armor;
                        hp += mob.chance->breastp.health_points_armor;
                    }
                }
                if (chancem == 7 || chancem == 12 || chancem == 16 || chancem == 21) {
                    if (strcmp(player.loot->pants.armor, "Pants") == 0) {
                        printf("You give loot from the mob, it's Pants, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Pants");
                        player.loot->breastp.armor = mob.chance->breastp.armor;
                        player.health_points += mob.chance->breastp.health_points_armor;
                        hp += mob.chance->breastp.health_points_armor;
                    }
                }
                if (chancem == 8 || chancem == 13 || chancem == 17 || chancem == 22 || chancem == 27) {
                    if (strcmp(player.loot->shoes.armor, "Shoes") == 0) {
                        printf("You give loot from the mob, it's Shoes, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Shoes");
                        player.loot->shoes.armor = mob.chance->shoes.armor;
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
                if (chancem == 5 || chancem == 10 || chancem == 15 || chancem == 20) {
                    if (strcmp(player.loot->helmet.armor, "Helmet") == 0) {
                        printf("You give loot from the mob, it's Helmet, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Helmet");
                        player.loot->helmet.armor = supermob.chance->helmet.armor;
                        player.health_points += supermob.chance->helmet.health_points_armor;
                        hp += supermob.chance->helmet.health_points_armor;
                    }
                }
                if (chancem == 6 || chancem == 11 || chancem == 32) {
                    if (strcmp(player.loot->breastp.armor, "Breastplace") == 0) {
                        printf("You give loot from the mob, it's Breastplace, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Breastplace");
                        player.loot->breastp.armor = supermob.chance->breastp.armor;
                        player.health_points += supermob.chance->breastp.health_points_armor;
                        hp += supermob.chance->breastp.health_points_armor;
                    }
                }
                if (chancem == 7 || chancem == 12 || chancem == 16 || chancem == 21) {
                    if (strcmp(player.loot->pants.armor, "Pants") == 0) {
                        printf("You give loot from the mob, it's Pants, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Pants");
                        player.loot->breastp.armor = supermob.chance->breastp.armor;
                        player.health_points += supermob.chance->breastp.health_points_armor;
                        hp += supermob.chance->breastp.health_points_armor;
                    }
                }
                if (chancem == 8 || chancem == 13 || chancem == 17 || chancem == 22 || chancem == 27) {
                    if (strcmp(player.loot->shoes.armor, "Shoes") == 0) {
                        printf("You give loot from the mob, it's Shoes, but you already have this.\n");
                    } else {
                        printf("Congratulations, you receive Shoes");
                        player.loot->shoes.armor = supermob.chance->shoes.armor;
                        player.health_points += supermob.chance->shoes.health_points_armor;
                        hp += supermob.chance->shoes.health_points_armor;
                    }
                }
            }
        }
    }
}

void move(char* scan, int** map, struct player player, struct mob mob, int fnum, int snum, int hp, struct mob supermob) {
    int i = 0;
    for(; ;) {
        battle_mob(player, mob, scan, fnum, snum, map, hp, supermob);
        i++;
        i--;
        gets(scan);
        if (strcmp(scan, "right") == 0) {
            if(snum + 1 >= mapn) {
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
            if(fnum + 1 >= mapn) {
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
    struct effect poison {
        
    };
    struct player player = {
            hp,
            2
    };
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
    struct mobloot chance = {
            helmet,
            breastp,
            pants,
            shoes
    };
    struct mob mob = {
            30,
            3,
            &chance
    };
    struct mob supermob = {
            50,
            5,
            &chance
    };
    int fnum = mapn - 1, snum = 0;
    int** map = create_matrix(mapn, mapn);
    fill_matrix(map, mapn, mapn);
    show_matrix(map, mapn, mapn);
    char scan[20];
    move(scan, map, player, mob, fnum, snum, hp, supermob);
    return 0;
}
