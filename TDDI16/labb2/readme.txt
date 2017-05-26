/**********************************************************************
 *  Kn�cka l�senord readme.txt
 **********************************************************************/

 Ungef�rligt antal timmar spenderade p� labben (valfritt): F�r m�nga

/**********************************************************************
 *  Ge en h�gniv�beskrivning av ditt program decrypt.c.
 **********************************************************************/

1: Programmet bruteforcar ena halvan av m�jliga l�senord f�r den givna tabellen, och lagrar varje resultat i ett
bin�rt s�ktr�d (std::map). Nyckeln i tr�det �r det krypterade l�senordet minus det bruteforcade v�rdet.
2: Sedan bruteforcar man andra halvan av m�jliga l�senord f�r den givna tabellen. Om det krypterade l�senordet minus
det nya bruteforcade v�rdet finns som nyckel i s�ktr�det, s� har vi lyckats avkryptera l�senordet.

/**********************************************************************
 *  Beskriv symboltabellen du anv�nt f�r decrypt.c.
 **********************************************************************/

Nyckel: Det krypterade l�senordet minus en delm�ngd av tabellen.
V�rde: De l�senord (det kan finnas mer �n ett l�senord) som gav denna delm�ngd.

/**********************************************************************
 *  Ge de dekrypterade versionerna av alla l�senord med 8 och 10
 *  bokst�ver i uppgiften du lyckades kn�ca med DIN kod.
 **********************************************************************/


8 bokst�ver                    10 bokst�ver
-----------                    ------------
xwtyjjin = congrats            h554tkdzti = completely
rpb4dnye = youfound            oykcetketn = unbreakabl
kdidqv4i = theright            bkzlquxfnt = cryptogram
m5wrkdge = solution            wixxliygk1 = ormaybenot

/****************************************************************************
 *  Hur l�ng tid anv�nder brute.c f�r att kn�cka l�senord av en viss storlek?
 *  Ge en uppskattning markerad med en asterisk om det tar l�ngre tid �n vad
 *  du orkar v�nta. Ge en kort motivering f�r dina uppskattningar.
 ***************************************************************************/


Char     Brute     
--------------
 4       0 sekunder
 5       28 sekunder
 6       1097 sekunder
 8       1203409 sekunder*

Antagningarna �r uttagna pga. att brute.cc har en tillv�xthastighet av O(2^n).

/******************************************************************************
 *  Hur l�ng tid anv�nder decrypt.c f�r att kn�cka l�senord av en viss storlek?
 *  Hur mycket minne anv�nder programmet?
 *  Ge en uppskattning markerad med en asterisk om det tar l�ngre tid �n vad
 *  du orkar v�nta. Ge en kort motivering f�r dina uppskattningar.
 ******************************************************************************/

Char    Tid (sekunder)    Minne (bytes) (valgrind bytes allocated)
----------------------------------------
6       0                 2,293,690
8       4                 75,497,400                  
10      231               2,751,463,342
12      13340*            f�r mycket                  

/*************************************************************************
 * Hur m�nga operationer anv�nder brute.c f�r ett N-bitars l�senord?
 * Hur m�nga operationer anv�nder din decrypt.c f�r ett N-bitars l�senord?
 * Anv�nd ordo-notation.
 *************************************************************************/

Brute: O(2^n)
Decrypt: O(2^(n/2))
