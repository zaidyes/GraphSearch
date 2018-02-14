/**
 *
 * Minimalistic Logging Framework
 *
 */

#pragma once

#include <string>
#include <stdio.h>
#include <stdarg.h>  

static const int MAX_LOG_BUFFER = 4096;
static thread_local char __logbuffer[MAX_LOG_BUFFER];

class Log
{
    public:
        typedef enum {
            LevelDebug = -1,
            LevelInfo = 0,
            LevelWarn = 1,
            LevelError = 2,
            LevelTest = 3
        } loglevel_t;

    private:
        static loglevel_t currentLogLevel;

    public:
        static void setLogLevel(loglevel_t logLevel) {
            currentLogLevel = logLevel;
        }

        static loglevel_t getLogLevel() {
            return currentLogLevel;
        }

    public:
        static void log(int level, const char* str) {
            write(level, str);
        }

        static void log(int level, const std::string& str) {
            write(level, str.c_str());
        }

        static void logf(int level, const char* format, ...) {
            va_list args; va_start(args, format);
            writef(level, format, args);
            va_end(args);
        }

    public:
        static void debug(const char* str) {
            write(LevelDebug, str);
        }

        static void debug(const std::string& str) {
            write(LevelDebug, str.c_str());
        }

        static void debugf(const char* format, ...) {
            if (currentLogLevel > LevelDebug) return;
            va_list args; va_start(args, format);
            writef(LevelDebug, format, args);
            va_end(args);
        }

        static void info(const char* str) {
            write(LevelInfo, str);
        }

        static void info(const std::string& str) {
            write(LevelInfo, str.c_str());
        }

        static void infof(const char* format, ...) {
            if (currentLogLevel > LevelInfo) return;
            va_list args; va_start(args, format);
            writef(LevelInfo, format, args);
            va_end(args);
        }

        static void warn(const char* str) {
            write(LevelWarn, str);
        }

        static void warn(const std::string& str) {
            write(LevelWarn, str.c_str());
        }

        static void warnf(const char* format, ...) {
            if (currentLogLevel > LevelWarn) return;
            va_list args; va_start(args, format);
            writef(LevelWarn, format, args);
            va_end(args);
        }

        static void error(const char* str) {
            write(LevelError, str);
        }

        static void error(const std::string& str) {
            write(LevelError, str.c_str());
        }

        static void errorf(const char* format, ...) {
            if (currentLogLevel > LevelError) return;
            va_list args; va_start(args, format);
            writef(LevelError, format, args);
            va_end(args);
        }

        static void test(const char* str) {
            write(LevelTest, str);
        }

        static void test(const std::string& str) {
            write(LevelTest, str.c_str());
        }

        static void testf(const char* format, ...) {
            if (currentLogLevel > LevelTest) return;
            va_list args; va_start(args, format);
            writef(LevelTest, format, args);
            va_end(args);
        }

    private:
        static void write(int level, const char* str) {
            if (level < (int)currentLogLevel) return;

            switch (level) {
                case LevelDebug:
                    printf("[DEBUG] %s\n", str);
                    break;

                case LevelInfo:
                    printf("[INFO]  %s\n", str);
                    break;

                case LevelWarn:
                    printf("[WARN]  %s\n", str);
                    break;

                case LevelError:
                    printf("[ERROR] %s\n", str);
                    break;

                case LevelTest:
                    printf("[TEST] %s\n", str);
                    break;

                default:
                    break;
            }
        }

        static void writef(int level, const char* format, va_list args) {
            if (level < (int)currentLogLevel) return;
            vsprintf(__logbuffer, format, args);
            write(level, __logbuffer);
        }

};

Log::loglevel_t Log::currentLogLevel = Log::LevelDebug;
