#include <dos.h>
#include <stdio.h>
#include <conio.h>

unsigned long far *count = (unsigned long far *)MK_FP(0x0040, 0x006c);

unsigned int interval, step;

void interrupt beep(...){
    static unsigned long last_time = 0;
    static unsigned int current_interval = interval;
    static int active = 1;

    if (*count - last_time >= current_interval && active){
	    sound(1000);
	    delay(50);
	    nosound();

	    last_time = *count;
	    if (current_interval > step){
		current_interval -= step;
	    } else {
		active = 0;
	    }
    }
}

int main(){
    clrscr();
    float interval_sec, step_sec;

    printf("Enter the initial interval (in seconds): ");
    scanf("%f", &interval_sec);
    printf("Enter the decrement step (in seconds): ");
    scanf("%f", &step_sec);

    interval = (unsigned int)(interval_sec / 0.055);
    step = (unsigned int)(step_sec / 0.055);

    if (interval <= step){
	    printf("Error: the step must be smaller than the initial interval.\n");
	    getch();
	    return 1;
    }

    void interrupt (*oldvect)(...);
    oldvect = getvect(0x1c);
    setvect(0x1c, beep);

    printf("Press any key to stop...\n");
    getch();

    setvect(0x1c, oldvect);
    return 0;
}


