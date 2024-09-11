#include <iostream>
#include <net_common/olc_net.h>

enum class CustomMsgTypes : uint32_t
{
    FireBullet,
    MovePlayer
};

int run()
{
    net_common::olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::FireBullet;

    int a = 1;
    bool b = true;
    float c = 3.14159f;

    struct
    {
        float x;
        float y;
    } d[5]; //array of 5 elements

    msg << a << b << c << d;

    a = 99;
    b = false;
    c = 99.0f;

    msg >> d >> c >> b >> a;

    std::cout << a << " " << b << " " << c << std::endl;


    return 0;
}
