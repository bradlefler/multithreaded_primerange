//Compile it using g++ omp_prange.cpp -o omp_prange -fopenmp
#include <omp.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <time.h>

#define BILLION  1000000000.0

using namespace std;

void find_primes(long start, long end, vector<long> &primes);

int main(int argc, char *argv[])
{
  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, &start);
  if(argc != 4)
  {
    cerr<<"Bad Arguments. Usage: ./prange <min> <max> <thread_count>"<<endl;
    exit(1);
  }
  long num1 = stol(argv[1]);
  long num2 = stol(argv[2]);
  int thread_ct = stoi(argv[3]);
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
  vector<long> primes;
  long count = num2 - num1;
  long amount = count/thread_ct + 1;
  #pragma omp parallel for
  for(int i = 0; i < thread_ct; i++)
  {
    long start = num1 + (amount * i);
    long end = num1 + (amount * (i+1) - 1);
    if(end > num2)
    {
      end = num2;
    }
    find_primes(start, end, primes);
  }
  clock_gettime(CLOCK_REALTIME, &end);
  double time_spent = (end.tv_sec - start.tv_sec) +
              (end.tv_nsec - start.tv_nsec) / BILLION;
  printf("%f\n", time_spent);
  sort (primes.begin(), primes.end());
  cout<<"[";
  for(unsigned long i = 0; i < primes.size(); i++)
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

void find_primes(long start, long end, vector<long> &primes)
{
  vector<long> temp;
  for(long i = start; i < end + 1; i++)
  {
    bool prime = true;
    long max = floor(sqrt(i)) + 1;
    for(long j = 2; j < max; j++)
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
  for(unsigned int i = 0; i < temp.size(); i++)
  {
    #pragma omp critical
    primes.push_back(temp[i]);
  }
}
