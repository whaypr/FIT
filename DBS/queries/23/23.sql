begin;

update hra set id_hra_stav = 2 where id_hra = ( select id_hra from ve_vyvoji );

insert into dokoncena
select  id_hra,
        CURRENT_DATE as datum_vydani,
        round(random() * 10000) as naklady,
        null as vydelek
from ve_vyvoji;

delete from ve_vyvoji;

-- KONTROLA
select 'Ve vývoji není žádná hra' as kontrola where not exists ( select * from ve_vyvoji );
select * from hra join dokoncena using(id_hra) where nazev = 'Half-Life 3';

rollback;
