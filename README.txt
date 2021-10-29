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

