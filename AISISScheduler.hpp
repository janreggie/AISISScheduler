/*
AISIS Scheduler: does shit
Copyright (C) 2017, Dela Cruz, Park, Vallente

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/*
Descriptions of AISISScheduler namespace

Objects:

source.json (extern)
  - In the same folder of AISISScheduler.hpp, there should be a file
  named source.json. This shall comprise of a list of courses
  available from AISIS. The repository shall provide a sample
  source.json for reference.
class Course
  - comprises of a course, a section, and other details regarding it.
  Data comprising ::Course shall be taken from a JSON. More about it
  later.
std::list <std::string> requiredSubjects
std::list <AISISScheduler::Course> theList
std::list <AISISScheduler::Course> resultList
bool scheduleTable
  - This is a schedule table: 6x28. 6 because there are 6 (working) days
  in a week (0 being Mon, 5 being Sat), and 28 because there are 28 possible
  30-minute slots (from 07:00 to 21:00). Each cell can either be 0 or 1: 0 if
  that time slot has not been taken by any subject, and 1 if it already has
  been. This shall be memset'd every time the program shall create a new
  viable schedule. More on that later.

Functions:

void extractUserInput()
  - the user shall input a list of courses they are required to take.
  The user shall be asked to put the name of the course, and hit Enter
  after each time they type something. Once the user is done, they may
  type Enter without placing anything. The list of courses shall be
  pushed onto ::requiredSubjects.
void pushToTheList()
  - Once ::requiredSubjects is filled up, this function shall be used
  to read from source.json, check ::requiredSubjects, create the
  necessary ::Course's and push them onto ::theList. The source.json
  may take up a chunk of space, but, once the function exits, the
  JSON shall be freed up from memory.
bool isCompatible(AISISScheduler::Course subj)
  - This shall determine whether subj is compatible with ::scheduleTable
  by checking subj.weekDays and subj.timeSlot, creating a temporary time
  table akin to ::scheduleTale, and checking whether there are any conflicts.
*/

// include guard
#ifndef AISIS_HPP
#define AISIS_HPP 1

// dependencies
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "json.hpp"

/**
 * The catch-all namespace for everything in this header file. A namespace
 * is introduced to prevent conflicts with functions of the same name.
 */
namespace AISISScheduler 
{
  /**
   * @class Course
   * @date 12/01/17
   * @file AISISScheduler.hpp
   * @brief Container for a Course object
   * 
   * Contains a Course object, whose elements are subject code, section, 
   * among others.
   */
  class Course
  {
  public:
    /** The subject code (e.g. "EN 11"). Note the space. */
    std::string subjectCode;

    /** The section of a class, as presented in AISIS. */
    std::string section;

    /** The title of the course, as presented in AISIS. */
    std::string courseTitle;


    /** 
     * A list of weekdays where class is present (e.g. ["Mon", "Wed"]).
     * \note It is assumed that there are six days:
     * "Mon", "Tue", "Wed", "Thu", "Fri", "Sat". 
     */
    std::list <std::string> weekDays;

    /** 
     * The time slot of a subject (time in, to time out) in 24-hour time
     * (e.g. ["0800", "1000"] for a 8-to-10 class).
     * 
     * \note It is assumed that classes start not earlier than 0700
     * and not later than 2100. It is also assumed that the classes occur in
     * 30-minute intervals (so a time of "0845" or similar is invalid).*/
    std::list <std::string> timeSlot;

    /** The room where the course occurs in, as presented in AISIS. */
    std::string room;

    /** The instructor of a given course, as presented in AISIS. */
    std::string instructor;

    /** The language of a given course, as presented in AISIS. */
    std::string lang;

    /** 
     * A function that reads from some JSON file and copies over relevant
     * information to the class object. Check source.json for an example.
     * 
     * \note If some parameter is absent, it will stop reading, print out that
     * an exception has occured, and return to fallback values
     * (i.e. empty string).
     */
    void inputFromJSON(nlohmann::json source);

    /** 
     * Prints out data regarding the current course to some stream (e.g. to stdout) 
     */
    friend std::ostream& operator<<(std::ostream & output, const AISISScheduler::Course & C);

  };

