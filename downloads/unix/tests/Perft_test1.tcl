#
# procedure factorial
#
proc Factorial {x} {
	set i 1; set product 1
	while {$i <= $x} {
		set product [expr $product * $i]
		incr i
	}
	return $product
}
 
#
# script
#
set script  {
    for {set i 0} {$i  < 10} {incr i} {
    set fact [Factorial $i];
    }
}

eval $script
