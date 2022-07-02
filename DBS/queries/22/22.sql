where (id_hra, id_platforma) not in ( select id_hra, id_platforma from vazba_hra_platforma );

select * from chybejici_platformy;

-- VLOŽENÍ
insert into vazba_hra_platforma (id_hra, id_platforma)
select id_hra, id_platforma
from hra
cross join platforma
where (id_hra, id_platforma) not in ( select id_hra, id_platforma from vazba_hra_platforma );

-- KONTROLA
select * from chybejici_platformy;

rollback;
