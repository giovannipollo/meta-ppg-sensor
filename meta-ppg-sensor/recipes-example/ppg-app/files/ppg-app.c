#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>


#define q 11       /* for 2^11 points */
#define N (1 << q) /* N-point FFT, iFFT */

typedef float real;

typedef struct {
    real Re;
    real Im;
} complex;

#ifndef PI
#define PI 3.14159265358979323846264338327950288
#endif




complex v[N], v2[N], scratch[N];
int i, k, m, minIdx, maxIdx;
sem_t sem_read_data, sem_compute;
int fd = -1;





void fft(complex *v, int n, complex *tmp);
void *read_data();
void *compute();


int main(int argc, char **argv) {



	char *app_name = argv[0];
	char *dev_name = "/dev/mymod_dev";
    	int ret_value1;
    	pthread_t thread[2];
    	sem_init(&sem_read_data, 0, 1);
    	sem_init(&sem_compute, 0, 0);

	if ((fd = open(dev_name, O_RDWR)) < 0)
	{
		fprintf(stderr, "%s: unable to open %s: %s\n", app_name, dev_name, strerror(errno));
		return 1;
	}
    	
	ret_value1 = pthread_create(&thread[0], NULL, read_data, (void *)NULL);
    	if (ret_value1 != 0) {
        	printf("Error. Thread can't be created\n");
		return 1;
   	}
    	ret_value1 = pthread_create(&thread[1], NULL, compute, (void *)NULL);
    	if (ret_value1 != 0) {
        	printf("Error. Thread can't be created\n");
		return 1;
   	}

    	pthread_join(thread[0], NULL);
    	pthread_join(thread[1], NULL);

    	sem_destroy(&sem_read_data);
    	sem_destroy(&sem_compute);
    	exit(EXIT_SUCCESS);




}


void *read_data() {

    	struct timespec tim, tim2;
    	tim.tv_sec = 0;
    	tim.tv_nsec = 20000000;
	int valori[2048];
	int to_be_read[1];
	

    while (1) {
        sem_wait(&sem_read_data);
        printf("\nRead Data has started\n");
        // Initialize the complex array for FFT computation
        for (int i = 0; i < N; i++) {
		read(fd, to_be_read, sizeof(to_be_read));
		valori[i] = to_be_read[0];
		printf("valori[%d]: %d\n", i, valori[i]);
		v[i].Re = to_be_read[0];
		v[i].Im = 0;
            	while (nanosleep(&tim, &tim2) < 0);
        }

        printf("\nRead Data has finished\n");

        sem_post(&sem_compute);
        sem_post(&sem_read_data);
    }
    return NULL;
}

void *compute() {
    float abs[N];

    while (1) {
        sem_wait(&sem_compute);
        printf("\nCompute has started\n");

        /* Usato così da evitare conflitti nel caso di lettura e computazione in parallelo */
        for (k = 0; k < N; k++) {
            v2[k].Re = v[k].Re;
            v2[k].Im = v[k].Im;
        }

        fft(v2, N, scratch);
        // PSD computation
        
        for (k = 0; k < N; k++) {
            abs[k] = (50.0 / 2048) * ((v2[k].Re * v2[k].Re) + (v2[k].Im * v2[k].Im));
        }
        minIdx = (0.5 * 2048) / 50; // position in the PSD of the spectral line corresponding to 30 bpm
        maxIdx = 3 * 2048 / 50;     // position in the PSD of the spectral line corresponding to 180 bpm

        // Find the peak in the PSD from 30 bpm to 180 bpm
        m = minIdx;
        for (k = minIdx; k < (maxIdx); k++) {
            if (abs[k] > abs[m])
                m = k;
        }
        printf("\nCompute has finished\n");
        printf("\n\n%d bpm\n\n", (m)*60 * 50 / 2048);
    }

    return NULL;
}

void fft(complex *v, int n, complex *tmp) {
    if (n > 1) { /* otherwise, do nothing and return */
        int k, m;
        complex z, w, *vo, *ve;
        ve = tmp;
        vo = tmp + n / 2;
        for (k = 0; k < n / 2; k++) {
            ve[k] = v[2 * k];
            vo[k] = v[2 * k + 1];
        }
        fft(ve, n / 2, v); /* FFT on even-indexed elements of v[] */
        fft(vo, n / 2, v); /* FFT on odd-indexed elements of v[] */
        for (m = 0; m < n / 2; m++) {
            w.Re = cos(2 * PI * m / (double)n);
            w.Im = -sin(2 * PI * m / (double)n);
            z.Re = w.Re * vo[m].Re - w.Im * vo[m].Im; /* Re(w*vo[m]) */
            z.Im = w.Re * vo[m].Im + w.Im * vo[m].Re; /* Im(w*vo[m]) */
            v[m].Re = ve[m].Re + z.Re;
            v[m].Im = ve[m].Im + z.Im;
            v[m + n / 2].Re = ve[m].Re - z.Re;
            v[m + n / 2].Im = ve[m].Im - z.Im;
        }
    }
    return;
}

