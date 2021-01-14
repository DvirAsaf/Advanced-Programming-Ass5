

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"


using namespace std;

class CLI {

    // you can add data members
public:
    DefaultIO* dio;
    CommandHelp help;
    Command* comArr[6]{};
    CLI(DefaultIO* dio);
    void start();
    virtual ~CLI();
};

#endif /* CLI_H_ */
