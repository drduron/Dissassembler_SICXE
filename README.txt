Brief summary of algorithm:

Gather the important data within test.sym and store within a hashmap for constant time accessing the data.
Iterate through each of the lines within test.obj. Check the first character to see the record type.
Depending on the record type, will determine which conditional the code will pass through.
If a text file, check if the address is within either of the hashmaps. If so, use that data.
If not, then check the characters by converting to decimal and checking the bits. This will give
information regarding the addressing type, format type, and whether or not it is indexed or not.
After gathering important data about the specific segment of the record, append onto a string for later use.
Continue going through the record until it has fully been traversed. After this, return the string and
output it onto out.lst for reading. This algorithm uses several helper functions that read characters as
hex values, convert between binary, hex, or decimal, and also compute differing values.

The majority of the logic happens within the text function. This function is seperated into various sections
that allowed me to focus on different parts. This includes finding whether an address from the maps is in
the specific line of object code. Then the operation is determined depending on the format. The format is determined
by the hex values being converted to a binary string, which is scanned for important bits such as nixbpe and the
object code. Then the algorithm checks the displacement at the end of the object code to determine what kind of
operation needs to be done.