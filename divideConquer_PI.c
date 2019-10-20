#include<omp.h>
#include<stdio.h>

#define MIN_BLK 10000000

double pi_comp(int Nstart,int Nfinish, double step)
{
        int i, iblk;
        double x, sum = 0.0,sum1, sum2;
        if (Nfinish-Nstart< MIN_BLK)
        {
                for (i=Nstart;i< Nfinish; i++)
                {
                        x = (i+0.5)*step;
                        sum = sum + 4.0/(1.0+x*x);
                }
        }
  else
  {
                iblk = Nfinish-Nstart;

                #pragma omp task shared(sum1)
                sum1 = pi_comp(Nstart,Nfinish-iblk/2,step);

                #pragma omp task shared(sum2)
                sum2 = pi_comp(Nfinish-iblk/2, Nfinish,step);

                #pragma omp taskwait
                sum = sum1 + sum2;
        }
  return sum;
}

int main ()
{
        int i,j,x;
        double avg,step, pi, sum;
        int iterations[]=  {24000000, 48000000, 96000000};
        int threads[] = {1, 6, 12, 24, 48};
        for(i=0;i<3;i++){
          step  = 1.0/(double) iterations[i];
         for(x=0;x<5;x++){
          for(j=0;j<5;j++){
                double start_time = omp_get_wtime();
                #pragma omp parallel num_threads(threads[x])
                {
                 #pragma omp single
                 sum = pi_comp(0,iterations[i],step);
                }
                pi = step * sum;
                double end_time =  omp_get_wtime() - start_time;
                  avg += end_time;
          }
          printf(" threads[x] = %d \n", threads[x]);
          printf(" average end time = %.3lf \n", avg/5);
          avg=0;
         }
         printf(" iterations[i] = %d \n", iterations[i]);
        }
}

