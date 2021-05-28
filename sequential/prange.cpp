#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <time.h>

#define BILLION  1000000000.0

using namespace std;

int main(int argc, char *argv[])
{
  struct timespec start, end;
  clock_gettime(CLOCK_REALTIME, &start);
  if(argc != 3)
  {
    cerr<<"Bad Arguments. Usage: ./prange <int> <int>"<<endl;
    exit(1);
  }
  long num1 = stol(argv[1]);
  long num2 = stol(argv[2]);
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
  for(long i = num1; i < num2 + 1; i++)
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
      primes.push_back(i);
    }
  }
  clock_gettime(CLOCK_REALTIME, &end);
  double time_spent = (end.tv_sec - start.tv_sec) +
              (end.tv_nsec - start.tv_nsec) / BILLION;
  printf("%f seconds to run. \n", time_spent);
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
