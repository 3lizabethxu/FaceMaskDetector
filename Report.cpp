/**
 * @file Report.cpp
 * @brief Comprises the Report class, is created then filled with data then exported to the users file system
 */

/** -- Includes -- **/
#include "Report.hpp"

Report::Report(int people, float compliance){

    this->people = people;
    this->compliance = compliance;
    
    // get the file output folder from the environment variables
    const char* rootFolder = OUTPUT_FOLDER;
    // get current time for use as file name
    time_t curtime;
    time(&curtime);

    string dateTime = format("%s", ctime(&curtime));
    
    this->dateTime = dateTime;

    string outputLocation = format("%s/Compliance_Report_%s.csv", rootFolder, dateTime.c_str());
    
    this->outputLocation = outputLocation;

};

/** @brief destroys Report.
 *
 *  this just destroys the Report
 *
 */
Report::~Report(){

};

void Report::setOutputLocation(string outputLocation){
    this->outputLocation = outputLocation;
}

string Report::getOutputLocation(){
    return outputLocation;
}

void Report::exportFile(){

    std::ofstream exportReport;
    exportReport.open(outputLocation);
    exportReport << "This is a mask compliance report for: ";
    exportReport << dateTime.c_str();
    exportReport << "\n";

    // add people statistic
    exportReport << "People,";
    exportReport << people;
    exportReport << "\n";

    // add compliance statistic
    exportReport << "Compliance,";
    exportReport << compliance;
    exportReport << "\n";

    exportReport.close();

}
