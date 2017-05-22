package se.liu.ida.tdp024.account.util.logger;

public interface Logger {
    
    enum LoggerLevel {
        DEBUG,
        INFO,
        NOTIFY,
        WARNING,
        ERROR,
        CRITICAL,
        ALERT,
        EMERGENCY;
    }
    
    //public void log(Throwable throwable);
    
    //public void log(LoggerLevel todoLoggerLevel, String shortMessage, String longMessage);
    
    public void log(LoggerLevel level, String message);
    
}
