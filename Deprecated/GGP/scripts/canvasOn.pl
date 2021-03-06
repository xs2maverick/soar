#!/usr/bin/perl

$file = "../agents/header.soar";

$ipfile = "tmp_hdr";

open $IN, "<$file" or die;

die if (-e $ipfile);
open $OUT, ">$ipfile" or die;

$changed = 0;
foreach $line (<$IN>) {
  if ($line =~ /\s*#\s*source canvas.soar/) {
    print $OUT "source canvas.soar\n";
    $changed++;
  }
  else {
    print $OUT $line;
  }
}

close $OUT;
print `mv $ipfile $file`;

if ($changed != 1) {
}
