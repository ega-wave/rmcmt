#
# Makefile for rmcmt
#
# Copyright (C) 2023 Yoshitaka Egawa.
#
# This program is free software: you can redistribute it 
# and/or modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either 
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, 
# but WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with this program.
# If not, see <https://www.gnu.org/licenses/>.
#
# Written by Yoshitaka Egawa.
#

all: rmcmt rmcmt_slaster rmcmt_dslash

rmcmt : rmcmt.c
	gcc rmcmt.c -o rmcmt

rmcmt_slaster : rmcmt_slaster.c
	gcc rmcmt_slaster.c -o rmcmt_slaster

rmcmt_dslash : rmcmt_dslash.c
	gcc rmcmt_dslash.c -o rmcmt_dslash

test :
	bash test.bash

clean:
	rm rmcmt rmcmt_slaster rmcmt_dslash

