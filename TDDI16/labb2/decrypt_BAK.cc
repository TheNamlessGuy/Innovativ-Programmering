#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

#include "Key.h"

int main(int argc, char* argv[])
{
  unsigned char buffer[C+1];     // temporary string buffer
  Key candidate = {{0}};         // a password candidate
  Key encrypted;                 // the encrypted password
  Key candenc;                   // the encrypted password candidate
  Key halffullkey = {{0}};       // the all zero key
  Key T[N];                      // table T
  vector<Key> Ubrute;            // Bruteforced lower half of T

  unsigned char halfword[C];
  unsigned char halffilledword[C];
  for (int i = 0; i < C; ++i)
  {
    if (i < C/2)
    {
      halfword[i] = ALPHABET[0];
    halffilledword[i] = ALPHABET[R-1];
    }
    else
    {
      halfword[i] = ALPHABET[R-1];
      halffilledword[i] = ALPHABET[0];
    }
  }
  halffullkey = KEYinit(halfword);

  if (argc != 2)
  {
    cout << "Usage:" << endl << argv[0] << " password < rand8.txt" << endl;
    return 1;
  }

  encrypted = KEYinit((unsigned char *) argv[1]);

  // read in table T, S & U
  for (int i{0}; i < N; ++i)
  {
    scanf("%s", buffer);
    T[i] = KEYinit(buffer);
  }

  // Get all the Ubrute values
  while (candidate != halffullkey)
  {
    ++candidate;
    candenc = KEYsubsetsum(candidate, T);
    Ubrute.push_back(candenc);
    //cout << "CAN: " << candidate << " KEY: " << halffullkey << endl;
  } 

  halffullkey = KEYinit(halffilledword);
  candidate = {{0}};
  
  // Check all the other values if they match
  while (candidate != halffullkey)
  {
    // candidate gets all values from A
    // Check every bTest for every candenc

    Key bTest = encrypted - candidate;
    //cout << bTest << endl;

    candenc = KEYsubsetsum(candidate, T);

    cout << candenc << " = " << bTest << endl;

    if (candenc == bTest)
      cout << candidate << endl << candenc << endl << bTest << endl << endl;

    for (int i = 0; i < (R*R*R); ++i)
      ++candidate;
    //cout << "CAN: " << candidate << " KEY: " << halffullkey << endl;
  }

  return 0;
}
