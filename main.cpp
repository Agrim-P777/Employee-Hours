#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <utility>
#include <ctime>
#include <iomanip>
#include <time.h>
#include <chrono>

using namespace std;

//Below are function declarations
bool isLeapYear(int year);
int getLastDayOfMonth(int month, int year);
std::string getNextDay(const std::string& inputDate);
bool different_day(std::string timeStr1,std::string timeStr2);


// Define the struct to store data
struct Employee {
    std::string positionID;
    std::string positionStatus;
    std::string timeIn;
    std::string timeOut;
    std::string timecardHours;
    std::string payCycleStartDate;
    std::string payCycleEndDate;
    std::string employeeName;
    std::string fileNumber;
};

// Function to parse a line and fill the struct
Employee parseLine(const std::string& line) {
    Employee data;
    std::istringstream iss(line);
    
    // Assuming tab-separated values, you can change the delimiter if needed
    std::getline(iss, data.positionID, '\t');
    std::getline(iss, data.positionStatus, '\t');
    std::getline(iss, data.timeIn, '\t');
    std::getline(iss, data.timeOut, '\t');
    std::getline(iss, data.timecardHours, '\t');
    std::getline(iss, data.payCycleStartDate, '\t');
    std::getline(iss, data.payCycleEndDate, '\t');
    std::getline(iss, data.employeeName, '\t');
    std::getline(iss, data.fileNumber, '\t');

    return data;
}

// Function to extract the date from the timeIn string
std::string extractDate(const std::string timeIn) {
    // Assuming the format is "MM-DD-YYYY HH:mm AM/PM"
    // Extract the date part from the string
    return timeIn.substr(0, 10);
}

/*
The below function "find_seven_days" prints the positionID and name of employees 
who have worked for 7 consecutive days.
Algorithm:
1. Create a map using  employeeName as the key, and pair(date, count)
                    employeeName -> {date, count}
2. If the data is blank or is the same day continue;  
3. Increment the count variable if its the next consecutive day.
4. Detail comments line-by-line in the function below.

Edge-Cases:
1. The getNextDay function (implemented below main) considers the edge cases where the next day is the end of the month or year and returns value accordingly,
    it also consideres the edge case of leap-year i.e, 29th of Feburary.
2. Considered the employee only if positionStatus == "Active"
3. Assuming employeeName to be unique as the positionID, if not key of the map should be positionID.
4. Only if timecardHours is not equal to 0:00, employee is considered to have worked, else that day is not considered.
5. Stored the data in a set before printing to avoid duplicate printing of values.
6. If an employee works only morning shift then also it is considered for the 7-consecutive days.
7. If "timeOut" is blank, then timecardHours==0 and that will not be counted.
*/

void find_seven_days(std::vector<Employee>& employeeList){
    // Map to store the consecutive working days count for each employee
    std::unordered_map<std::string, std::pair<std::string,int>> consecutiveDaysMap;
    std::unordered_set<std::string> resultSet; 
    // Use std::unordered_set for unique pairs
    // Iterate through the data and process only for "Active" positions

    for (const auto& data : employeeList) {
        if (data.positionStatus == "Active" && data.timecardHours != "0:00") {
            std::string date = extractDate(data.timeIn);
            std::string key = data.employeeName;
            
            // Check if the employee has already been encountered
            if (consecutiveDaysMap.find(key) != consecutiveDaysMap.end()) {
		
                // Check if the current date is same as the stored date.
                if (date == consecutiveDaysMap[key].first) {
                    consecutiveDaysMap[key].first = date; // Update the stored date
                } else if (date == ""){
                    continue;   //If null continue
                } else if (date == getNextDay(consecutiveDaysMap[key].first)){
                    //If its a consecutive day, update the date and increase the counter.
                    consecutiveDaysMap[key].first = date;
                    consecutiveDaysMap[key].second++;
                } else{ 
                    //else if the streak breaks reset the date and counter.
                    consecutiveDaysMap[key].first = date;
                    consecutiveDaysMap[key].second = 1;
                }

                // Check if the employee has worked for 7 consecutive days
                if (consecutiveDaysMap[key].second == 7) {
                    resultSet.insert("Position ID: "+ data.positionID + ", Employee Name: " + data.employeeName);
                }
            } else {
                // First time encountering the employee, initialize the counter and store the date
                consecutiveDaysMap[key].first = date;
                consecutiveDaysMap[key].second = 1;
            }
        }
    }  
    //Print the values to console or file as needed!
    for (string s : resultSet) {
        std::cout << s <<'\n';
    }  
}

bool isTimeMoreThan14Hours(const std::string& timeString) {
    // Assuming the input string is in the format "hh:mm"
    size_t colonPos = timeString.find(':');

    if (colonPos == std::string::npos || colonPos == 0 || colonPos == timeString.length() - 1) {
        // Invalid format, return false
        return false;
    }

    // Extract hours and minutes from the string
    std::string hoursStr = timeString.substr(0, colonPos);
    std::string minutesStr = timeString.substr(colonPos + 1);

    try {
        int hours = std::stoi(hoursStr);
        int minutes = std::stoi(minutesStr);

        // Convert hours to minutes and check if the total time is more than 14 hours
        int totalMinutes = hours * 60 + minutes;
        return totalMinutes > 14 * 60;
    } catch (std::invalid_argument& e) {
        // Failed to convert to integers, return false
        return false;
    }
}

