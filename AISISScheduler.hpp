/*
AISIS Scheduler: does shit
Copyright (C) 2017, Dela Cruz, Sewon, Vallente

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
#include "json.hpp"

namespace AISISScheduler
{
  class Course
  {
  public:
    std::string subjectCode;
    std::string section;
    std::string courseTitle;
    std::list <std::string> weekDays;
    std::list <std::string> timeSlot;
    std::string instructor;
    std::string lang;
    void inputFromJSON(nlohmann::json source);
  };
  static std::list <std::string> requiredSubjects;
  static std::list <std::list <AISISScheduler::Course> > theList;
  static std::list <AISISScheduler::Course> resultList;
  void pushToTheList();
  void extractUserInput();  // get from userinput to requiredSubjects
  bool isCompatible(AISISScheduler::Course subject);
  void start();  // do shit here
  static bool scheduleTable[6][28];  // 0 if free; 1 if taken
}

#endif // AISIS_HPP
