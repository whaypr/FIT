#! /bin/bash

OUT_SEAL="$1"
OUT_OPEN="$2"
FILE="$3"

ERRORS=0

for len in 2048 3000; do
    for type in aes-{128,192,256}-{cbc,ecb}; do
        ./"${OUT_SEAL}" keys/pubkey_"$len".pem files/testfile "$type"
        ./"${OUT_OPEN}" keys/privkey_"$len".pem files/testfile_sealed
        diff files/"${FILE}" files/"${FILE}"_sealed_opened || ERRORS=$((ERRORS+1))
    done
done

if [ "$ERRORS" = 0 ]; then
    echo "ALL TESTS PASSED"
else
    echo "ERROR IN $ERRORS TESTS"
fi
