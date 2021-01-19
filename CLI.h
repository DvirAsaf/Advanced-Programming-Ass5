
/**
 * @author : Dvir Asaf 313531113.
 */
#ifndef CLI_H_
#define CLI_H_
#include <string.h>
#include "commands.h"
#include <memory>
using namespace std;
class CLI {
    DefaultIO* dio;
    CommandHelp* help;
    Command* comArr[6];
public:
    CLI(DefaultIO* dio);
    void start();
    virtual ~CLI();
};
#endif /* CLI_H_ */