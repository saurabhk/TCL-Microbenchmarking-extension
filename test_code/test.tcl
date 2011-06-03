package require Tcl 8.5




proc quicksort {m} {
    if {[llength $m] <= 1} {
        return $m
    }
    set pivot [lindex $m 0]
    set less [set equal [set greater [list]]]
    foreach x $m {
        lappend [expr {$x < $pivot ? "less" : $x > $pivot ? "greater" : "equal"}] $x
    }
    return [concat [quicksort $less] $equal [quicksort $greater]]
}


proc Factorial {x} {

	set i 1; set product 1

	while {$i <= $x} {

		set product [expr $product * $i]

		incr i

	}

	return $product

}
 

set script {puts [Factorial 20];

puts [quicksort {8 6 4 2 1 3 5 7 9}] ;# => 1 2 3 4 5 6 7 8 9
};


#load ./libperf5.so;

#perf2::init [list PAPI_TOT_CYC  PAPI_L1_DCM];

#set result [ perf2::run1 $script];
#puts $result;






