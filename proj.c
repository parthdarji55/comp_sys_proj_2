#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 4
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>

pthread_t tid[NUM_THREADS];
void sig_func(int sig){
    printf("Caught signal no = %d\n", sig);
    if ( sig == 2 || sig == 4 || sig ==6 ){
        printf("Sending signal to thread: 0\n");
        pthread_kill(tid[0], SIGINT);
        pthread_kill(tid[0], SIGABRT);
        pthread_kill(tid[0], SIGILL);
    }

    if ( sig == 20 || sig == 11 || sig == 8 ){
        printf("Sending signal to thread: 1\n");
        //pthread_kill(tid[1], SIGCHLD);
        pthread_kill(tid[1], SIGSEGV);
        //pthread_kill(tid[1], SIGFPE);
    }

    if ( sig == 1 || sig == 18 ){
        printf("Sending signal to thread: 2\n");
        pthread_kill(tid[2], SIGHUP);
        pthread_kill(tid[2], SIGTSTP);
    }
}

void sig_func2(int sig) {
    printf("Caught signal no = %d, in thread 0\n", sig);
}

void sig_func3(int sig) {
    printf("Caught signal no = %d, in thread 1\n", sig);
}

void sig_func4(int sig) {
    printf("Caught signal no = %d, in thread 2\n", sig);
}

void *threadFunc (void *thread_id) {

    int id = *((int*)thread_id);
    if (id == 0 ) {
        signal( SIGINT, sig_func2);
        signal( SIGABRT, sig_func2);
        signal( SIGILL, sig_func2);
    } 
    if (id == 1 ) {
        signal(SIGCHLD, sig_func3);
        signal(SIGSEGV, sig_func3);
        signal(SIGFPE, sig_func3);
    }
    if (id ==2 ) {
        signal(SIGHUP, sig_func4);
        signal(SIGTSTP, sig_func4);
    }
    if (id == 3 ) {
    }
    printf("Thread number %d \n", id);

    /* sigset_t set;
    int s;
    sigemptyset(&set);
    sigaddset(&set, SIGSEGV);
    sigaddset(&set, SIGUSR1);
    s = sigprocmask(SIG_BLOCK, &set, NULL);
    //s = pthread_sigmask(SIG_BLOCK, &set, NULL);
    printf("%d", s);*/
}

int main () {
 
    signal(SIGINT, sig_func);
    signal(SIGABRT, sig_func);
    signal(SIGILL, sig_func);
    signal(SIGCHLD, sig_func);
    signal(SIGSEGV, sig_func);
    signal(SIGFPE, sig_func);
    signal(SIGHUP, sig_func);
    signal(SIGTSTP, sig_func);
    int ids[4];
    for (int i = 0; i < NUM_THREADS; i++){
        ids[i] = i;
        pthread_create(&tid[i], NULL, threadFunc, &ids[i]);
    }
    
    sleep(1);

    kill(getpid(), SIGINT);
    kill(getpid(), SIGABRT);
    kill(getpid(), SIGILL);
    kill(getpid(), SIGCHLD);
    kill(getpid(), SIGSEGV);
    kill(getpid(), SIGFPE);
    kill(getpid(), SIGHUP);
    kill(getpid(), SIGTSTP);

    sleep(1);
/*
    pthread_kill(tid[1], SIGSEGV);      // Line A
    pthread_kill(tid[2], SIGSEGV);      // Line C
    pthread_kill(tid[1], SIGINT);       // Line D
    pthread_kill(tid[3], SIGINT);       // Line E
*/
    for ( int i = 0; i < NUM_THREADS; i++){
        pthread_join(tid[i], NULL);
    }

    signal(SIGINT, SIG_DFL);
    signal(SIGABRT, SIG_DFL);
    signal(SIGILL, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);
    signal(SIGSEGV, SIG_DFL);
    signal(SIGFPE, SIG_DFL);
    signal(SIGHUP, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);

    return 0;
}

