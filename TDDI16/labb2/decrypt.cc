#include <iostream>
#include <chrono>
#include <map>
#include <vector>

#include "Key.h"

#define keymap map<Key, vector<Key>>

void decrypt(Key encrypted)
{
  unsigned char buffer[C+1];     // temporary string buffer
  Key candidate = {{0}};         // a password candidate
  Key candenc;                   // the encrypted password candidate
  Key halffullkey = {{0}};       // hurr durr
  Key zero = {{0}};              // the all zero key
  Key T[N];                      // table T
  keymap bBrute;                 // Bruteforced lower half of T
  unsigned char halfword[C];
  unsigned char incrementString[C];

  int breakPoint;
  if (C % 2 == 0) {
    breakPoint = C/2;
  } else {
    breakPoint = (C+1)/2;
  }

  for (int i = 0; i < C; ++i)
  {
    incrementString[i] = ALPHABET[0];
    if (i == (breakPoint) - 1)
    {
      incrementString[i] = ALPHABET[1];
    }
    if (i < breakPoint)
    {
      halfword[i] = ALPHABET[0];
    }
    else
    {
      halfword[i] = ALPHABET[R-1];
    }
  }
  halffullkey = KEYinit(halfword);
  Key increment = KEYinit(incrementString);

  for (int i{0}; i < N; ++i)
  {
    scanf("%s", buffer);
    T[i] = KEYinit(buffer);
  }

  // Get all the Ubrute values
  do
  {
    candenc = KEYsubsetsum(candidate, T);
    bBrute[encrypted - candenc].push_back(candidate);
    ++candidate;
  } while (candidate != halffullkey);

  candenc = KEYsubsetsum(candidate, T);
  bBrute[encrypted - candenc].push_back(candidate);

  candidate = {{0}};
  do
  {
    candenc = KEYsubsetsum(candidate, T);
    if (bBrute.find(candenc) != bBrute.end())
    {
      for (Key& k : bBrute[candenc])
      {
        cout << k + candidate << endl;
      }
    }
    candidate = candidate + increment;
  } while (candidate != zero);
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    cout << "Usage:" << endl << argv[0] << " password < rand8.txt" << endl;
    return 1;
  }

  auto begin = chrono::high_resolution_clock::now();

  decrypt(KEYinit((unsigned char*) argv[1]));

  auto end = chrono::high_resolution_clock::now();
  cout << "Decryption took: " <<
    chrono::duration_cast<chrono::seconds>(end - begin).count() << 
    " seconds" << endl;
  
  return 0;
}
