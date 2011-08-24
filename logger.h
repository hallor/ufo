#ifndef LOGGER_H
#define LOGGER_H


class Logger
{
public:
    Logger();

    void log(const char *prefix, const char * format,...);

private:
};

extern Logger log;

#define LogCrit(X,...) log.log("Critical Error at ", "%s@%s(%i): "X,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);
#define LogError(X,...) log.log("Error at ", "%s@%s(%i): "X,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);
#define LogInfo(X,...) log.log("Info at ", "%s@%s(%i): "X,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);
#define LogDebug(X,...) log.log("Debug at ", "%s@%s(%i): "X,__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);

#endif // LOGGER_H
