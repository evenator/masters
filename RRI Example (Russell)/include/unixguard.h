
#ifndef _UNIXGUARD_H_
#define _UNIXGUARD_H_

#ifdef WIN32

//#include <winsock2.h>
//#include <windows.h>
	
typedef DWORD pthread_t;
typedef HANDLE  pthread_mutex_t;

#define pthread_create(f, a, c, d) CreateThread(a, 0, c, d, 0, f) 
#define pthread_mutex_init(a, b)   *(a) = CreateMutex(b, false, NULL)

#define pthread_mutex_destroy(a) CloseHandle(*(a))

/*BOOL  pthread_mutex_destroy(pthread_mutex_t *a){
	return CloseHandle(*a);
} */




#define pthread_mutex_lock(a) WaitForSingleObject(*(a), INFINITE)
#define pthread_mutex_unlock(a) ReleaseMutex(*(a))
#endif

#endif 
