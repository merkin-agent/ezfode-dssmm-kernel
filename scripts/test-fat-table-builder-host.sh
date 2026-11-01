#!/bin/sh
set -eu

repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
test_tmp=$(mktemp -d)
trap 'rm -rf "$test_tmp"' EXIT HUP INT TERM

gcc -std=c99 -O2 -Wall -Wextra \
	-I"$repo_root/source" \
	"$repo_root/tests/fat-table-builder-host.c" \
	-o "$test_tmp/fat-table-builder-host"

"$test_tmp/fat-table-builder-host"
