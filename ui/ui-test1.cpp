#include <gtk/gtk.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define HOVER 120
#define MAX_SPEED 255

typedef enum inst {
    OFF,
    INIT,
    RESET,
    ROT_L,
    ROT_R,
    ACC_F,
    ACC_B,
    ACC_L,
    ACC_R,
    RAISE,
    LOWER
} INSTRUCTION;

pthread_t running = 0;
// pid_t rotateL = 0;
// pid_t rotateR = 0;
// pid_t raise = 0;
// pid_t lower = 0;
// pid_t moveForward = 0;
// pid_t moveBackward = 0;
// pid_t moveLeft = 0;
// pid_t moveRight = 0;

//LAYOUT OF DRONE
// M1----M2
// |     |
// M3----M4

int Motor1 = 0;
int Motor2 = 0;
int Motor3 = 0;
int Motor4 = 0;

static void print_hello (GtkWidget *widget, gpointer   data)
{
  g_print ("Hello World\n");
}

void *instruction_handler(void *varg) {
  INSTRUCTION * t_ins = (INSTRUCTION *) varg;
  INSTRUCTION ins = *t_ins;
  while(running != 0) {
    if(ins == ROT_L) {
      if(Motor1 > 100 && Motor4 > 100 && Motor2 < 140 && Motor3 < 140) {
        Motor1--;
        Motor4--;

        Motor2++;
        Motor3++;
      }
    }
    else if(ins == ROT_R) {
      if(Motor2 > 100 && Motor3 > 100 && Motor1 < 140 && Motor4 < 140) {
        Motor1++;
        Motor4++;

        Motor2--;
        Motor3--;
      }
    }
    else if(ins == ACC_R) {
      if(Motor1 < 140 && Motor3 < 140 && Motor2 > 100 && Motor4 > 100) {
        Motor1++;
        Motor3++;

        Motor2--;
        Motor4--;
      }
    }
    else if(ins == ACC_L) {
      if(Motor2 < 140 && Motor4 < 140 && Motor1 > 100 && Motor3 > 100) {
        Motor2++;
        Motor4++;

        Motor1--;
        Motor3--;
      }
    }
    else if(ins == ACC_F) {
      if(Motor3 < 140 && Motor4 < 140 && Motor1 > 100 && Motor2 > 100) {
        Motor3++;
        Motor4++;

        Motor1--;
        Motor2--;
      }
    }
    else if(ins == ACC_B) {
      if(Motor1 < 140 && Motor2 < 140 && Motor3 > 100 && Motor4 > 100) {
        Motor1++;
        Motor2++;

        Motor3--;
        Motor4--;
      }
    }
    else if(ins == RAISE) {
      if(Motor1 < MAX_SPEED && Motor2 < MAX_SPEED && Motor3 < MAX_SPEED && Motor4 < MAX_SPEED) {
        Motor1++;
        Motor2++;
        Motor3++;
        Motor4++;
      }
    }
    else if(ins == LOWER) {
      if(Motor1 > 0 && Motor2 > 0 && Motor3 > 0 && Motor4 > 0) {
        Motor1--;
        Motor2--;
        Motor3--;
        Motor4--;
      }
    }
    printf("New speed M1: %d M2: %d M3: %d M4: %d\r", Motor1, Motor2, Motor3, Motor4);
    fflush(stdout);
    timespec time;
    time.tv_nsec = 1000000000/4; //half second (1 billion nanosec in a sec)
    nanosleep(&time, NULL);

  }
  return NULL;
}

gboolean handle_key_release(GtkWidget *widget, GdkEventKey *event, gpointer data) {
  if(running != 0) 
  {
    // pthread_kill(running, SIGINT);
    running = 0;
    //Submit reset to socket here
    printf("New speed M1: %d M2: %d M3: %d M4: %d\n", Motor1, Motor2, Motor3, Motor4);

    Motor1 = Motor2 = Motor3 = Motor4 = HOVER;
    printf("RESET M1: %d M2: %d M3: %d M4: %d\n", Motor1, Motor2, Motor3, Motor4);

  }
  return TRUE;
}

//Function is called multiple times if key is being held
gboolean handle_key_press (GtkWidget *widget, GdkEventKey *event, gpointer data) {

  INSTRUCTION ins;
  if(event -> keyval == GDK_KEY_A || event -> keyval == GDK_KEY_a && running == 0) {
    ins = ROT_L;
    pthread_create(&running, NULL, (instruction_handler), (void *) &ins);

  }
  else if(event -> keyval == GDK_KEY_S || event -> keyval == GDK_KEY_s  && running == 0) {
    ins = LOWER;
    pthread_create(&running, NULL, (instruction_handler), (void *) &ins);
  }

  else if(event -> keyval == GDK_KEY_W || event -> keyval == GDK_KEY_w  && running == 0) {
    ins = RAISE;
    pthread_create(&running, NULL, (instruction_handler), (void *) &ins);
  }

  else if(event -> keyval == GDK_KEY_D || event -> keyval == GDK_KEY_d  && running == 0) {
    ins = ROT_R;
    pthread_create(&running, NULL, (instruction_handler), (void *) &ins);
  }

  if(event -> keyval == GDK_KEY_Up && running == 0) {
    ins = ACC_F;
    pthread_create(&running, NULL, (instruction_handler), (void *) &ins);
  }

  else if(event -> keyval == GDK_KEY_Down && running == 0) {
    ins = ACC_B;
    pthread_create(&running, NULL, (instruction_handler), (void *) &ins);
  }
  else if(event -> keyval == GDK_KEY_Left && running == 0) {
    ins = ACC_L;
    pthread_create(&running, NULL, (instruction_handler), (void *) &ins);
  }
  else if(event -> keyval == GDK_KEY_Right && running == 0) {
    ins = ACC_R;
    pthread_create(&running, NULL, (instruction_handler), (void *) &ins);
  }
  

  return TRUE;
}

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  // GtkWidget *button;
  // GtkWidget *button_box;
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);  
  g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (handle_key_press), NULL);
  g_signal_connect (G_OBJECT (window), "key_release_event", G_CALLBACK (handle_key_release), NULL);

  // button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  // gtk_container_add (GTK_CONTAINER (window), button_box);

  // button = gtk_button_new_with_label ("Hello World");
  // g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  // gtk_container_add (GTK_CONTAINER (button_box), button);

  gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;
  Motor1 = Motor2 = Motor3 = Motor4 = HOVER;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}