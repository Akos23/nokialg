#include<iostream>
#include"Logger.h"


using namespace std;



int main()
{
    

   

    string testmsg = "here would be some log message";
    nokialg::LogLevel d = nokialg::DEBUG;
    nokialg::LogLevel w = nokialg::WARNING;
    string testsource = "Source";

    nokialg::Logger logger;

    logger.log(d, testmsg, testsource);
    logger.log(w, testmsg, testsource);
    logger.log(nokialg::INFO, "some other log msg", testsource);
    logger.log(d, testmsg, testsource);
    logger.getErrors();
    logger.log(nokialg::ERROR, testmsg, testsource);
    logger.log(nokialg::ERROR, testmsg, testsource);
    logger.getErrors();
    logger.clear(-1);
    logger.getErrors();
    logger.clear(-2);
    logger.log(nokialg::ERROR, "some error msg","main.cpp");
    logger.getErrors();
    logger.clear(-4);






}