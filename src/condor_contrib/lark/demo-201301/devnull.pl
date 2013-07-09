#! /usr/bin/perl

my $READ_SIZE=1024;

my $total = 0;
my $tmp;

while(my $size = read STDIN, $tmp, $READ_SIZE) { $total += $size; }
print <<END;
Content-type: text/plain

$total
END