/*
The below function "find_fourteen_hrs" prints the positionID and name of employees 
who has worked for more than 14 hours in a single shift
1. A single shift is considered based on the timecardHours.

*/
void find_fourteen_hrs(std::vector<Employee>& employeeList){
    std::unordered_set<std::string> resultSet; 
    // Use std::unordered_set for unique pairs
	for (const auto& data : employeeList){
	    string timestamp = data.timecardHours;
        	if(isTimeMoreThan14Hours(timestamp)){
        	    resultSet.insert("Position ID: "+ data.positionID + ", Employee Name: " + data.employeeName);
        	}
	}
	//Print the values to console or file as needed!
    for (string s : resultSet) {
        std::cout << s <<'\n';
    }
}


bool isTimeDifferenceValid(const std::string timeStr1, const std::string timeStr2) {
    // Convert string representations to std::tm
    std::tm time1 = {}, time2 = {};
    std::istringstream ss1(timeStr1), ss2(timeStr2);
    ss1 >> std::get_time(&time1, "%m/%d/%Y %I:%M %p");
    ss2 >> std::get_time(&time2, "%m/%d/%Y %I:%M %p");

    if (ss1.fail() || ss2.fail()) {
        // Invalid input format
        return false;
    }

    // Convert std::tm to std::chrono::system_clock::time_point
    auto tp1 = std::chrono::system_clock::from_time_t(std::mktime(&time1));
    auto tp2 = std::chrono::system_clock::from_time_t(std::mktime(&time2));

    // Calculate the time difference in hours
    auto duration = std::chrono::duration_cast<std::chrono::hours>(tp2 - tp1);

    // Check if the time difference is greater than 1 hour and less than 10 hours
    return duration.count() > 1 && duration.count() < 10;
}


/*
The below function "find_shift_time" prints the positionID and name of employees 
who have less than 10 hours of time between shifts but greater than 1 hour

Assumptions:
1. The data is sorted based on positionID, so a linear scan is sufficient, if the data were not sorted
then a data structure should be used to keep track of the values.

*/

void find_shift_time(std::vector<Employee>& employeeList){
    int n = employeeList.size();
    int i=0;
    std::unordered_set<std::string> resultSet;
    while(i<(n-1)){
        auto data = employeeList[i];
        auto data_next = employeeList[i+1];
        if(data.timeIn=="" || data_next.timeOut=="") {
		i++;
		continue;
	}
        if(different_day(data.timeIn,data_next.timeOut)) i++; //If both the dates are different, the time difference is not between the shifts.
        else{
            if(isTimeDifferenceValid(data.timeOut,data_next.timeOut)){
                //If we find the required time difference between the shifts then we add that to the set.
                resultSet.insert("Position ID: "+ data.positionID + ", Employee Name: " + data.employeeName);
            }
            i = i+2;
        }
    }
    //Print the values to console or file as needed!
    for (string s : resultSet) {
        std::cout << s <<'\n';
    }
}



int main() {
    std::ifstream inputFile("Assignment_Timecard.tsv"); //Taking the input from the file.
    
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::vector<Employee> employeeList;
    std::string line;

    // Skip the header line
    std::getline(inputFile, line);

    while (std::getline(inputFile, line)) {
        Employee data = parseLine(line);
        employeeList.push_back(data);
    }

    // Now employeeList contains all the data in the struct

    cout<<"PositionID and name of employees who has worked for 7 consecutive days.\n";
    find_seven_days(employeeList);
    cout<<'\n';
    
    cout<<"PositionID and name of employees who have less than 10 hours of time between shifts but greater than 1 hour\n";
    find_shift_time(employeeList);
    cout<<'\n';
    
    cout<<"PositionID and name of employees who has worked for more than 14 hours in a single shift\n";
    find_fourteen_hrs(employeeList);
    cout<<'\n';

    inputFile.close();

    return 0;
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int getLastDayOfMonth(int month, int year) {
    if (month == 2) {
        return isLeapYear(year) ? 29 : 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
    }
}

std::string getNextDay(const std::string& inputDate) {
    std::istringstream ss(inputDate);
    std::tm tmDate = {};
    ss >> std::get_time(&tmDate, "%m/%d/%Y");

    tmDate.tm_mday++;  // Increase the day by one

    // Check if the new day is valid, considering month and year boundaries
    if (tmDate.tm_mday > getLastDayOfMonth(tmDate.tm_mon + 1, tmDate.tm_year + 1900)) {
        tmDate.tm_mday = 1;
        tmDate.tm_mon++;  // Move to the next month

        if (tmDate.tm_mon > 11) {
            tmDate.tm_mon = 0;
            tmDate.tm_year++;  // Move to the next year
        }
    }

    std::ostringstream result;
    result << std::put_time(&tmDate, "%m/%d/%Y");
    return result.str();
}

bool different_day(std::string timeStr1,std::string timeStr2){
    string s1 = extractDate(timeStr1);
    string s2 = extractDate(timeStr2);
    if(s1!=s2) return true;
    else return false;
}
