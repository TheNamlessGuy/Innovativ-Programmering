/**********************************************************************
 *  Knäcka lösenord readme.txt
 **********************************************************************/

 Ungefärligt antal timmar spenderade på labben (valfritt): För många

/**********************************************************************
 *  Ge en högnivåbeskrivning av ditt program decrypt.c.
 **********************************************************************/

1: Programmet bruteforcar ena halvan av möjliga lösenord för den givna tabellen, och lagrar varje resultat i ett
binärt sökträd (std::map). Nyckeln i trädet är det krypterade lösenordet minus det bruteforcade värdet.
2: Sedan bruteforcar man andra halvan av möjliga lösenord för den givna tabellen. Om det krypterade lösenordet minus
det nya bruteforcade värdet finns som nyckel i sökträdet, så har vi lyckats avkryptera lösenordet.

/**********************************************************************
 *  Beskriv symboltabellen du använt för decrypt.c.
 **********************************************************************/

Nyckel: Det krypterade lösenordet minus en delmängd av tabellen.
Värde: De lösenord (det kan finnas mer än ett lösenord) som gav denna delmängd.

/**********************************************************************
 *  Ge de dekrypterade versionerna av alla lösenord med 8 och 10
 *  bokstäver i uppgiften du lyckades knäca med DIN kod.
 **********************************************************************/


8 bokstäver                    10 bokstäver
-----------                    ------------
xwtyjjin = congrats            h554tkdzti = completely
rpb4dnye = youfound            oykcetketn = unbreakabl
kdidqv4i = theright            bkzlquxfnt = cryptogram
m5wrkdge = solution            wixxliygk1 = ormaybenot

/****************************************************************************
 *  Hur lång tid använder brute.c för att knäcka lösenord av en viss storlek?
 *  Ge en uppskattning markerad med en asterisk om det tar längre tid än vad
 *  du orkar vänta. Ge en kort motivering för dina uppskattningar.
 ***************************************************************************/


Char     Brute     
--------------
 4       0 sekunder
 5       28 sekunder
 6       1097 sekunder
 8       1203409 sekunder*

Antagningarna är uttagna pga. att brute.cc har en tillväxthastighet av O(2^n).

/******************************************************************************
 *  Hur lång tid använder decrypt.c för att knäcka lösenord av en viss storlek?
 *  Hur mycket minne använder programmet?
 *  Ge en uppskattning markerad med en asterisk om det tar längre tid än vad
 *  du orkar vänta. Ge en kort motivering för dina uppskattningar.
 ******************************************************************************/

Char    Tid (sekunder)    Minne (bytes) (valgrind bytes allocated)
----------------------------------------
6       0                 2,293,690
8       4                 75,497,400                  
10      231               2,751,463,342
12      13340*            för mycket                  

/*************************************************************************
 * Hur många operationer använder brute.c för ett N-bitars lösenord?
 * Hur många operationer använder din decrypt.c för ett N-bitars lösenord?
 * Använd ordo-notation.
 *************************************************************************/

Brute: O(2^n)
Decrypt: O(2^(n/2))
