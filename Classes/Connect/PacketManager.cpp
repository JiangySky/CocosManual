//
//  PacketManager.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "PacketManager.h"
#include "Packets/PacketAll.h"

PacketManager::~PacketManager()
{
    delete packet;
}

bool PacketManager::init()
{
    return true;
}

Packet * PacketManager::getPacket(uint packetId, bool instance)
{
    switch (packetId) {
        case PACKET_ERROR: // error info
            packet = INSTANCE(PacketError);
            break;
        case PACKET_SERVER_TIP: // server tip
            packet = INSTANCE(PacketServerTip);
            break;
        case PACKET_GET_HANDLE:
        case PACKET_GET_HANDLE_RES: // handle
            packet = INSTANCE(PacketGetHandle);
            break;
        case PACKET_TEST: // test
            if (instance) {
                packet = INSTANCE(PacketTest);
            } else {
                packet = new PacketTest();
                if (packet) {
                    packet->init();
                    packet->isInstance = false;
                }
            }
            break;
        case PACKET_HEARTBEAT: // heart beat
        case PACKET_HEARTBEAT_RES:
            packet = INSTANCE(PacketHBeat);
            break;
            
        default:
            packet = INSTANCE(Packet);
            break;
    }
    
    return packet;
}
