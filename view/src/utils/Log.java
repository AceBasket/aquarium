package utils;

import java.io.*;
public class Log {
    public enum LogLevel {
        FATAL_ERROR,
        ERROR,
        WARNING,
        INFO
    }

    private static LogLevel verbosityLevel;

    public static void setVerbosityLevel(LogLevel level) {
        verbosityLevel = level;
    }

    public static void logMessage(PrintWriter logFile, LogLevel level, String message) {
        if (level == LogLevel.INFO && verbosityLevel.ordinal() <= level.ordinal()) {
            logFile.println(message);
        } else if (level == LogLevel.WARNING && verbosityLevel.ordinal() <= level.ordinal()) {
            logFile.println("WARNING: " + message);
        } else if (level == LogLevel.ERROR && verbosityLevel.ordinal() <= level.ordinal()) {
            logFile.println("ERROR: " + message);
        } else if (level == LogLevel.FATAL_ERROR) {
            if (verbosityLevel.ordinal() <= level.ordinal()) {
                logFile.println("FATAl ERROR: " + message);
            }
        }
        logFile.flush();
    }
}
