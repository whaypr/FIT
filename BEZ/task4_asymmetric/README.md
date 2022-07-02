## Struktura zašifrovaného souboru

| **Pozice**                            | **Délka [B]**               | **Struktura** | **Popis**                          |
|---------------------------------------|-----------------------------|---------------|------------------------------------|
| 0                                     | sizeof(int)                 | int           | NID použité symetrické šifry       |
| sizeof(int)                           | sizeof(int)                 | int           | keyLen - délka zašifrovaného klíče |
| 2 * sizeof(int)                       | keyLen                      | binární data  | zašifrovaný klíč                   |
| 2 * sizeof(int) + keyLen              | block_size                  | binární data  | inicializační vektor               |
| 2 * sizeof(int) + keyLen + block_size | délka(in_soubor + padding)  | binární data  | zašifrovaný soubor                 |