#include <iostream>

#include <SharedConstants.hpp>
#include <messages.pb.h>
#include <CommandHash.hpp>
#include <rsa_cpp/random_engine.hpp>
#include <rsa_cpp/rsa.hpp>
#include <exception>
#include <cstddef>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/address.hpp>
#include <memory>

static std::string gen_truly_random_bytes();

static void send_data(std::string str);

int main(const int argc, const char *const *const argv)
{
    try
    {
        if (argc != 3)
        {
            std::cout << "Usage: " << "client.exe RSA_private_key_d command_string" << std::endl;
            return 1;
        }

        const boost::multiprecision::cpp_int private_key_d{ argv[1] };
        const std::string command_string{ argv[2] };
        auto command = std::make_unique<Command>();
        command->set_command(command_string);
        command->set_unique_identifier(gen_truly_random_bytes());
        const boost::multiprecision::cpp_int message_hash = hash(*command);
        const auto signature_opt{
            cryptb::rsa{
                boost::multiprecision::cpp_int{ g_public_key.e },
                boost::multiprecision::cpp_int{ private_key_d },
                boost::multiprecision::cpp_int{ g_public_key.N }
                }.sign(message_hash)
        };
        if (!signature_opt)
        {
            std::cout << "Failed to sign the message" << std::endl;
            return -1;
        }
        VerifiedCommand verified_command;
        verified_command.set_allocated_command(command.release());
        const std::vector<std::uint8_t> serialized_signature_vec = to_bytes(signature_opt.value());
        std::string serialized_signature_str = std::string{ reinterpret_cast<const char*>(serialized_signature_vec.data()), serialized_signature_vec.size() };
        verified_command.set_signature(serialized_signature_str);
        const std::string data_to_send = verified_command.SerializeAsString();
        std::cout << "Sending the command. Data is " << data_to_send.size() << " bytes long" << std::endl;
        send_data(data_to_send);
    }
    catch (std::exception& e)
    {
        std::cout << "Exception caught in main: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}

std::string gen_truly_random_bytes()
{
    const auto gen_truly_random_bytes = []() -> std::array<std::uint8_t, 208> {
        // Default constructor creates truly random seed
        cryptb::random_engine random_engine{};
        return random_engine.gen_truly_random_bytes();
    };
    
    const auto random_bytes = gen_truly_random_bytes();

    // Purposefully make the first-byte null so that nobody assumes that the string is a C-style string
    std::string result = "\0";
    result.reserve(random_bytes.size() + 1);
    for (std::uint8_t byte : random_bytes)
    {
        result.push_back(static_cast<char>(byte));
    }
    return result;
}

void send_data(std::string str)
{
    const auto io_context = std::make_shared<boost::asio::io_context>();
    // Send the data to UDP port g_port_number
    const boost::asio::ip::udp::endpoint endpoint{ boost::asio::ip::address::from_string("127.0.0.1"), g_port_number };
    const auto socket = std::make_shared<boost::asio::ip::udp::socket>(*io_context);
    socket->open(boost::asio::ip::udp::v4());
    socket->send_to(boost::asio::buffer(str), endpoint);
}
