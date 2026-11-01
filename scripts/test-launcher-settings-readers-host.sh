#!/bin/sh
# Differential test: the on/off settings readers in the working tree must behave exactly like the
# readers in a reference version of source/ezkernelnew.c (default: 83e350d, the last commit with the original per-setting readers).
# Usage: scripts/test-launcher-settings-readers-host.sh [reference-git-ref]
set -eu

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
ref=${1:-83e350d}
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT HUP INT TERM

# Extract the verbatim readers (and the shared reader, if the version has one) from a source file.
extract() {
	awk '
		/^static void Launcher_Read(Sounds|HideSystemFiles|ListFolders|CleanList|WrapAround|OnOff)Setting\(/ {p=1}
		p {print}
		p && /^}/ {p=0; print ""}
	' "$1"
}

git -C "$repo_root" show "$ref:source/ezkernelnew.c" > "$tmp/ref.c"
extract "$tmp/ref.c" > "$tmp/ref_readers.inc"
extract "$repo_root/source/ezkernelnew.c" > "$tmp/new_readers.inc"
[ -s "$tmp/ref_readers.inc" ] && [ -s "$tmp/new_readers.inc" ] || { echo "FAIL: could not extract readers" >&2; exit 1; }

for v in ref new; do
	gcc -std=gnu99 -O2 -Wall -Wno-unused-function -I"$repo_root/source" \
		-DREADERS_FILE="\"$tmp/${v}_readers.inc\"" \
		"$repo_root/tests/launcher-settings-readers-host.c" -o "$tmp/$v"
	"$tmp/$v" > "$tmp/$v.out"
done

lines=$(wc -l < "$tmp/new.out")
if cmp -s "$tmp/ref.out" "$tmp/new.out"; then
	echo "Settings readers behave identically to $ref over $lines cases."
else
	echo "FAIL: readers differ from $ref:" >&2
	diff "$tmp/ref.out" "$tmp/new.out" | head -10 >&2
	exit 1
fi
