/**********************************************************************
 *  Mönsterigenkänning readme.txt
 **********************************************************************/

 Ungefärligt antal timmar spenderade på labben (valfritt):

/**********************************************************************
 *  Empirisk    Fyll i tabellen nedan med riktiga körtider i sekunder
 *  analys      när det känns vettigt att vänta på hela beräkningen.
 *              Ge uppskattningar av körtiden i övriga fall.
 *
 **********************************************************************/
    
      N       brute(ms)   sortering(ms)   sortering med optimiseringsflaggor(ms)
 -------------------------------------------------------------------------------
    150       42          8               1
    200       99          18              3
    300       315         32              5
    400       736         55              15
    800       5808        178             33
   1600       46594       752             106
   3200       374632      3118            408
   6400       2997056*    13353           1763
  12800       23976448*   55812           7481


/**********************************************************************
 *  Teoretisk   Ge ordo-uttryck för värstafallstiden för programmen som
 *  analys      en funktion av N. Ge en kort motivering.
 *
 **********************************************************************/

Brute: O(n⁴)

för att loop i loop i loop i loop = O(n⁴)

Sortering: O(n² * log n)

Loop 1: O(n)
Loop 2: O(n)
Sortering: O(n log n)
Loop 3: O(n)

O(n) * (O(n) + O(n log n) + O(n)) = O(n² * log n)
