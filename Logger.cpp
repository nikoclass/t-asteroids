#include "Logger.h"

#ifdef DO_DEBUG

#include <sstream>
#include <string>

LogLevel LogLevel::Debug   ("[DEBUG]", 0);
LogLevel LogLevel::Info    ("[INFO]", 1);
LogLevel LogLevel::Warning ("[WARNING]", 2);
LogLevel LogLevel::Error   ("[ERROR]", 3);
LogLevel LogLevel::Fatal   ("[FATAL]", 4);
ostream& operator << (ostream & co, const LogLevel & logLevel)
{
    co << logLevel.getValue();
    return co;
}
bool operator>=(const LogLevel & log1,const LogLevel & log2)
{
    return log1.getPriority() >= log2.getPriority();
}

Logger* Logger::s_instance = NULL;

Logger* Logger::getInstance()
{
    if (!s_instance)
        s_instance = new Logger();
    return s_instance;
}

Logger::Logger() :m_minLevel(LogLevel::Debug)
{
    InitializeCriticalSection(&m_lockLog);
}

Logger::~Logger()
{
    m_logStream.close();
    DeleteCriticalSection(&m_lockLog);
}



void Logger::Init(const LogLevel & minLevel, const string & logName, bool overWrite, const string & funcName, int line)
{
    EnterCriticalSection(&m_lockLog);
        m_minLevel = minLevel;
        string fileName(logName);
        if (fileName.find_last_of(".") == string::npos){
            fileName += ".txt";
        }
        m_logStream.open(fileName.data(),  fstream::app);
        //Log(LogLevel::Info, funcName, line, "Log started, min log level is %s.", minLevel.getValue().data());
    LeaveCriticalSection(&m_lockLog);
}

void Logger::Log(const LogLevel & level, const string & functionName, int line, const string & logInfo)
{
    EnterCriticalSection(&m_lockLog);

        if (level >= m_minLevel)
        {
            logDateLevelFunction(level, functionName, line);

            m_logStream<<logInfo<<endl;

            m_logStream.flush();
        }
    LeaveCriticalSection(&m_lockLog);
}



void Logger::Log(const LogLevel & level, const string & functionName, int line, char* format, ...)
{
	// wait del lock
EnterCriticalSection(&m_lockLog);
	if (level >= m_minLevel)
	{
	    logDateLevelFunction(level, functionName, line);
		char buffer[LOG_MAX_BUFFER_SIZE];
		va_list args;
		va_start(args, format);
		int len;
		if (format){
			len = _vsnprintf(buffer, LOG_MAX_BUFFER_SIZE - 1, format, args);
			if(len == -1) {
				len = LOG_MAX_BUFFER_SIZE - 1;
				buffer[LOG_MAX_BUFFER_SIZE - 1] = 0;
			}
		}
		va_end(args);
		m_logStream<<buffer<<endl;
		m_logStream.flush();
	}
LeaveCriticalSection(&m_lockLog);
}

void Logger::logDateLevelFunction(const LogLevel & level, const string & functionName, int line)
{
    SYSTEMTIME st;
    GetLocalTime(&st);

    char timeStr[9];
    GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, &st /*Puede ser null pero mejor joder lo menos posible*/,
                            "HH:mm:ss",timeStr, 8);
    timeStr[8] = 0;

    char dateStr[11];
    GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, "yyyy-MM-dd", dateStr, 10);
    dateStr[10] = 0;

    m_logStream<<dateStr<<" "<<timeStr<<".";
    if (st.wMilliseconds < 100){
        m_logStream<<"0";
        if (st.wMilliseconds < 10)
            m_logStream<<"0";
    }
    m_logStream<<st.wMilliseconds<<";"<<level<<";"<<functionName<<";"<<"Line."<<line<<";";
}

#endif
