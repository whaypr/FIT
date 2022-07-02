select * from platforma where id_platforma not in ( -- platformy, které mezi nimi nejsou (žádné)
    select id_platforma from vazba_hra_platforma where id_hra in ( -- id platforem, na kterých jdou hrát hry z původního dotazu (všechny)
        -- <PUVODNI DOTAZ> --
        select id_hra from hra h where not exists ( -- id her, které jsou na všech platformách
            select * from platforma p where not exists (
                select * from vazba_hra_platforma v where p.id_platforma = v.id_platforma and h.id_hra = v.id_hra
            )
        )
        -- </PUVODNI DOTAZ> --
    )
);
