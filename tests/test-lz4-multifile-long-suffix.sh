#!/bin/sh
set -e
LZ4=${1:-../programs/lz4}
TMPDIR=${TMPDIR:-/tmp}
IN="$TMPDIR/lz4_mfsuf_in.$$"
# 64-char custom suffix via LZ4IO is not exposed on CLI (CLI uses .lz4).
# This script documents CLI sanity; the C harness covers the API overflow.
printf 'hello' > "$IN"
"$LZ4" -f -m "$IN" >/dev/null
test -f "$IN.lz4"
rm -f "$IN" "$IN.lz4"
echo "OK multifile default suffix"
