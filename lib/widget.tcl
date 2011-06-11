#
# widget.tcl --
#
# It is used to provide a GUI to allow users to choose
# among the possible hardware events easily.
#

package require Tk
 wm withdraw . 
 toplevel .selectEvents
 
set selected_event_list [list ]
set event_names [list ]
set event_codes [list ]  
 
#
# This function will be executed when 'Done' button is pushed. It creates
# a papi eventset for the selected events.
#
proc push_button {events_name} {
    global selected_event_list
    set selected_event_list [list ]
    set selections [.selectEvents.lst curselection] ; #Get the selected events
    foreach choice $selections {
        lappend selected_event_list [lindex $events_name $choice]
    }    
    set init_result [perft::init $selected_event_list]
    puts $init_result
    destroy .selectEvents
}

#
# Get the list of all available events
#
set event_list [perft::available_events]


#
# Storing event names and descriptions in list 'event_names'
#
set counter 0	
set tmp ""
foreach x $event_list {	
    if { [expr $counter%2] == 0} {
        lappend event_names [append tmp [lindex $event_list $counter] ": " [lindex $event_list [expr $counter+1]]]
        set tmp ""
    }
    incr counter
}

#
# Storing event codes  list 'event_codes'
#
set counter 0	
foreach x $event_list {	
    if { [expr $counter%2] == 0} {
        lappend event_codes [lindex $event_list $counter]
    }
    incr counter
}

#
# Widget elements
#
label .selectEvents.lab -text "Select the events to measure: \nTo select an event click on it and to deselect a selected event click on it once more."
button .selectEvents.but -text "Done" -command {push_button $event_codes}
frame .selectEvents.listarea
listbox .selectEvents.lst -listvariable event_names  -selectmode multiple -yscrollcommand ".selectEvents.srl_y set" -xscrollcommand ".selectEvents.srl_x set" -width 60 -height 30
scrollbar .selectEvents.srl_y -command ".selectEvents.lst yview" -orient v
scrollbar .selectEvents.srl_x -command ".selectEvents.lst xview" -orient h

#Geometry Management
grid .selectEvents.lab -row 0 -column 1
grid .selectEvents.listarea   -row 1 -column 1 -padx 40 -pady 40
grid .selectEvents.lst  -in .selectEvents.listarea -row 1 -column 1
grid .selectEvents.srl_y -in .selectEvents.listarea -row 1 -column 2 -sticky ns -columnspan 30
grid .selectEvents.srl_x -in .selectEvents.listarea -row 2 -column 1 -sticky ew -rowspan 30
grid .selectEvents.but -row 29 -column 1



