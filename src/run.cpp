#include <iostream>
#include <string>
#include <chrono>

#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "yaml-cpp/yaml.h"

struct Address
{
    std::string ip_address;
    std::string host_url;
    std::string get_url;
    Address(std::string ip_address_, std::string host_url_, std::string get_url_): ip_address{ip_address_}, host_url{host_url_}, get_url{get_url_} {}
};

Address ReadYamlData()
{
    Address address{"","",""};

    YAML::Node config = YAML::LoadFile("resources/config.yaml");
    if (config["ip"])
        address.ip_address = config["ip"].as<std::string>();
    if (config["url"])
        address.host_url = config["url"].as<std::string>();
    if (config["get"])
        address.get_url = config["get"].as<std::string>();

    std::cout << "Welcome " << address.ip_address <<" " << address.host_url << address.get_url << std::endl;

    return address;
}

std::vector<char> vBuffer(20*1024);

void GrabSomeData(asio::ip::tcp::socket& socket)
{
    socket.async_read_some(asio::buffer(vBuffer.data(), vBuffer.size()),
        [&](std::error_code ec, std::size_t length)
        {
            if(!ec)
            {
                std::cout << "\n\nRead" << length << " bytes\n\n";

                for (int i = 0; i < length; i++)
                    std::cout << vBuffer[i];

                GrabSomeData(socket);
            }
        }

    );
}

int run()
{
    Address address = ReadYamlData();

    asio::error_code ec;

    // Create a "context" - assentially the platform specific interface
    asio::io_context context;

    // Give some fake tasks to asio so the context doesn't finish
    // asio::io_context::work idleWork{context};

    asio::executor_work_guard<asio::io_context::executor_type> idleWork= asio::make_work_guard(context);

    std::thread thrContext = std::thread([&]() { context.run(); });

    // Get the address
    asio::ip::tcp::endpoint endpoint(asio::ip::make_address(address.ip_address, ec), 80);

    // Create a socket, the context will deliver the implementation
    asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if(!ec)
    {
        std::cout << "Connected!" << std::endl;
    }
    else
    {
        std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
    }

    if (socket.is_open())
    {
        GrabSomeData(socket);

        using namespace std;

        std::string sRequest{
            "GET "s+
            address.get_url+
            " HTTP/1.1\r\n"s+
            "Host: "s+
            address.host_url+
            "\r\n"s+
            "Connection: close\r\n\r\n"s
        };

        socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

        // socket.wait(socket.wait_read);

        // size_t bytes = socket.available();
        // std::cout << "Bytes Available: " << bytes << std::endl;

        // if(bytes > 0)
        // {
        //     std::vector<char> vBuffer(bytes);
        //     socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);
        //     for (auto c : vBuffer)
        //         std::cout << c;
        //
        // }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(5000ms);
    }

    return 0;
}
