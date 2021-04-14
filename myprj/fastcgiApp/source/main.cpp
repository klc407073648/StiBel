#include "Event.h"
#include "CLogin.h"
#include "CUpload.h"
#include "CRegister.h"
#include "Config.h"

#include "fcgi_config.h"
#include "fcgi_stdio.h"

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

using namespace std;
using StiBel::Log::LoggerManager;

int main(int argc, char** argv) 
{
	char **initialEnv = environ;
    int count = 0;
	
	LoggerManager::start("./conf/log4cpp.conf");
    
    Config mConfig(json_file,"my json conf file"); 

    Event mEvent(10);
    
    Login mLogin;
    Register mReg;
    Upload mUpload;

    mEvent.addEvent("/register", eventBind(Register::reg, mReg));
	mEvent.addEvent("/login", eventBind(Login::login, mLogin));
    mEvent.addEvent("/logout", eventBind(Login::logout, mLogin));
    mEvent.addEvent("/upload", eventBind(Upload::upload, mUpload));
    mEvent.exec();

	return 0;
}
