# Introduction #

## Parameters ##

Tasks have a number of parameters, some of which have a state as well as a value.

Examples of parameters that that states as well as a value are:
  * Start - date & time when task starts (not necessarily when allocated resources start spending effort)
  * End - date & time when task ends (not necessarily when allocated resources finish spending effort)
  * Duration - working time period between task start and end based on a calendar
  * Work - time allocated by assigned resources


## States ##

Typically there are two states
  1. Automatic - i.e. derived by QPlanner from other information enter by the user
  1. Fixed - i.e. user entered and defined

## Examples ##

### Start ###

Default = Automatic
  * Automatic = QPlanner sets value to project start, or after subject to task predecessors (TBC)
  * Fixed = User entered value used to defined date & time task must not start before (TBC)