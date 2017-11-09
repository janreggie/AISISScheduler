#include "AISISScheduler.hpp"
#include "json.hpp"
// do shit here daisuki de

void AISISScheduler::pushToTheList()
{
  // read from json
  std::ifstream i ("source.json");
  nlohmann::json allCourses;
  i >> allCourses;

  // overwrite allCourses to make it allCourses["Classes"]
  // to make shit easier
  allCourses = allCourses.at("Classes");

  // for each element in requiredSubjects, check whether it
  // is equal to allCourses.at("subjectCode")
  for (auto &x : AISISScheduler::requiredSubjects)
  {
    AISISScheduler::theList.push_back({});
    auto theLastElement = --AISISScheduler::theList.end();  // the last element
    for (int j = 0; j < allCourses.size(); ++j)
    {
      if (x == allCourses.at(j).at("subjectCode"))
      {
        AISISScheduler::Course currentClass;
        currentClass.inputFromJSON(allCourses.at(j));
        theLastElement->push_back(currentClass);
      }
    }
  }

  // the JSON object need not be deleted since, outside the
  // function, it becomes out of scope.
}

void AISISScheduler::extractUserInput()
{
  std::cout << "Please input your CAT codes for all of the subjects you are required to take this semester.\n\n";
  std::cout << "Please press enter after typing each CAT codes.\n\n";
  std::cout << "e.g. use EN 11 instead of EN11\n\n";
  std::cout << "Please press enter without writing anything when you have inputted all the required CAT codes.\n\n";
  std::string CAT;
  do
  {
    std::getline(std::cin,CAT);

    if (CAT != "")
    {
      bool isThereDupe = false;
      for (auto & x : AISISScheduler::requiredSubjects)
      {

        if (x == CAT)
        {
          std::cout << "Duplicate Input. Please try agian.\n";
          isThereDupe = true;
        }
      }
      if (isThereDupe)
      {
        continue;
      }
      AISISScheduler::requiredSubjects.push_back(CAT);
    }
  }
  while (CAT != "");
  std::cout << "START HERE:\n";
  for (auto & x : AISISScheduler::requiredSubjects)
  {
    std::cout << " " << x << std::endl;
  }
  std::cout << "END BITCH";
}

void AISISScheduler::start()
{
  // extract user input
  AISISScheduler::extractUserInput();

  // do list pushing
  AISISScheduler::pushToTheList();
}

void AISISScheduler::Course::inputFromJSON(nlohmann::json source)
{
  this->subjectCode = source.at("subjectCode");
  this->section = source.at("section");
  this->courseTitle = source.at("courseTitle");
  for (int j = 0; j < source.at("weekDays").size(); ++j)
  {
    this->weekDays.push_back(source.at("weekDays").at(j));
  }
  for (int j = 0; j < source.at("timeSlot").size(); ++j)
  {
    this->timeSlot.push_back(source.at("timeSlot").at(j));
  }
  this->instructor = source.at("instructor");
  this->lang = source.at("lang");
}

bool AISISScheduler::isCompatible(AISISScheduler::Course subject)
{
  //get details of time and save it in a temporary vector
  std::list<std::string>::iterator i;
  int temptime [2] = {0, 0};  // used to compare with ::scheduleTable
  i = subject.timeSlot.begin();
  std::list<std::string> listOfTimes = {"0700", "0730", "0800", "0830",
  "0900", "0930", "1000", "1030", "1100", "1130", "1200", "1230", "1300",
  "1330", "1400", "1430", "1500", "1530", "1600", "1630", "1700", "1730",
  "1800", "1830", "1900", "1930", "2000", "2030", "2100"};
  // check starting time, and iterate temptime[0]
  //if is equal to the initial time
  // additionally, check the latter time
  for (auto x : listOfTimes)
  {
    if (*i == x)
    {
      break;
    }
    ++temptime[0];
  }
  // note that the latter is gonna be the time the class ENDS
  ++i; // for the next timeslot
  for (auto x : listOfTimes)
  {
    if (*i == x)
    {
      --temptime[1];
      break;
    }
    ++temptime[1];
  }
/*  Judd's version (works, but not for all weird use cases)
 *  (delete this for later)
 *   int tempx = std::stoi (*i);
 *   ++i;
 *   int tempy = std::stoi (*i);
 *   int tempdiff = tempy - tempx;
 *
 *   if (tempdiff == 100)
 *     temptime.push_back(temptime[0]+1);
 *   if (tempdiff == 130)
 *     temptime.push_back(temptime[0]+1);
 *     temptime.push_back(temptime[0]+2);
 *   if (tempdiff == 200)
 *   {
 *     for(int i = 1; i<4; i++)
 *       temptime.push_back(temptime[0]+i);
 *   }
 *   if (tempdiff == 300)
 *   {
 *     for(int i = 1; i<6; i++)
 *       temptime.push_back(temptime[0]+i);
 *   }
 *   if (tempdiff == 400)
 *   {
 *     for(int i = 1; i<8; i++)
 *       temptime.push_back(temptime[0]+i);
 *   }
 */

  //get weekdays detail and store in tempvector
  std::vector<int> tempday;

  for(auto j = subject.weekDays.begin(); j != subject.weekDays.end(); ++j)
  {
    if (*j == "Mon")
      tempday.push_back(0);
    else if (*j == "Tue")
      tempday.push_back(1);
    else if (*j == "Wed")
      tempday.push_back(2);
    else if (*j == "Thu")
      tempday.push_back(3);
    else if (*j == "Fri")
      tempday.push_back(4);
    else if (*j == "Sat")
      tempday.push_back(5);
  }

  //check for availability of day&timeslot
  for(auto a : tempday)
  {
    for(int b = temptime[0]; b <= temptime[1]; ++b)
    {
      if (AISISScheduler::scheduleTable[a][b] == false)
        continue;
      else
        return false;
    }
  }
  return true;
}

std::ostream& AISISScheduler::operator<<(std::ostream & output, const AISISScheduler::Course & C)
{
	std::cout << "SUBJECT CODE: " << C.subjectCode << std::endl;
	std::cout << "SECTION: " << C.section << std::endl;
	std::cout << "COURSE TITLE: " << C.courseTitle << std::endl;
	std::cout << "SCHEDULE: ";
	//output weekdays
	std::list<std::string>::iterator i;
	for(auto i : C.weekDays)
		std::cout << i << " ";
	//output timeslot
	for(auto i : C.timeSlot)
		std::cout << i << " ";
	std::cout << std::endl;
	std::cout << "INSTRUCTOR: " << C.instructor << std::endl;
	std::cout << "LANGUAGE: "<< C.lang << std::endl << std::endl;

	return output;
}

