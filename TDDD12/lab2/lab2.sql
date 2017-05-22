/* maxbr431, davsp799 */

/* QUESTION 3 - Implement questions 1 & 2*/

CREATE TABLE jbmanagers
(
   id int PRIMARY KEY AUTO_INCREMENT,
   bonus int,
   FOREIGN KEY (id) REFERENCES jbemployee(id)
);

INSERT IGNORE INTO jbmanagers (id, bonus)
(
   SELECT e1.id, 0
   FROM jbemployee AS e1, jbemployee AS e2
   WHERE e1.id = e2.manager AND
         e1.id NOT IN (SELECT id
                       FROM jbmanagers)
);

INSERT IGNORE INTO jbmanagers (id, bonus)
(
   SELECT d.manager, 0
   FROM jbdept AS d, jbemployee as e
   WHERE d.manager = e.id AND
         e.id NOT IN (SELECT id
                       FROM jbmanagers)
);

ALTER TABLE jbdept
DROP FOREIGN KEY fk_dept_mgr;
ALTER TABLE jbdept
ADD FOREIGN KEY (manager) REFERENCES jbmanagers(id);

ALTER TABLE jbemployee
DROP FOREIGN KEY fk_emp_mgr;
ALTER TABLE jbemployee
ADD FOREIGN KEY (manager) REFERENCES jbmanagers(id);

/*
 You do not have to set bonus (since it's not NOT NULL),
 but you should to ease further use
*/

/* QUESTION 4 - Give all department managers 10000 in bonus*/

UPDATE jbmanagers
   SET bonus=bonus + 10000
   WHERE jbmanagers.id IN (SELECT manager
                           FROM jbdept);

/* QUESTION 5 b - Implement 5a */

CREATE TABLE jbcustomer
(
   id int PRIMARY KEY AUTO_INCREMENT,
   city_id int,
   name varchar(40),
   addr varchar(40),
   FOREIGN KEY (city_id) REFERENCES jbcity(id)
);

CREATE TABLE jbaccount
(
   id int PRIMARY KEY AUTO_INCREMENT,
   balance int,
   customer_id int,
   FOREIGN KEY (customer_id) REFERENCES jbcustomer(id)
);

CREATE TABLE jbtransaction
(
   id int PRIMARY KEY AUTO_INCREMENT,
   sdate Date,
   employee_id int,
   account_id int,
   amount int,
   FOREIGN KEY (employee_id) REFERENCES jbemployee(id),
   FOREIGN KEY (account_id) REFERENCES jbaccount(id)
);

CREATE TABLE jbmoneyio
(
   id int PRIMARY KEY,
   isWithdrawal boolean NOT NULL, /* false => deposit, true => withdrawal */
   FOREIGN KEY (id) REFERENCES jbtransaction(id)
);

/*
CREATE TABLE jbdeposit
(
   id int PRIMARY KEY,
   FOREIGN KEY (id) REFERENCES jbtransaction(id)
);

CREATE TABLE jbwithdrawal
(
   id int PRIMARY KEY,
   FOREIGN KEY (id) REFERENCES jbtransaction(id)
);*/

INSERT INTO jbcustomer (city_id)
(
   SELECT id FROM jbcity WHERE name = 'Hickville'
);

INSERT IGNORE INTO jbaccount (id, customer_id)
(
   SELECT deb.account, 1
   FROM jbdebit AS deb
);

INSERT IGNORE INTO jbtransaction (id, sdate, employee_id, account_id)
(
   SELECT id, sdate, employee, account FROM jbdebit
);

ALTER TABLE jbdebit
DROP COLUMN sdate;
ALTER TABLE jbdebit
DROP FOREIGN KEY fk_debit_employee;
ALTER TABLE jbdebit
DROP COLUMN employee;
ALTER TABLE jbdebit
DROP COLUMN account;
