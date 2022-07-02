-- ZPŮSOB 1
-- Hry, pro které neexistuje platforma, na které by nešly hrát.
select * from hra h where not exists (
    select * from platforma p where not exists (
        select * from vazba_hra_platforma v where p.id_platforma = v.id_platforma and h.id_hra = v.id_hra
    )
);

/*
-- ZPŮSOB 2
select * from hra h where
    ( select count(*) from platforma )
=
    ( select count(distinct id_platforma) from vazba_hra_platforma v where v.id_hra = h.id_hra );
*/
