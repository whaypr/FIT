select v.jmeno as vydavatel, d.jmeno as distributor
from vydavatel v
full join distributor d on v.id_distributor = d.id_distributor
