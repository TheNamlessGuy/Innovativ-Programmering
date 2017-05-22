SELECT
   supplier.name AS 'Supplier Name',
   SUM(parts.weight * supply.quan) AS 'Total Weight'
FROM
   jbsupply AS supply
      JOIN
         jbparts AS parts
      ON
         supply.part = parts.id
            JOIN
               jbsupplier AS supplier
            ON
               supply.supplier = supplier.id
                  JOIN
                     jbcity AS city
                  ON
                     supplier.city = city.id
                     AND
                     city.state = 'Mass'
GROUP BY
   supplier.name
;
