#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <time.h>
#define BILLION  1000000000.0

using namespace std;

vector<long> primes;

int thread_ct;
long num1;
long num2;
pthread_mutex_t lock;

void* find_primes(void *thread_id);

int main(int argc, char *argv[])
{
  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, &start);
  pthread_mutex_init(&lock,NULL);

  if(argc != 4)
  {
    cerr<<"Bad Arguments."<<endl;
    exit(1);
  }
  num1 = stol(argv[1]);
  num2 = stol(argv[2]);
  thread_ct = stoi(argv[3]);
  pthread_t th[thread_ct];
  if(num1 > num2)
  {
    long temp = num1;
    num1 = num2;
    num2 = temp;
  }
  if(num1 < 2)
  {
    num1 = 2;
  }
  for(long i = 0; i < thread_ct; i++)
  {
    pthread_create(&th[i], NULL, find_primes, (void*)i);
  }

  for(long i = 0; i < thread_ct; i++)
  {
    pthread_join(th[i], NULL);
  }
  clock_gettime(CLOCK_REALTIME, &end);
  double time_spent = (end.tv_sec - start.tv_sec) +
              (end.tv_nsec - start.tv_nsec) / BILLION;
  printf("%f seconds \n", time_spent);
  sort (primes.begin(), primes.end());
  cout<<"[";
  for(unsigned int i = 0; i < primes.size(); i++)
  {
    cout<<primes[i];
    if(i != primes.size()-1)
    {
      cout<<", ";
    }
  }
  cout<<"]"<<endl;
  return 0;
}

void* find_primes(void *thread_id)
{
  long tid;
  tid = (long)thread_id;
  long count = num2 - num1;
  long amount = count/thread_ct + 1;
  long start = num1 + (amount * tid);
  long end = num1 + (amount * (tid+1) - 1);
  vector<long> temp;
  if(end > num2)
  {
    end = num2;
  }
  for(int i = start; i < end + 1; i++)
  {
    bool prime = true;
    int max = floor(sqrt(i)) + 1;
    for(int j = 2; j < max; j++)
    {
      if(i%j == 0)
      {
        prime = false;
        break;
      }
    }
    if(prime)
    {
      temp.push_back(i);
    }
  }
  pthread_mutex_lock(&lock);
  for(unsigned int i = 0; i < temp.size(); i++)
  {
    primes.push_back(temp[i]);
  }
  pthread_mutex_unlock(&lock);
  pthread_exit(NULL);
}
