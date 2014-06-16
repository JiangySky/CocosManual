//
//  PacketReconnect.cpp
//  Hero
//
//  Created by Jiangy on 13-4-17.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "PacketReconnect.h"
#include "../AppConnect.h"

int PacketReconnect::write(SocketOutputStream * oStream)
{
    int writeSize = oStream->getTail();
    // NOTE: write data to stream
    oStream->writeInt(pId);
    oStream->writeString(pName);
    
    return oStream->getTail(writeSize) - writeSize;
}

int PacketReconnect::read(SocketInputStream * iStream)
{
    int readSize = iStream->getHead();
    // NOTE: read data from stream
    result = iStream->readInt();
    
    return iStream->getHead(readSize) - readSize;
}

int PacketReconnect::execute()
{
    switch (requestResult) {
        case kResultSuccess: { // Success
#ifdef DISPATCH_ENABEL
            dispatch_async(dispatch_get_main_queue(), ^{
                AppConnect::client()->onResponse(this);
            });
#elif COCOS2D_ENGINE
            AppConnect::client()->onResponse(this);
#else
            // other platform or engine
#endif
            return 1;
        }
        default: { // Failure or otherwise
            std::cout << "Response: " << requestResult << " (" + requestErrMsg + ")\n";
#ifdef DISPATCH_ENABEL
            dispatch_async(dispatch_get_main_queue(), ^{
                AppConnect::client()->onResponseError(this);
            });
#elif COCOS2D_ENGINE
            AppConnect::client()->onResponseError(this);
#else
            // other platform or engine
#endif
        }
    }
    return -1;
}

