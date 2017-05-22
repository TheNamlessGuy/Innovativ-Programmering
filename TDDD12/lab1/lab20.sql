/* maxbr431, davsp799 */

DROP VIEW IF EXISTS jbsale_supply CASCADE;
SOURCE company_schema.sql;
SOURCE company_data.sql;

CREATE VIEW jbsale_supply(supplier, item, sold_amount) AS (
       SELECT supplier.name, item.name, sale.quantity
       FROM jbsupplier AS supplier
            JOIN jbitem AS item
            ON supplier.id = item.supplier
               LEFT JOIN jbsale AS sale
               ON item.id = sale.item
);

SELECT supplier, sum(sold_amount) AS sum
FROM jbsale_supply GROUP BY supplier;
