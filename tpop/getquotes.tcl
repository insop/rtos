# Copyright (C) 1999 Lucent Technologies
# Excerpted from 'The Practice of Programming'
# by Brian W. Kernighan and Rob Pike

# getquotes.tcl: stock prices for Lucent, AT&T, Microsoft

set so [socket quote.yahoo.com 80]  ;# connect to server
set q "/d/quotes.csv?s=LU+T+MSFT&f=sl1d1t1c1ohgv"

puts $so "GET $q HTTP/1.0\n\n"      ;# send request
flush $so
puts [read $so]                     ;# read & print reply
