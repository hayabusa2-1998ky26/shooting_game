#include <stdio.h>
#include <conio.h>
#include <windows.h>

const int ROWS = 50;
const int COLUMNS = 30;
#define MAX_BULLET_AMOUNT 100


// 弾
typedef struct {
    int bx;
    int by;
    int bdx;
    int bdy;
    int damage;
    int is_active;
} Bullet;

void Bullet_init(Bullet *self, int bx, int by, int bdx, int bdy, int damage, int is_active) {
    self->bx = bx;
    self->by = by;
    self->bdx = bdx;
    self->bdy = bdy;
    self->damage = damage;
    self->is_active = is_active;
}

void Bullet_update(Bullet *self) {
    self->bx += self->bdx;
    self->by += self->bdy;
    int bx = self->bx;
    int by = self->by;

    if (bx < 1 || by < 1 || ROWS - 1 < bx || COLUMNS - 1 < by) {
        self->is_active = 0;
    }
}

Bullet bullets[MAX_BULLET_AMOUNT];

void display(int mx, int my, Bullet *bullets) {
    // 表示リストを作成
    char screen[COLUMNS][ROWS];
    for (int y = 0; y < COLUMNS; y++) {
        for (int x = 0; x < ROWS; x++) {
            if (x == 0 || x == ROWS - 1) {
                screen[y][x] = '|';
            } else if (y == 0 || y == COLUMNS - 1) {
                screen[y][x] = '-';
            } else {
                screen[y][x] = ' ';
            }
        }
    }

    // 弾を表示
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
        if (bullets[i].is_active == 1) {
            screen[bullets[i].by][bullets[i].bx] = '|';
        }
    }

    // 自機を表示
    screen[my][mx] = '/';
    screen[my][mx + 1] = '\\';

    printf("\033[2J\033[H");

    for (int y = 0; y < COLUMNS; y++) {
        for (int x = 0; x < ROWS; x++) {
            printf("%C", screen[y][x]);
        }
        printf("\n");
    }

    fflush(stdout);
    Sleep(2);
}

void spawn_bullet(Bullet bullets[], int mx, int my) {
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++){
        if (bullets[i].is_active == 0) {
            Bullet_init(&bullets[i], mx, my, 0, -1, 1, 1);
            break;
        }
    }

    for (int i = 0; i < MAX_BULLET_AMOUNT; i++){
        if (bullets[i].is_active == 0) {
            Bullet_init(&bullets[i], mx + 1, my, 0, -1, 1, 1);
            break;
        }
    }
}

int key_pressed(char key){
    if (GetAsyncKeyState(key) & 0x8000) {
        return 1;
    } else {
        return 0;
    }
}

int main(void) {
    int mx = ROWS / 2;
    int my = COLUMNS - 2;
    
    for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
        Bullet bullet;
        bullets[i] = bullet;
        Bullet_init(&bullets[i], 0, 0, 0, 0, 0, 0);
    }

    while (1) {
        display(mx, my, bullets);
        for (int i = 0; i < MAX_BULLET_AMOUNT; i++) {
            Bullet_update(&bullets[i]);
        }

        if (key_pressed('E')) {
            break;
        }

        if (key_pressed(VK_RIGHT)) {
            mx += 2;
        }

        if (key_pressed(VK_LEFT)) {
            mx -= 2;
        }

        if (key_pressed(VK_UP)) {
            my --;
        }

        if (key_pressed(VK_DOWN)) {
            my ++;
        }

        if (key_pressed('X')) {
            spawn_bullet(bullets, mx, my);
        }
    }
}

