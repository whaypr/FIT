select count(*) as celkem, ( select count(*) from vazba_hra_platforma ) as skutecne
from hra
cross join platforma
