/* QUESTION 1 */
SELECT * FROM jbemployee;

/*
+------+--------------------+--------+---------+-----------+-----------+
| id   | name               | salary | manager | birthyear | startyear |
+------+--------------------+--------+---------+-----------+-----------+
|   10 | Ross, Stanley      |  15908 |     199 |      1927 |      1945 |
|   11 | Ross, Stuart       |  12067 |    NULL |      1931 |      1932 |
|   13 | Edwards, Peter     |   9000 |     199 |      1928 |      1958 |
|   26 | Thompson, Bob      |  13000 |     199 |      1930 |      1970 |
|   32 | Smythe, Carol      |   9050 |     199 |      1929 |      1967 |
|   33 | Hayes, Evelyn      |  10100 |     199 |      1931 |      1963 |
|   35 | Evans, Michael     |   5000 |      32 |      1952 |      1974 |
|   37 | Raveen, Lemont     |  11985 |      26 |      1950 |      1974 |
|   55 | James, Mary        |  12000 |     199 |      1920 |      1969 |
|   98 | Williams, Judy     |   9000 |     199 |      1935 |      1969 |
|  129 | Thomas, Tom        |  10000 |     199 |      1941 |      1962 |
|  157 | Jones, Tim         |  12000 |     199 |      1940 |      1960 |
|  199 | Bullock, J.D.      |  27000 |    NULL |      1920 |      1920 |
|  215 | Collins, Joanne    |   7000 |      10 |      1950 |      1971 |
|  430 | Brunet, Paul C.    |  17674 |     129 |      1938 |      1959 |
|  843 | Schmidt, Herman    |  11204 |      26 |      1936 |      1956 |
|  994 | Iwano, Masahiro    |  15641 |     129 |      1944 |      1970 |
| 1110 | Smith, Paul        |   6000 |      33 |      1952 |      1973 |
| 1330 | Onstad, Richard    |   8779 |      13 |      1952 |      1971 |
| 1523 | Zugnoni, Arthur A. |  19868 |     129 |      1928 |      1949 |
| 1639 | Choy, Wanda        |  11160 |      55 |      1947 |      1970 |
| 2398 | Wallace, Maggie J. |   7880 |      26 |      1940 |      1959 |
| 4901 | Bailey, Chas M.    |   8377 |      32 |      1956 |      1975 |
| 5119 | Bono, Sonny        |  13621 |      55 |      1939 |      1963 |
| 5219 | Schwarz, Jason B.  |  13374 |      33 |      1944 |      1959 |
+------+--------------------+--------+---------+-----------+-----------+
25 rows in set (0.00 sec)
*/

/* QUESTION 2 */
SELECT name FROM jbdept ORDER BY name;

/*
+------------------+
| name             |
+------------------+
| Bargain          |
| Book             |
| Candy            |
| Children's       |
| Children's       |
| Furniture        |
| Giftwrap         |
| Jewelry          |
| Junior Miss      |
| Junior's         |
| Linens           |
| Major Appliances |
| Men's            |
| Sportswear       |
| Stationary       |
| Toys             |
| Women's          |
| Women's          |
| Women's          |
+------------------+
19 rows in set (0.00 sec)
*/

/* QUESTION 3 */
SELECT * FROM jbparts WHERE qoh = 0;

/*
+----+-------------------+-------+--------+------+
| id | name              | color | weight | qoh  |
+----+-------------------+-------+--------+------+
| 11 | card reader       | gray  |    327 |    0 |
| 12 | card punch        | gray  |    427 |    0 |
| 13 | paper tape reader | black |    107 |    0 |
| 14 | paper tape punch  | black |    147 |    0 |
+----+-------------------+-------+--------+------+
4 rows in set (0.00 sec)
*/

/* QUESTION 4 */
SELECT * FROM jbemployee where salary >= 9000 and salary <= 10000;

/*
+-----+----------------+--------+---------+-----------+-----------+
| id  | name           | salary | manager | birthyear | startyear |
+-----+----------------+--------+---------+-----------+-----------+
|  13 | Edwards, Peter |   9000 |     199 |      1928 |      1958 |
|  32 | Smythe, Carol  |   9050 |     199 |      1929 |      1967 |
|  98 | Williams, Judy |   9000 |     199 |      1935 |      1969 |
| 129 | Thomas, Tom    |  10000 |     199 |      1941 |      1962 |
+-----+----------------+--------+---------+-----------+-----------+
4 rows in set (0.00 sec)
*/

/* QUESTION 5 */
SELECT name, (startyear - birthyear) as age FROM jbemployee ORDER BY age;

