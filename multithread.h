#ifndef __MULTITHREAD_H_INCLUDED__
#define __MULTITHREAD_H_INCLUDED__

#include <pthread.h>

class thread {
public:
    /****************************
    // Constructer/Destructor   */
    thread() {}
    ~thread() {}

    /****************************
    // Modifiers                */
    void start() {
        assert(m_running == false);
        pthread_create(&_thread,NULL,&thread::start_t,this); }
    void stop() {
        (void) pthread_join(&_thread,NULL); }

protected:
    /****************************
    // Class Extension          */
    virtual void run_thread() = 0;

private:
    pthread_t _thread;
    static void * start_t(void * this) { 
        ((thread*)this)->run_thread(); return NULL; }

};

#endif
