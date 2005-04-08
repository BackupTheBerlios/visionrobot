#!/bin/bash

aclocal \
&& autoheader \
&& libtoolize --force \
&& automake --add-missing --gnu \
&& autoconf
