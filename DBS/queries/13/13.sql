select * from vydavatel v where exists (
    select * from hra h
    join zanr z using(id_zanr)
    where h.id_vydavatel = v.id_vydavatel and z.nazev in ('RPG', 'Indie', 'Strategická', 'Hororová')
);
