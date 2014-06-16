//
//  PacketReconnect.h
//  Hero
//
//  Created by Jiangy on 13-4-17.
//  Copyright (c) 2013 35VI. All rights reserved.
//

/**
 * #include "PacketReconnect.h" in PacketAll.h
 * define PACKET_* in _RequestType of ConnectDefine.h
 * return INSTANCE(PacketReconnect) at case in PacketManager::getPacket(uint packetId)
 */

#ifndef __Hero__PacketReconnect__
#define __Hero__PacketReconnect__

#include <iostream>
#include "../Packet.h"

class PacketReconnect : public Packet {
    
public:
    CLASS_INSTANCE(PacketReconnect)
    
    // NOTE: declare variable
    int             pId;    // 游戏角色Id
    std::string     pName;  // 游戏角色名
    int             result; // 重连成功标记
    
    virtual int write(SocketOutputStream * oStream);
    virtual int read(SocketInputStream * iStream);
    virtual int execute();
    
};

#endif /* defined(__Hero__PacketReconnect__) */