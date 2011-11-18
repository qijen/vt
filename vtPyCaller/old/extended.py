#-------------------------------------------------------------------------------
#           Name: extended.py
#         Author: Kevin Harris
#  Last Modified: 01/19/07
#    Description: This script tests our new Boost.Python module by calling its
#                 functions.
#
# Note: To test your new module, you'll need to either place a copy of the
#       "extended_test.pyd" in the "DLLs" directory of your Python installation
#       (example: C:\Python25\DLLs), or move this script to be in the same 
#       directory as the "extended_test.pyd" so it can be loaded locally.
#-------------------------------------------------------------------------------

import extended_test

extended_test.returnNothing()

print extended_test.returnString()

extended_test.setValue( 55 );

print "getValue() returned: " + str( extended_test.getValue() )

player1 = extended_test.Player()
player1.setName( "player_1" )
print "player1.getName() returned: " + player1.getName()

player2 = extended_test.Player( "player_2" )
print "player2.getName() returned: " + player2.getName()

player3 = extended_test.Player( "player_3", 50 )
print "player3.getName() returned: " + player3.getName()
print "player3.getScore() returned: " +  str( player3.getScore() )
player3.setScore( 100 )
print "player3.getScore() returned: " +  str( player3.getScore() )

raw_input( '\nPress Enter to exit...' )
