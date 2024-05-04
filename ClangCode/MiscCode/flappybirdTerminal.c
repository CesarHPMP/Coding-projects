#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define MAX 5 // Maximum number of columns
#define SCREEN_WIDTH 165
#define SCREEN_HEIGHT 55 // Adjusted to fit within a standard terminal window
#define BIRD_UP 'w'
#define BIRD_DOWN 's'

typedef struct {
    int posx;
    int posy;
} Bird;

typedef struct {
    int posx;
    int posy;
} Position;

typedef struct {
    Position posit[MAX];
} Column;

int verpos(Bird passaro, Column coluna);
void run(Bird *passaro, Column *coluna);
void createmap(Bird *passaro, Column *coluna);
void newframe(Bird *passaro, Column *coluna);
void newcolumn(Position *coluna);
int retry(void);

size_t FRAME_DELAY_US = 100000; // Frame update interval in microseconds

int main() {
    Bird passaro;
    Column coluna;
    int c;

    printf("Qual dificuldade vc quer? de 1, 2, 3, 4 ou 5\nResposta: ");
    scanf("%d", &c);
    FRAME_DELAY_US = FRAME_DELAY_US / c;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE); // Set no delay for getch()

    run(&passaro, &coluna);

    endwin();

    return 0;
}

int verpos(Bird passaro, Column coluna) {
    for (int i = 0; i < MAX; i++) {
        if (passaro.posx == coluna.posit[i].posx) {
            if (coluna.posit[i].posy > SCREEN_HEIGHT/2 && passaro.posy > coluna.posit[i].posy){
            	return 1;
            }
            
            if (coluna.posit[i].posy < SCREEN_HEIGHT/2 && passaro.posy < coluna.posit[i].posy){
            	return 1;
            }
        }
    }
    return 0; // No collision
}

void run(Bird *passaro, Column *coluna) {
    int counter = 0;
    int speed = 0;
    int modifier = 0;
    
    createmap(passaro, coluna);
    newframe(passaro, coluna);

    do {
        while (!verpos(*passaro, *coluna)) {
            char input = getch();
            if (input == BIRD_UP && passaro->posy > 0) {
                passaro->posy -= 1;
            } else if (input == BIRD_DOWN && passaro->posy < SCREEN_HEIGHT - 1) {
                passaro->posy += 1;
            }
 
	if (counter == 4) { // Move bird down every 3 frames without input
                if (passaro->posy < SCREEN_HEIGHT - 1) {
                    passaro->posy += 1;
                }
                counter = 0; // Reset counter
                speed++;
                if(speed == 100)
                {
            	    FRAME_DELAY_US -= 100;
            	    modifier++;
            	}
 
            } else {
                counter++; // Increment counter
            }
 
            for (int i = 0; i < MAX; i++) {
                coluna->posit[i].posx -= 1;
                if (coluna->posit[i].posx < 0)
                    newcolumn(&coluna->posit[i]);
            }
 
            newframe(passaro, coluna);
            usleep(FRAME_DELAY_US);
        }

        // Check if the game should be retried
        if (retry()) {
            // Restart the game
            createmap(passaro, coluna);
            newframe(passaro, coluna);
            FRAME_DELAY_US += (modifier * 100);
            modifier = 0;
        } else {
            // Quit the game
            break;
        }
    } while (1);
}


void createmap(Bird *passaro, Column *coluna) {
    srand(time(NULL)); // Seed for random number generation

    passaro->posy = SCREEN_HEIGHT / 2; // Start bird in the middle of the screen

    // Randomize columns for the top half of the screen
    for (int i = 0; i < MAX / 2; i++) {
        coluna->posit[i].posx = rand() % (SCREEN_WIDTH - 2); // Adjusted for column width
        coluna->posit[i].posy = rand() % ((SCREEN_HEIGHT / 2) - 1) + 1; // Randomize column height for top half
    }

    // Randomize columns for the bottom half of the screen
    for (int i = MAX / 2; i < MAX; i++) {
        coluna->posit[i].posx = rand() % (SCREEN_WIDTH - 2); // Adjusted for column width
        coluna->posit[i].posy = rand() % ((SCREEN_HEIGHT / 2) - 1) + (SCREEN_HEIGHT / 2 + 1); // Randomize column height for bottom half
    }
}

void newframe(Bird *passaro, Column *coluna) {
    clear(); // Clear the screen

    // Print bird
    if (passaro->posy >= 0 && passaro->posy < SCREEN_HEIGHT) {
        mvprintw(passaro->posy, 0, ">");
    }

    // Print top columns with a gap
    for (int i = 0; i < MAX / 2; i++) {
        for (int j = 0; j < SCREEN_HEIGHT; j++) {
            if (j < coluna->posit[i].posy) {
                mvprintw(j, coluna->posit[i].posx, "||");
            }
        }
    }

    // Print bottom columns with a gap
    for (int i = MAX / 2; i < MAX; i++) {
        for (int j = SCREEN_HEIGHT - 1; j >= 0; j--) { // Loop from bottom to top
            if (j >= coluna->posit[i].posy) {
                mvprintw(j, coluna->posit[i].posx, "||");
            }
        }
    }

    refresh(); // Refresh the screen to show changes
}

int retry()
{
    clear();
    
    nodelay(stdscr, FALSE); // Set delay to wait for getch()
    
    mvprintw(SCREEN_HEIGHT/2, SCREEN_WIDTH/2, "Press Q to quit and any key to retry");
    
    refresh();

    char key = getch(); // Get the next character

    nodelay(stdscr, TRUE); // Set no delay for getch()
    
    if(key == 'q' || key == 'Q')
        return 0; // Quit the game
    else
        return 1; // Continue the game
}





void newcolumn(Position *coluna)
{
	if(coluna->posy < SCREEN_HEIGHT/2)
	{
		coluna->posx = SCREEN_WIDTH - 2; // Adjusted for column width
		coluna->posy = rand() % ((SCREEN_HEIGHT / 2) - 1) + 1; // Randomize column height for top half 	
		return;

	}
	else
	{
		coluna->posx = SCREEN_WIDTH - 2; // Adjusted for column width                                                .
        	coluna->posy = rand() % ((SCREEN_HEIGHT / 2) - 1) + (SCREEN_HEIGHT / 2 + 1); // Randomize column height for bottom half
		return;
	
	}

}
