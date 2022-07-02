-- MOŽNOST 1 (druhá v dotazu D4)

select * from hra where id_hra in (
    select id_hra from vazba_hra_platforma
    join platforma using(id_platforma)
    where nazev = 'PlayStation 4'
        except
    select id_hra from vazba_hra_platforma
    join platforma using(id_platforma)
    where nazev != 'PlayStation 4'
);
