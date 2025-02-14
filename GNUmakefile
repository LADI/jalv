# Copyright 2025 Nedko Arnaudov
# SPDX-License-Identifier: 0BSD OR ISC

.PHONY: build/jalv
build/jalv: build
	meson compile -C build

build:
	meson setup build --wrap-mode=forcefallback -Dzix:default_library=static -Dsord:default_library=static -Dserd:default_library=static -Dsuil:default_library=static -Dsratom:default_library=static -Dlilv:default_library=static --debug -Dtests=enabled

test: build/jalv
	meson test -C build

clean:
	rm -rf build
