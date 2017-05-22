package se.liu.ida.tdp024.account.util.logger;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import se.liu.ida.tdp024.account.util.http.HTTPHelper;
import se.liu.ida.tdp024.account.util.http.HTTPHelperImpl;

/*
 * 
 * This is an extremly simple implemenation of logger,
 * one should really consider writing a new one where
 * you implement a much bettern way of persisting logs.
 * An example would be using REST calls to Monlog.
 * 
 */
public class LoggerImpl implements Logger {
    
    private final DateFormat dateFormat;
    private Date date;
    private final HTTPHelper http;
    
    private static final String loggerAddress = "http://localhost:8050/log/";
    
    public LoggerImpl() {
        dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss:SSS");
        date = new Date();
        http = new HTTPHelperImpl();
    }
    
    @Override
    public void log(LoggerLevel level, String message)
    {
        date = new Date();
        String str = dateFormat.format(date) + ": " + level.toString() + " => " + message;
        
        System.out.println(str);
        http.get(loggerAddress, "msg", str);
    }

    /*@Override
    public void log(Throwable throwable) {
        // TODO: Fix
        System.out.println(LoggerLevel.ERROR.toString() + " => " + throwable.getMessage());
    }*/

    /*@Override
    public void log(LoggerLevel todoLoggerLevel, String shortMessage, String longMessage) {
        if (todoLoggerLevel == LoggerLevel.CRITICAL || todoLoggerLevel == LoggerLevel.ERROR) {
            System.err.println(shortMessage);
            System.err.println(longMessage);
        } else {
            System.out.println(shortMessage);
            System.out.println(longMessage);
        }
    }*/
}