  /** 
   * A list of subject codes the user is required to take.
   * Taken from user input (i.e. stdin, extractUserInput()). 
   */
  static std::list <std::string> requiredSubjects;


  /** 
   * A list of list of courses. Each element of scheduleList
   * is a viable schedule.
   */
  static std::list <std::list <AISISScheduler::Course> > scheduleList;

  /** 
   * A list of list of courses. Each element of theList is a list of
   * courses of the same subjectCode. Taken from pushToTheList(). 
   */
  static std::list <std::list <AISISScheduler::Course> > theList;

  /**
   * @brief Modifies theList based on source.json and requiredSubjects
   * 
   * Reads from source.json, and, for each element in requiredSubjects,
   * it shall append an empty list in theList, and from that empty list, it
   * will append a Course object if one can be found by reading source.json. 
   * 
   * In the case that there are no instances of a subject code in 
   * requiredSubjects in source.json, the function shall ignore that subject
   * code, and remove the empty list from theList.
   */
  void pushToTheList();

  /**
   * @brief Appends objects to requiredSubjects.
   * 
   * Reads the user input from std::getline, letting them type subject
   * codes, and appending these to requiredSubjects after pressing [Enter].
   * Detects whether duplicate entries are found. If [Enter] is pressed without
   * typing anything in, the function shall stop. 
   */
  void extractUserInput();

  /**
   * @brief 
   * @param subject, a Course object
   * @return boolean
   * 
   * Shall determine whether subject is compatible with scheduleTable.
   * Returns True if it is, False otherwise.
   */
  bool isCompatible(AISISScheduler::Course subject);

  /** 
   * @brief Starts the program proper.
   * 
   * Shall be the "frontend" of the program, if a programmer shall
   * carry the header file to another program.
   * 
   * Shall execute the following functions, in that order:
   * extractUserInput(), pushToTheList(), backTrack(). 
   */
  void start();

  /** 
   * A boolean two-dimensional C-like array, which contains whether a certain
   * 30-minute time slot (e.g. at Mon from 1400 to 1430) is taken. The first
   * dimension representing the days (0 for Mon, 5 for Sat), and the second
   * representing each 30-minute time slot (i.e. 0700-0730, 0730-0800, ... ,
   * 2030-2100). 
   */
  static bool scheduleTable[6][28];

  /**
   * @brief Make AISIS Great Again
   * 
   * This function shall be responsible for making AISIS great again. Earlier
   * versions of the program has given the name MAGA (or Make AISIS Great Again)
   * for this function.
   */
  void backTrack();

  /**
   * @brief Modify scheduleTable
   * @param subject
   * 
   * Shall modify scheduleTable so that the appropriate time slots for
   * subject are "taken" (i.e. flipped to 1).
   * 
   * \note It is assumed that isCompatible() returns True! 
   */
  void pushToSched(AISISScheduler::Course subject);

  /** 
   * Shall modify scheduleTable so that the appropriate time slots for
   * subject are "freed up" (i.e. flipped to 0).
   * 
   * \note It is assumed that pushToSched() was invoked! 
   */
  void pullFromSched(AISISScheduler::Course subject);

  /**
   * @brief Returns a permutation of 
   * @param courseList, a std::list of std::list objects of Course objects
   * @return a std::list of std::list objects of Course objects
   * 
   * From courseList, which is a list of lists with the same Course.subjectCode,
   * it shall return a list of permutations from courseList, wherein each 
   * element of the result is a viable schedule.
   */
  std::list <std::list <AISISScheduler::Course> > listOfPermutations(std::list <std::list <AISISScheduler::Course> > courseList);

  /**
   * @brief Helper function for removing first element of a list
   * @param lst, a std::list of type T
   * @return A std::list of type T
   * 
   * Returns the same lst but without the leading element. 
   */
  template <typename T> std::list <T> helper_removeFirstElement (std::list <T> lst)
  {
    std::list <T> result = lst;
    result.pop_front();
    return result;
  }
}

#endif // AISIS_HPP
