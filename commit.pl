#!/usr/bin/perl

use strict;

my $name = shift @ARGV || die "\nLack of commit name\n\n";

foreach my $arg ( @ARGV ) {
  $name .= " $arg";
}

system("git add README\n");
system("git add LICENSE\n");
system("git add *.pl\n");
system("git add Makefile\n");
system("git add *.cpp\n");
system("git add lib/Makefile\n");
system("git add lib/*.cpp\n");
system("git add lib/*.h\n");
system("git add data/images/tiles/*.png");
system("git add data/images/transitions/*.png");
system("git add data/images/icons/*.png");
system("git add data/images/chars/*.png");
system("git add data/images/backgrounds/*.png");
system("git add data/sounds/*.wav");
system("git add data/font/font.ttf");

system("git commit -m '$name'\n");
