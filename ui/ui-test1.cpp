#include <string>
#include <gtk/gtk.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "../include/Client.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define HOVER 120
#define MAX_SPEED 255

char addr[13] = "192.168.0.10";
uint32_t s = inet_addr(addr);
Client *client = new Client(8080, addr, AF_INET, SOCK_STREAM, 0);


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

typedef struct _cmd {
  INSTRUCTION ins;
  pthread_t *tid;
} COMMAND;

pthread_t rotateL = 0;
pthread_t rotateR = 0;
pthread_t up = 0;
pthread_t lower = 0;
pthread_t moveForward = 0;
pthread_t moveBackward = 0;
pthread_t moveLeft = 0;
pthread_t moveRight = 0;

pthread_mutex_t lock;
//LAYOUT OF DRONE
// M1----M2
// |     |
// M3----M4

int Motor1 = 0;
int Motor2 = 0;
int Motor3 = 0;
int Motor4 = 0;


//threaded function started by key press and 
//exits upon key release
void *instruction_handler(void *varg) {
  COMMAND * t_ins = (COMMAND *) varg;
  INSTRUCTION ins = t_ins -> ins;
  pthread_t *id = t_ins -> tid;
  char cins;
  //set the lock before checking id, any process about to manipulate the id will hold until mutex is unlocked
  pthread_mutex_lock(&lock);
  //id is the address of a global variable, that stores either 0 or the thread id
  while(*id != 0) {
    if(ins == ROT_L) {
      cins = 'a';
      client -> send(&cins, 1);
    }
    else if(ins == ROT_R) {
      cins = 'd';
      client -> send(&cins, 1);
    }
    else if(ins == ACC_R) {
      cins = '>';
      client -> send(&cins, 1);
    }
    else if(ins == ACC_L) {
      cins = '<';
      client -> send(&cins, 1);
    }
    else if(ins == ACC_F) {
      cins = '^';
      client -> send(&cins, 1);
    }
    else if(ins == ACC_B) {
      cins = 'b';
      client -> send(&cins, 1);
    }
    else if(ins == RAISE) {
      cins = 'w';
      client -> send(&cins, 1);
    }
    else if(ins == LOWER) {
      cins = 's';
      client -> send(&cins, 1);
    }
    printf("New speed M1: %d M2: %d M3: %d M4: %d\r", Motor1, Motor2, Motor3, Motor4);
    fflush(stdout);
    //motors have been adjusted, unlock the mutex and sleep for a 1/4 second
    pthread_mutex_unlock(&lock);
    timespec time;
    time.tv_nsec = 1000000000/4; //quarter second (1 billion nanosec in a sec)
    nanosleep(&time, NULL);

  }
  return NULL;
}

gboolean handle_key_release(GtkWidget *widget, GdkEventKey *event, gpointer data) {
  pthread_t *id = NULL;

  if(event -> keyval == GDK_KEY_A || event -> keyval == GDK_KEY_a)
    id = &rotateL;
  else if(event -> keyval == GDK_KEY_S || event -> keyval == GDK_KEY_s)
    id = &lower;
  else if(event -> keyval == GDK_KEY_W || event -> keyval == GDK_KEY_w)
    id = &up;

  else if(event -> keyval == GDK_KEY_D || event -> keyval == GDK_KEY_d )
    id = &rotateR;

  else if(event -> keyval == GDK_KEY_Up)
    id = &moveForward;

  else if(event -> keyval == GDK_KEY_Down )
    id = &moveBackward;
  else if(event -> keyval == GDK_KEY_Left)
    id = &moveLeft;
  else if(event -> keyval == GDK_KEY_Right)
    id = &moveRight;

  if(id != NULL) 
  {
    //lock mutex before updating id. Will wait if mutex is already locked
    //otherwise it will make the threads wait until it is unlocked
    pthread_mutex_lock(&lock);
    *id = 0;
    printf("New speed M1: %d M2: %d M3: %d M4: %d\n", Motor1, Motor2, Motor3, Motor4);

    //Submit reset to socket here
    Motor1 = Motor2 = Motor3 = Motor4 = HOVER;
    printf("RESET M1: %d M2: %d M3: %d M4: %d\n\n", Motor1, Motor2, Motor3, Motor4);
    pthread_mutex_unlock(&lock);
  }
  return TRUE;
}

//Function is called multiple times if key is being held
gboolean handle_key_press (GtkWidget *widget, GdkEventKey *event, gpointer data) {
  //command structure stores the instruction and a pointer to the thread id
  //which is used in the instruction_handler while loop
  COMMAND cmd;

  if(event -> keyval == GDK_KEY_A || event -> keyval == GDK_KEY_a && rotateL == 0) {
    cmd.ins = ROT_L;
    cmd.tid = &rotateL;
    pthread_create(&rotateL, NULL, (instruction_handler), (void *) &cmd);

  }
  else if(event -> keyval == GDK_KEY_S || event -> keyval == GDK_KEY_s  && lower == 0) {
    cmd.ins = LOWER;
    cmd.tid = &lower;
    pthread_create(&lower, NULL, (instruction_handler), (void *) &cmd);
  }

  else if(event -> keyval == GDK_KEY_W || event -> keyval == GDK_KEY_w  && up == 0) {
    cmd.ins = RAISE;
    cmd.tid = &up;
    pthread_create(&up, NULL, (instruction_handler), (void *) &cmd);
  }

  else if(event -> keyval == GDK_KEY_D || event -> keyval == GDK_KEY_d  && rotateR == 0) {
    cmd.ins = ROT_R;
    cmd.tid = &rotateR;
    pthread_create(&rotateR, NULL, (instruction_handler), (void *) &cmd);
  }

  else if(event -> keyval == GDK_KEY_Up && moveForward == 0) {
    cmd.ins = ACC_F;
    cmd.tid = &moveForward;
    pthread_create(&moveForward, NULL, (instruction_handler), (void *) &cmd);
  }

  else if(event -> keyval == GDK_KEY_Down && moveBackward == 0) {
    cmd.ins = ACC_B;
    cmd.tid = &moveBackward;
    pthread_create(&moveBackward, NULL, (instruction_handler), (void *) &cmd);
  }
  else if(event -> keyval == GDK_KEY_Left && moveLeft == 0) {
    cmd.ins = ACC_L;
    cmd.tid = &moveLeft;
    pthread_create(&moveLeft, NULL, (instruction_handler), (void *) &cmd);
  }
  else if(event -> keyval == GDK_KEY_Right && moveRight == 0) {
    cmd.ins = ACC_R;
    cmd.tid = &moveRight;
    pthread_create(&moveRight, NULL, (instruction_handler), (void *) &cmd);
  }
    

  return TRUE;
}

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

  gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);  
  g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (handle_key_press), NULL);
  g_signal_connect (G_OBJECT (window), "key_release_event", G_CALLBACK (handle_key_release), NULL);

  gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;
  Motor1 = Motor2 = Motor3 = Motor4 = HOVER;

  
  //initialize up mutex lock
  if(pthread_mutex_init(&lock, NULL) != 0) {
    perror("Couldn't initialize mutex");
    exit(EXIT_FAILURE);
  }

  app = gtk_application_new ("Drone App", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  printf("To start using the drone\n\tPlease make sure the battery is disconnected then press c.\n");
  return status;
}