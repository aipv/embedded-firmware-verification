#!/bin/bash
set -e

if [ "$#" -ne 4 ]; then
echo "Usage: $0 <input_file> <output_file> <offset> <hex_bytes>"
echo "Example:"
echo "  $0 input.bin output.bin 0x1effa0 11223344"
exit 1
fi

INPUT="$1"
OUTPUT="$2"
OFFSET="$3"
HEX="$4"

cp "$INPUT" "$OUTPUT"

# Convert hex string to binary bytes

BYTES=$(echo "$HEX" | sed 's/../\x&/g')

printf "$BYTES" | 
dd of="$OUTPUT" bs=1 seek=$((OFFSET)) conv=notrunc status=none

echo "Tampered image written to: $OUTPUT"
echo "Offset: $OFFSET"
echo "Bytes : $HEX"