/*
+--------------------+------+
| name               | age  |
+--------------------+------+
| Bullock, J.D.      |    0 |
| Ross, Stuart       |    1 |
| Schwarz, Jason B.  |   15 |
| Ross, Stanley      |   18 |
| Bailey, Chas M.    |   19 |
| Wallace, Maggie J. |   19 |
| Onstad, Richard    |   19 |
| Jones, Tim         |   20 |
| Schmidt, Herman    |   20 |
| Brunet, Paul C.    |   21 |
| Smith, Paul        |   21 |
| Zugnoni, Arthur A. |   21 |
| Collins, Joanne    |   21 |
| Thomas, Tom        |   21 |
| Evans, Michael     |   22 |
| Choy, Wanda        |   23 |
| Bono, Sonny        |   24 |
| Raveen, Lemont     |   24 |
| Iwano, Masahiro    |   26 |
| Edwards, Peter     |   30 |
| Hayes, Evelyn      |   32 |
| Williams, Judy     |   34 |
| Smythe, Carol      |   38 |
| Thompson, Bob      |   40 |
| James, Mary        |   49 |
+--------------------+------+
25 rows in set (0.01 sec)
*/

/* QUESTION 6 */
SELECT name FROM jbemployee WHERE substring(name,1,instr(name, " ") -2) LIKE '%son';

/*
+---------------+
| name          |
+---------------+
| Thompson, Bob |
+---------------+
1 row in set (0.00 sec)
*/

/* QUESTION 7 */
SELECT * FROM jbitem WHERE supplier in (SELECT id FROM jbsupplier WHERE name = 'Fisher-Price');

/*
+-----+-----------------+------+-------+------+----------+
| id  | name            | dept | price | qoh  | supplier |
+-----+-----------------+------+-------+------+----------+
|  43 | Maze            |   49 |   325 |  200 |       89 |
| 107 | The 'Feel' Book |   35 |   225 |  225 |       89 |
| 119 | Squeeze Ball    |   49 |   250 |  400 |       89 |
+-----+-----------------+------+-------+------+----------+
3 rows in set (0.00 sec)
*/

/* QUESTION 8 */
SELECT I.* FROM jbitem I, jbsupplier S WHERE I.supplier = S.id and S.name = 'Fisher-Price';

/*
+-----+-----------------+------+-------+------+----------+
| id  | name            | dept | price | qoh  | supplier |
+-----+-----------------+------+-------+------+----------+
|  43 | Maze            |   49 |   325 |  200 |       89 |
| 107 | The 'Feel' Book |   35 |   225 |  225 |       89 |
| 119 | Squeeze Ball    |   49 |   250 |  400 |       89 |
+-----+-----------------+------+-------+------+----------+
3 rows in set (0.00 sec)
*/

/* QUESTION 9 */
SELECT name FROM jbcity WHERE id IN (SELECT city FROM jbsupplier);

/*
+----------------+
| name           |
+----------------+
| Amherst        |
| Boston         |
| New York       |
| White Plains   |
| Hickville      |
| Atlanta        |
| Madison        |
| Paxton         |
| Dallas         |
| Denver         |
| Salt Lake City |
| Los Angeles    |
| San Diego      |
| San Francisco  |
| Seattle        |
+----------------+
15 rows in set (0.00 sec)
*/

/* QUESTION 10 */
SELECT name, color FROM jbparts WHERE weight > (SELECT weight FROM jbparts WHERE name = 'card reader');

/*
+--------------+--------+
| name         | color  |
+--------------+--------+
| disk drive   | black  |
| tape drive   | black  |
| line printer | yellow |
| card punch   | gray   |
+--------------+--------+
4 rows in set (0.00 sec)
*/

/* QUESTION 11 */
SELECT P.name, P.color FROM jbparts P, jbparts Q WHERE P.weight > Q.weight AND Q.name = 'card reader';

/*
+--------------+--------+
| name         | color  |
+--------------+--------+
| disk drive   | black  |
| tape drive   | black  |
| line printer | yellow |
| card punch   | gray   |
+--------------+--------+
4 rows in set (0.00 sec)
*/

/* QUESTION 12 */
SELECT AVG(weight) AS 'Average Weight' FROM jbparts WHERE color = 'black';

/*
+----------------+
| Average Weight |
+----------------+
|       347.2500 |
+----------------+
1 row in set (0.00 sec)
*/

/* QUESTION 13 */
SELECT S.name, SUM(P.weight * Z.quan) as 'Total Weight' FROM jbsupplier S, jbparts P, jbsupply Z WHERE S.city in (SELECT id FROM jbcity WHERE state = 'Mass');

/* QUESTION 14 */

/* QUESTION 15 */

/* QUESTION 16 */

/* QUESTION 17 */

/* QUESTION 18 */

/* QUESTION 19 */

/* QUESTION 20 */
