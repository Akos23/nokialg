#include<iostream>
#include"Logger.h"
#include<chrono>
#include<ctime>
#include<time.h>

using namespace std;
#pragma warning(disable : 4996)


int main()
{
	/*nokialg::Logger Logger;
	
	Logger.log(nokialg::Level::WARNING, "semmi komoly ne aggodj", "forrás");
	Logger.getErrors();
	Logger.clear(5);*/

    //std::time_t t = std::time(0);   // get time now
    //std::tm* now = std::localtime(&t);
    //std::cout << (now->tm_year + 1900) << '-'
    //    << (now->tm_mon + 1) << '-'
    //    << now->tm_mday
    //    << "\n";

    /*auto end = std::chrono::system_clock::now();

    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time);*/

    string testmsg = "out of bounds";
    nokialg::Level testlvlDebug= nokialg::DEBUG;
    nokialg::Level testlvlWarning = nokialg::WARNING;
    string testsource = "Source.cpp";

    nokialg::Logger logger;
    logger.log(testlvlDebug, testmsg, testsource);
    logger.log(testlvlWarning, testmsg, testsource);
    logger.log(nokialg::INFO, "new text msg", testsource);
    logger.log(testlvlDebug, testmsg, testsource);
    logger.getErrors();
    logger.log(nokialg::ERROR, testmsg, testsource);
    logger.log(nokialg::ERROR, testmsg, testsource);
    logger.getErrors();




}
