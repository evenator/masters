#ifndef _WINGUARD_H_
#define _WINGUARD_H_

#ifndef WIN32
#define DWORD int
#define __stdcall

#define LPVOID void*
#define LIBRARY_API


#define CreateThread(a, b, c, d, e, f) \
	pthread_create(f, a, c, static_cast<void*>(d))

#define HANDLE pthread_mutex_t
#define WaitForSingleObject(a, b) pthread_mutex_lock(&a)
#define ReleaseMutex(a) pthread_mutex_unlock(&a)

int __nsleep(const struct timespec *req, struct timespec *rem);

int Sleep(unsigned long milisec);

#endif /* WIN32 */
#endif /* _WINGUARD_H_ */
