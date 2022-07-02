select *
from hra
join dokoncena using(id_hra)
where to_char(datum_vydani, 'YYYY') = '2015'; -- nebo extract(year from datum_vydani)
