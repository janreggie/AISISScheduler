#!/usr/bin/env python3

import sys
import os

def parse(filename):
    """
    Parse filename to filename.json, wherein filename is some
    copied text from AISIS.
    """
    # check first whether filename.json exists
    # if it does, prompt the user on what he is doing
    if (os.path.exists(filename + ".json")):
        print("WARNING: FILE EXISTS. OVERWRITE? [y|N]", end=' ')
        s = input()
        if (s == 'y'):
            print("Okay. Overwriting.")
        else:
            print("Okay. Exiting.")
            return 0  # voluntary exit

    # now for the meat of the function
    # read toRead (filename)
    # read (and overwrite, if necessary!) toWrite (filename.json)
    toRead = open(filename, 'r')
    toWrite = open(filename + ".json", 'w')
    # and write stuff in toWrite ("Classes" and stuff)
    toWrite.write("""{\n  "Classes": [\n""")

    # supplementary variables (for later)
    dictDays = {"M": "Mon",
                "T": "Tue",
                "W": "Wed",
                "TH": "Thu",
                "F": "Fri",
                "SAT": "Sat",
                "TUTORIAL": "TUTORIAL",
                "TBA": "TBA"}

    # for every line, do something with it
    for line in toRead:
        # append a brace
        toWrite.write("    {\n")
        # create a temporary variable which is a list
        course = line.split('\t')

        # and add stuff to toWrite
        # subjectCode, section, and courseTitle
        toWrite.write("""      "subjectCode": "%s",\n""" % course[0][:-1])
        toWrite.write("""      "section": "%s",\n""" % course[1][:-1])
        toWrite.write("""      "courseTitle": "%s",\n""" % course[2][:-1])

        # weekDays
        listDays = [dictDays[i] for i in course[4].split(' ')[0].split('-')]
        toWrite.write("""      "weekDays": """)
        toWrite.write("[\"%s\"]" % "\", \"".join(listDays))
        toWrite.write(",\n")

        # timeSlot
        # NOTE: Even if it says TBA, it should NOT matter, listTime
        # shall be [""].
        listTime = course[4].split(' ')[1].split('-')
        toWrite.write("""      "timeSlot": """)
        toWrite.write("[\"%s\"]" % "\", \"".join(listTime))
        toWrite.write(",\n")

        # room, instructor, lang
        toWrite.write("""      "room": "%s",\n""" % course[5][:-1])
        toWrite.write("""      "instructor": "%s",\n""" % course[6][:-1])
        toWrite.write("""      "lang": "%s"\n""" % course[8][:-1])

        # and close that brace
        toWrite.write("    },\n")

    # Once that's loop's done, do the following:
    toWrite.write("  ]\n}")
    toWrite.close()


if __name__ == "__main__" and len(sys.argv) == 2:
    print(sys.argv[1])
    parse(sys.argv[1])
