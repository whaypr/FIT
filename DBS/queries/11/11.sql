select * from hra 
where id_hra in (
    (
        select id_hra from oceneni where nazev = 'Game of the Year'
        union
        select id_hra from oceneni where nazev = 'Best Art Direction'
    )
    except
    (
        select id_hra from oceneni where nazev = 'Game of the Year'
        intersect
        select id_hra from oceneni where nazev = 'Best Art Direction'
    )
);
