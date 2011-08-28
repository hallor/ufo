#ifndef LOGGER_H
#define LOGGER_H


namespace Logger {

class Logger
{
public:
    Logger();

    void log(const char *prefix, const char * format,...);

private:
};

extern Logger __log;
};

#define LogCrit(X,...) Logger::__log.log("Critical Error at ", "%s@%s(%i): "X,__FILE__,__PRETTY_FUNCTION__,__LINE__,##__VA_ARGS__);
#define LogError(X,...) Logger::__log.log("Error at ", "%s@%s(%i): "X,__FILE__,__PRETTY_FUNCTION__,__LINE__,##__VA_ARGS__);
#define LogWarning(X,...) Logger::__log.log("%s: "X,"Warning",##__VA_ARGS__);
#define LogInfo(X,...) Logger::__log.log("Info at ", "%s@%s(%i): "X,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);
#define LogDebug(X,...) Logger::__log.log("Debug at ", "%s@%s(%i): "X,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);

#endif // LOGGER_H
