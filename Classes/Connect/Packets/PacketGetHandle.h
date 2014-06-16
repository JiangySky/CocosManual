//
//  PacketGetHandle.h
//  HeroAppCN
//
//  Created by Jiangy on 14-4-26.
//  Copyright (c) 2013 35VI. All rights reserved.
//

/**
 * #include "PacketGetHandle.h" in PacketAll.h
 * define PACKET_* in _RequestType of ConnectDefine.h
 * return INSTANCE(PacketGetHandle) at case in PacketManager::getPacket(uint packetId)
 */

#ifndef __HeroAppCN__PacketGetHandle__
#define __HeroAppCN__PacketGetHandle__

#include <iostream>
#include "../Packet.h"

class PacketGetHandle : public Packet {
    
public:
    CLASS_INSTANCE(PacketGetHandle)
    
    // NOTE: declare variable
    ushort handleId;
    
    virtual int write(SocketOutputStream * oStream);
    virtual int read(SocketInputStream * iStream);
    virtual int execute();
    
};

#endif /* defined(__HeroAppCN__PacketGetHandle__) */