/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "Profile.h"
#include "platform/CCFileUtils.h"
#include "cocos2d.h"
#include <time.h>

#define LOG_FILE_NAME_FMT   "PerformanceLog-%s-%s.json"
#define PLIST_FILE_NAME     "PerformanceLog.plist"

#define KEY_DEVICE              "device"
#define KEY_ENGINE_VERSION      "engineVersion"
#define KEY_RESULTS             "results"
#define KEY_CONDITION_HEADERS   "conditionHeaders"
#define KEY_RESULT_HEADERS      "resultHeaders"
#define KEY_FILE_VERSION        "fileVersion"
#define KEY_OS_VERSION          "osVersion"
#define KEY_TIMESTAMP           "timeStamp"

#define FILE_VERSION          1

#define USE_PRETTY_OUTPUT_FORMAT        0
#define USE_JSON_FORMAT                 1


// For different device & os, change these values
// TODO : get device info automatically
#define DEVICE_NAME         "DeviceName"
#define OS_VERSION          "SystemVersion"

static Profile* s_profile = nullptr;

USING_NS_CC;

// tools methods
std::string genStr(const char* format, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, format);

    char buf[MAX_LOG_LENGTH];
    vsnprintf(buf, MAX_LOG_LENGTH - 3, format, arg_ptr);

    va_end(arg_ptr);

    return buf;
}

std::vector<std::string> genStrVector(const char* str1, ...)
{
    std::vector<std::string> ret;
    va_list arg_ptr;
    const char* str = str1;
    va_start(arg_ptr, str1);
    while (nullptr != str) {
        std::string strObj = str;
        ret.push_back(strObj);
        str = va_arg(arg_ptr, const char*);
    }
    va_end(arg_ptr);
    
    return ret;
}

Profile* Profile::getInstance()
{
    if (nullptr == s_profile)
    {
        s_profile = new Profile();
    }
    
    return s_profile;
}

void Profile::destroyInstance()
{
    CC_SAFE_DELETE(s_profile);
}

Profile::Profile()
{
    
}

Profile::~Profile()
{
    
}

void Profile::testCaseBegin(std::string testName, std::vector<std::string> condHeaders, std::vector<std::string> retHeaders)
{
    curTestName = testName;

    ValueVector conds;
    for (int i = 0; i < condHeaders.size(); i++) {
        conds.push_back(Value(condHeaders[i]));
    }
    
    ValueVector rets;
    for (int j = 0; j < retHeaders.size(); j++) {
        rets.push_back(Value(retHeaders[j]));
    }
    
    auto findValue = testData.find(curTestName);
    if (findValue != testData.end())
    {
        auto curMap = findValue->second.asValueMap();
        curMap[KEY_CONDITION_HEADERS] = Value(conds);
        curMap[KEY_RESULT_HEADERS] = Value(rets);

        if (curMap.find(KEY_RESULTS) != curMap.end())
            curTestResults = curMap[KEY_RESULTS].asValueVector();
        else
            curTestResults.clear();
    }
    else
    {
        ValueMap theData;
        theData[KEY_CONDITION_HEADERS] = conds;
        theData[KEY_RESULT_HEADERS] = rets;
        testData[curTestName] = Value(theData);
        curTestResults.clear();
    }
}

void Profile::addTestResult(std::vector<std::string> conditions, std::vector<std::string> results)
{
    ValueVector curRet;

    for (int i = 0; i < conditions.size(); i++) {
        curRet.push_back(Value(conditions[i]));
    }
    
    for (int j = 0; j < results.size(); j++) {
        curRet.push_back(Value(results[j]));
    }
    curTestResults.push_back(Value(curRet));
}

void Profile::testCaseEnd()
{
    // add the result of current test case into the testData.
    ValueMap theData = testData[curTestName].asValueMap();
    theData[KEY_RESULTS] = curTestResults;
    testData[curTestName] = Value(theData);
}

void Profile::flush()
{
    // record the format version
    testData[KEY_FILE_VERSION] = Value(FILE_VERSION);
    testData[KEY_DEVICE] = Value(DEVICE_NAME);
    testData[KEY_OS_VERSION] = Value(OS_VERSION);
    testData[KEY_ENGINE_VERSION] = Value(cocos2d::cocos2dVersion());
    time_t t = time(0);
    localtime(&t);
    testData[KEY_TIMESTAMP] = Value(genStr("%ld", t));

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    std::string checkPath = "/mnt/sdcard/PerfTest";
    auto writablePath = checkPath;
    if (! cocos2d::FileUtils::getInstance()->isDirectoryExist(checkPath)) {
        auto createRet = cocos2d::FileUtils::getInstance()->createDirectory(checkPath);
        if (! createRet) {
            writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();
        }
    }
    cocos2d::log("write path : %s", writablePath.c_str());
#else
    auto writablePath = cocos2d::FileUtils::getInstance()->getWritablePath();
#endif

#if USE_JSON_FORMAT
    char timeStr[64];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d-%H%M", localtime(&t));
    std::string fileName = genStr(LOG_FILE_NAME_FMT, DEVICE_NAME, timeStr);
    std::string fullPath = genStr("%s/%s", writablePath.c_str(), fileName.c_str());

    cocos2d::JSON::Option option;
#if USE_PRETTY_OUTPUT_FORMAT
    // write pretty format json
    option._prettyPrint = true;
#else  // #else USE_PRETTY_OUTPUT_FORMAT
    // write json in one line
    option._prettyPrint = false;
#endif // #endif USE_PRETTY_OUTPUT_FORMAT
    auto out = cocos2d::JSON::encode(cocos2d::Value(testData), option);
    
    FILE *fp = fopen(fullPath.c_str(), "w");
    fputs(out.c_str(), fp);
    fclose(fp);
#else  // #else USE_JSON_FORMAT
    // Write the test data into plist file.
    std::string plistFullPath = genStr("%s/%s", writablePath.c_str(), PLIST_FILE_NAME);
    cocos2d::FileUtils::getInstance()->writeValueMapToFile(testData, plistFullPath);
#endif // #endif USE_JSON_FORMAT
}
