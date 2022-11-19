#include <artemis_channels.h>

namespace
{
    Artemis::Teensy::Radio::RFM23 rfm23(RFM23_CS_PIN, RFM23_INT_PIN, hardware_spi1);
    PacketComm packet;
}

void Artemis::Teensy::Channels::rfm23_channel()
{
    while (!rfm23.init())
        ;

    while (true)
    {
        if (PullQueue(packet, rfm23_queue, rfm23_queue_mtx))
        {
            switch (packet.header.type)
            {
            case PacketComm::TypeId::CommandPing:
            case PacketComm::TypeId::CommandEpsSwitchName:
            {
                rfm23.send(packet);
                threads.delay(500);
                break;
            }
            default:
                break;
            }
        }

        if (rfm23.recv(packet))
        {
            Serial.print("[RFM23] RECEIVED: [");
            for (size_t i = 0; i < packet.data.size(); i++)
            {
                Serial.print(packet.data[i], HEX);
            }
            Serial.println("]");
        }
        threads.delay(10);
    }
}
