## Jak spustit

**make && ./exec {1} {2} {3}**

### {1}
- cesta k souboru, se kterým se pracuje
- vždy pouze původní soubor

### {2}
- **e** pro šifrování
- **d** pro dešifrování

### {3}
- **ecb** pro operační mód ecb
- **cbc** pro operační mód cbc

Příklad: **make && ./exec images/homer-simpson.tga d cbc**

---

Pro zašifrování všech ukázkových souborů pomocí obou módů lze zavolat:
**./ecrypt_all.sh**

A pro dešifrování:
**./decrypt_all.sh**