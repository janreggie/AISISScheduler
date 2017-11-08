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
      AISISScheduler::requiredSubjects.push_back(CAT);
    }
  }
  while (CAT != "");
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
