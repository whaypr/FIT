-- ZPŮSOB 1 --
select * from veletrh
where not exists (
    select *
    from vazba_hra_veletrh
    where vazba_hra_veletrh.id_veletrh = veletrh.id_veletrh
);

-- ZPŮSOB 2 --
select * from veletrh
except
select v.* from veletrh v join vazba_hra_veletrh vazba on v.id_veletrh = vazba.id_veletrh;

-- ZPŮSOB 3 --
select * from veletrh
where id_veletrh not in (
    select id_veletrh from vazba_hra_veletrh
);

-- ZPŮSOB 4 --
select * from veletrh
left join vazba_hra_veletrh using(id_veletrh)
where id_hra is null;
