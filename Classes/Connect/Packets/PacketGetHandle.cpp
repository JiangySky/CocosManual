//
//  PacketGetHandle.cpp
//  HeroAppCN
//
//  Created by Jiangy on 14-4-26.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "PacketGetHandle.h"
#include "../AppConnect.h"

int PacketGetHandle::write(SocketOutputStream * oStream)
{
    int writeSize = oStream->getTail();
    // NOTE: write data to stream
    oStream->writeUShort(0);
    return oStream->getTail(writeSize) - writeSize;
}

int PacketGetHandle::read(SocketInputStream * iStream)
{
    int readSize = iStream->getHead();
    // NOTE: read data from stream
    handleId = iStream->readUShort();
    
    return iStream->getHead(readSize) - readSize;
}

int PacketGetHandle::execute()
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

