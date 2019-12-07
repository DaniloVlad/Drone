/**
*@file Client_UI
*@brief UI provided to user for interaction with the client socket
*/

#include <iostream>
#include <string>
#include <gtk/gtk.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "include/Client.h"



char addr[13] = "192.168.4.1";
Client *client = new Client(8080, addr);

int calibration = 0;

//Insturction set used in key_press handler to issue correct character to socket
typedef enum inst {
    ROT_L,
    ROT_R,
    ACC_F,
    ACC_B,
    ACC_L,
    ACC_R,
    RAISE,
    LOWER
} INSTRUCTION;


//Command struct that will store an INSTRUCTION and thread id declared below
typedef struct _cmd {
  INSTRUCTION ins;
  pthread_t *tid;
} COMMAND;

//various thread id's for instructions
pthread_t rotateL = 0;
pthread_t rotateR = 0;
pthread_t up = 0;
pthread_t lower = 0;
pthread_t moveForward = 0;
pthread_t moveBackward = 0;
pthread_t moveLeft = 0;
pthread_t moveRight = 0;

//lock so that only 1 command is sent to the socket at a time
pthread_mutex_t lock;

/**
*print instructions function
*@brief - print availible commands
*/
void print_instructions() {
    std::cout << "Controls are as follows:\n\ta => Rotate Left\n\td => Rotate Right\n\tw => Raise\n\ts => Lower\n\tArrow Left => Move Left\n\tArrow Right => Move Right\n\tArrow Forward => Move Forward\n\tArrow Back => Move Backward\n\tc => Calibrate/Initialise\n\tx => Turn off motors\n\tl => Land\n\th => Hover hold (Expiremental!)\n\ti => Show instruction list" << std::endl;
}

/**
*instruction handler function
*@brief - threaded function started by key press and exits upon key release
*@param varg - this holds the arguments for the command
*/
void *instruction_handler(void *varg) {
  COMMAND * t_ins = (COMMAND *) varg;
  INSTRUCTION ins = t_ins -> ins;
  pthread_t *id = t_ins -> tid;
  char cins;
  //id is the address of a global variable, that stores either 0 or the thread id
  while(*id != 0) {

    //set the lock before checking id, any process about to manipulate the id will hold until mutex is unlocked
    pthread_mutex_lock(&lock);

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
    fflush(stdout);
    //motors have been adjusted, unlock the mutex and sleep for a 1/4 second
    pthread_mutex_unlock(&lock);
    //put the thread to sleep
    timespec time;
    time.tv_nsec = 1000000000/8; //8th of a second (1 billion nanosec in a sec)
    nanosleep(&time, NULL);

  }
  return NULL;
}

/**
*handle_key_release function
*@brief - certain key releases need to set their respective thread id to kill the thread
*@param widget - pointer to the widget in use
*@param event - pointer to the key event
*@param data - data that was send 
*/
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

    pthread_mutex_unlock(&lock);
  }
  return TRUE;
}

/**
*handle_key_press function
*@brief - Function is called multiple times if key is being held
*@param widget - the pointer for the widget that we are using
*@param event - the pointer to the key event
*@param data - 
*/
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

  else if(event -> keyval == GDK_KEY_X || event -> keyval == GDK_KEY_x) {
    std::cout << "Turning motors off!" << std::endl; 
    char x = 'x';
    client -> send(&x, 1);
  }

  else if(event -> keyval == GDK_KEY_C || event -> keyval == GDK_KEY_c) {
    char c = 'c';
    if(calibration == 0) {
      std::cout << "Starting calibration." << std::endl;
      client -> send(&c, 1);
      std::cout << "Please connect the battery and press c again once the beeping is completed!" << std::endl;
      calibration = 1;
    }

    else if(calibration == 1) {
      client -> send(&c, 1);
      std::cout << "Wait for the beeps to complete and you can start flying the drone!" << std::endl;
      calibration = 2;
    }

    else {
      std::cout << "Calibration already completed. Start flying!" << std::endl;
    }
  }

  else if(event -> keyval == GDK_KEY_H || event -> keyval == GDK_KEY_h) {
    std::cout << "You are testing the expiremental hover hold! Press h to cancel it before executing other commands!" << std::endl;
    char h = 'h';
    client -> send(&h, 1);
  }

  else if(event -> keyval == GDK_KEY_L || event -> keyval == GDK_KEY_l) {
    std::cout << "Landing initiated. Reducing motor speeds. Press x once landed to turn off motors" << std::endl;
    char l = 'l';
    client -> send(&l, 1);
  }
  else if(event -> keyval == GDK_KEY_I || event -> keyval == GDK_KEY_i) {
    print_instructions();
  }

  return TRUE;
}

/**
*activate function
*@brief - creates the windows and sets it to have the ability to get key presses
*@param app - a pointer to create the window linked with it
*@param user_data -  a gpointer object containing user data
*/
static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  
  //add key press events to the window
  gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);  
  //connect key press and key release to proper functions
  g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (handle_key_press), NULL);
  g_signal_connect (G_OBJECT (window), "key_release_event", G_CALLBACK (handle_key_release), NULL);

  //show the window
  gtk_widget_show_all (window);
}

/**
*the main method
*@brief - the main function which performs the operations required to create the client ui
*@param argc - this is the number of arguments present in the command line execution
*@param argv - a pointer to the array of arguments
*/
int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;
  std::cout << "Hello and Welcome to your Drone Controller" << std::endl;
  std::cout << "To start: please ensure the battery is disconnected from the motors and press c on your keyboard!" << std::endl;
  
  print_instructions();
  //initialize up mutex lock
  if(pthread_mutex_init(&lock, NULL) != 0) {
    perror("Couldn't initialize mutex");
    exit(EXIT_FAILURE);
  }
  //create the gtk apllication
  app = gtk_application_new ("org.app.test", G_APPLICATION_FLAGS_NONE);
  //connect the activate function
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  //run the app
  status = g_application_run (G_APPLICATION (app), argc, argv);
  //upon closing free the app
  g_object_unref (app);

  return status;
}
