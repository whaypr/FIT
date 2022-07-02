begin;

create or replace view malo_financovane_hry as
select * from hra
where id_hra in (
    select id_hra from vazba_hra_platforma
    group by id_hra
    having count(*) > 3
) and rozpocet < 50000
with check option;

select * from malo_financovane_hry;

update malo_financovane_hry set rozpocet = rozpocet + 15000;

-- KONTROLA
select * from malo_financovane_hry;

rollback;
