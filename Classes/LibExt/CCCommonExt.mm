//
//  CCCommonExt.m
//  CocosManual
//
//  Created by Jiangy on 14-5-22.
//
//

#import "CCCommonExt.h"
#import <SystemConfiguration/SCNetworkReachability.h>
#import <netinet/in.h>
#include "cocos2d.h"

NS_CC_BEGIN

float LabelDimHeight(const char * content, const char * fontName, float fontSize, float width)
{
    UIFont * font = [UIFont fontWithName:[NSString stringWithUTF8String:fontName] size:fontSize];
    NSString * labelContent = [NSString stringWithUTF8String:content];
    CGSize dimension = [labelContent sizeWithFont:font
                                constrainedToSize:CGSizeMake(width, NSIntegerMax)
                                    lineBreakMode:NSLineBreakByCharWrapping];
    return dimension.height;
}

void LogFile(const std::string &log)
{
    char timeNow[20];
    time_t lt;
    struct tm * tp;
    lt = time(NULL);
    tp = localtime(&lt);
    strftime(timeNow, 20, "%Y-%m-%d %T", tp);
    
    std::string path = FileUtils::getInstance()->getWritablePath();
    path.append("log.txt");
    FILE * fp = fopen(path.c_str(), "at+");
    if (fp) {
        std::string content(timeNow);
        content.append(">> ");
        content.append(log);
        content.append("\n");
        fwrite(content.c_str(), 1, content.size(), fp);
        fclose(fp);
    }
}

NetworkStatus NetworkEnable(const std::string &host/* = ""*/)
{
    if (host.compare("") == 0) {
        const int CheckAmount = 0;
        static int reCheckCount = CheckAmount;
        NetworkStatus netStatus = kNotReachable;
        // Create zero addy
        struct sockaddr_in zeroAddress;
        bzero(&zeroAddress, sizeof(zeroAddress));
        zeroAddress.sin_len = sizeof(zeroAddress);
        zeroAddress.sin_family = AF_INET;
        
        // Recover reachability flags
        SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
        SCNetworkReachabilityFlags flags;
        
        BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags); // ???: maybe crash
        CFRelease(defaultRouteReachability);
        
        if (!didRetrieveFlags) {
            printf("Error. Could not recover network reachability flags\n");
        } else if ((flags & kSCNetworkReachabilityFlagsReachable) == 0) {
            printf("Error. The target host is not reachable\n");
            
        } else if ((flags & kSCNetworkReachabilityFlagsConnectionRequired) == 0) {
            // If the target host is reachable and no connection is required then we'll assume (for now) that you're on Wi-Fi...
            netStatus = kReachableViaWiFi;
        } else  if ((((flags & kSCNetworkReachabilityFlagsConnectionOnDemand ) != 0) ||
                     (flags & kSCNetworkReachabilityFlagsConnectionOnTraffic) != 0)) {
            // and the connection is on-demand (or on-traffic) if the calling application is using the CFSocketStream or higher APIs...
            if ((flags & kSCNetworkReachabilityFlagsInterventionRequired) == 0) {
                // and no [user] intervention is needed...
                netStatus = kReachableViaWiFi;
            }
        } else if ((flags & kSCNetworkReachabilityFlagsIsWWAN) == kSCNetworkReachabilityFlagsIsWWAN) {
            // but WWAN connections are OK if the calling application is using the CFNetwork APIs.
            netStatus = kReachableViaWWAN;
        }
        
        if (netStatus == kNotReachable && reCheckCount > 0) {
            reCheckCount--;
            netStatus = kReachableViaWWAN;
        } else {
            reCheckCount = CheckAmount;
        }
        return netStatus;
        
    } else {
        // TODO: modify later
        return NetworkEnable();
    }
}

void IgnorInteractionEvents(bool ignor, float delay, bool clear/* = false*/)
{
    static int ignorCount = 0;
    if (clear) {
        ignorCount = 0;
    }
    if (ignor) {
        ignorCount++;
    } else {
        if (--ignorCount <= 0) {
            ignorCount = 0;
        } else {
            NSLog(@"IgnorInteractionEvents: %i", ignorCount);
            return;
        }
    }
    
    UIApplication * application = [UIApplication sharedApplication];
    if (([application isIgnoringInteractionEvents]) ^ ignor) {
        if (ignor) {
            if (fabs(delay - 0) < 0.01) {
                [application beginIgnoringInteractionEvents];
            } else {
                [application performSelector:@selector(beginIgnoringInteractionEvents) withObject:nil afterDelay:delay];
            }
        } else {
            if (fabs(delay - 0) < 0.01) {
                [application endIgnoringInteractionEvents];
            } else {
                [application performSelector:@selector(endIgnoringInteractionEvents) withObject:nil afterDelay:delay];
            }
        }
    }
}

const char * LocalizedString(const char * key)
{
    NSString * str = [NSString stringWithUTF8String:key];
    NSString * local = NSLocalizedString(str, @"");
    return [local UTF8String];
}


NS_CC_END
