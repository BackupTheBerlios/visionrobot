#!/bin/bash

aclocal \
&& autoheader \
&& automake --add-missing --gnu \
&& autoconf \
