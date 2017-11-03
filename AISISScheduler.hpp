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
// include guard
#ifndef AISIS
#define AISIS

// dependencies
#include <list>
#include <string>
#include <iostream>
#include "json.hpp"

namespace AISISScheduler
{
  class Course
  {
  private:
    std::string subjectCode;
    std::string section;
    std::string courseTitle;
    std::list <std::string> weekDays;
    std::list <std::string> timeSlot;
    std::string instructor;
    std::string lang;
    friend std::ostream & operator<<
      (std::ostream & output, const AISISScheduler::Course & C);
  public:
    Course();
  };
  std::list <std::string> requiredSubjects;
  std::list <AISISScheduler::Course> theList;
  std::list <AISISScheduler::Course> resultList;
  void extractUserInput();
  void main();
  bool scheduleTable[7][28];
}

#endif // AISIS
